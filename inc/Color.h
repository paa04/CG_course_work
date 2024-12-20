//
// Created by paa04 on 13.11.24.
//

#ifndef COLOR_H
#define COLOR_H
#include <fstream>
#include <map>
#include <qcolor.h>
#include <vector>


class Color
{
private:
    static std::map<double, std::vector<double> > color_map_;

public:
    Color() = delete;

    static void init();

    static QColor get_color_by_height(double height, double max_height, double min_height);
};


#endif //COLOR_H
