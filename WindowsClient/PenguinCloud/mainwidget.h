﻿#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include<QMap>

#include "basicwidget/basicwidget.h"
#include "network/msgtype.h"
#include "basiccontrol/filetablewidget.h"

class QLabel;
class QPushButton;
class QToolButton;
class QListView;
class QTableView;
class ConnectToServer;
class QFileInfo;

class MainWidget : public BasicWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

    QString getUserName() const;
    void setUserName(const QString &UserName);

    QString getPrePath() const;
    void setPrePath(const QString &PrePath);

    QString getCurrentPath() const;
    void setCurrentPath(const QString &CurrentPath);

    void replyFileLists(const QString &FolderPath);

protected:
    void paintEvent(QPaintEvent *event);

protected slots:
    void recvFileLists(QByteArray byteArray);
    void getDir(QString dirname);
    void previousDir();
    void uploadFile();
private:
    void init();
    void setListViewItem();
    void setFileTable();

    ConnectToServer *m_pConnectToServer;

    QLabel *label; // 显示图标
    QPushButton *download;
    QPushButton *upload;
    QPushButton *dele;
    QPushButton *share;
    QPushButton *previous;
    QListView *listView;
    FileTableWidget *tableWidget;

    QString m_stUserName;
    QString m_stPrePath;
    QString m_stCurrentPath;

    QMap<QString, QFileInfo *> *m_pFileMap;
};

#endif // MAINWIDGET_H
