#include <QApplication>
#include <QFile>
#include "loginwidget.h"
#include "mainwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/resource/qss/style.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(file.readAll());
    }
    LoginWidget w;
    w.show();
//    MainWidget w;s
//    w.show();

    return a.exec();
}
