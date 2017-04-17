﻿#include <QApplication>
#include <QFile>

#include "mainwidget.h"
#include "loginwidget.h"
#include "registerwidget.h"
#include "basicwidget/mymessagebox.h"
#include "basiccontrol/pdfwidget.h"
#include "basiccontrol/imagepreview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/resource/qss/style.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(file.readAll());
    }


//    ImagePreView w;
    LoginWidget w;

//    PdfWidget w;
//    ManageWidget w;
//    RegisterWidget w;
//    MyMessageBox w;
    w.show();
//    MainWidget w;s
//    w.show();

    return a.exec();
}
