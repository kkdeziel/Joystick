#include "joystickprototype.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JoystickPrototype w;
    w.show();

    return a.exec();
}
