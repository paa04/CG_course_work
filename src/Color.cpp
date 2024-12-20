//
// Created by paa04 on 13.11.24.
//

#include "Color.h"

#include <iostream>

std::map<double, std::vector<double> > Color::color_map_;

void Color::init()
{
    std::ifstream in("../color_data/geo_smooth");

    for (int i = 0; i < 29; ++i)
    {
        double key, r, g, b;
        in >> key >> r >> g >> b;
        color_map_[key] = {r, g, b};
    }

    in.close();
}

QColor Color::get_color_by_height(double height, double max_height, double min_height)
{

    height = (height - min_height) / (max_height - min_height);

    auto upper_color_it = color_map_.lower_bound(height);

    std::vector<double> color;

    if (upper_color_it != color_map_.end() && upper_color_it != color_map_.begin())
    {
        auto lower_color_it = std::prev(upper_color_it);
        double lower_height = lower_color_it->first;
        double upper_height = upper_color_it->first;

        double u = (height - lower_height) / (upper_height - lower_height);

        const std::vector<double>& lower_color = lower_color_it->second;
        const std::vector<double>& upper_color = upper_color_it->second;

        color = {
            u * (upper_color[0] - lower_color[0]) + lower_color[0],
            u * (upper_color[1] - lower_color[1]) + lower_color[1],
            u * (upper_color[2] - lower_color[2]) + lower_color[2],
        };
    }
    else if (upper_color_it == color_map_.end())
    {
        color = color_map_.rbegin()->second;
    }
    else if (upper_color_it == color_map_.begin())
    {
        color = upper_color_it->second;
    }

    return QColor(
        static_cast<int>(color[0] * 255),
        static_cast<int>(color[1] * 255),
        static_cast<int>(color[2] * 255)
    );
}

