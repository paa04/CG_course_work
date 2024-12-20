#include "Diamond_Square.h"
#include "experement.h"
#include "Fixed_Avg.h"
#include "MainWindow.h"
#include "Plane.h"
#include "Render.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    MainWindow main_window;
    main_window.render_scene();
    main_window.show();

    // experement::do_exp();

    return QApplication::exec();
}
