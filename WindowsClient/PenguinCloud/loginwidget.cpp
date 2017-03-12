﻿#include "loginwidget.h"
#include <QComboBox>
#include <QLabel>
#include <QFile>
#include <QDebug>
#include <QCheckBox>

#include "logic/rwfile.h"
LoginWidget::LoginWidget(QWidget *parent) : BasicWidget(parent)
{

    this->setBackgroundColor("#0698f5");
    resize(320,400);
    init();
    setStyleSheetFromFile(":/resource/qss/login.qss");

    RWFile rw(":/password.txt");
    QPair<QString, QString> info = rw.read();
    if(!info.first.isEmpty())
        username_input->setText(info.first);
    if(!info.second.isEmpty())
        password_input->setText(info.second);

}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::init()
{

    label_title = new QLabel(this);
    QPixmap pix(":/resource/image/title.png");
    label_title->setPixmap(pix);
    label_title->move((this->width() - pix.width()) /2, 60);
    username_input = new LineEdit(this);
    username_input->setLeftPixmap(":/resource/image/username.png");
    password_input = new LineEdit(this);
    password_input->setLeftPixmap(":/resource/image/password.png");
    login_button = new QPushButton(QString::fromLocal8Bit("登录"),this);
    login_button->resize(200,30);
    remember_password = new QCheckBox(QString::fromLocal8Bit("记住密码"), this);
    find_passord = new QLabel(QString::fromLocal8Bit("找回密码"), this);

    //绝对定位
    username_input->move((this->width() - username_input->width()) /2, 150);
    password_input->move((this->width() - password_input->width()) /2 ,183);
    login_button->move((this->width() - login_button->width()) /2 ,250);
    remember_password->move(75, 225);
    find_passord->move(180, 225);
    find_passord->setObjectName("find_passord");
    //默认配置
    username_input->setPlaceholderText(QString::fromLocal8Bit("账号"));
    password_input->setPlaceholderText(QString::fromLocal8Bit("密码"));
    password_input->setEchoMode(QLineEdit::Password);
    login_button->setObjectName("login_button");

}
void LoginWidget::setStyleSheetFromFile(const QString &filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());
        qDebug() << this->styleSheet();
    }
}
