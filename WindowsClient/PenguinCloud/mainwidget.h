﻿#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include<QMap>
#include <QStack>
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
class DownloadManage;

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


signals:
    void paste(bool);
public slots:
    void recvFileLists(QByteArray byteArray);
    void recvUploadFile(UploadMsg uploadMsg);
    void recvDownloadFile_readyReadDownloadMsg(DownloadMsg downloadMsg);
    void getDir(QString dirname);
    void previousDir();
    void uploadFile();
    void doloadFile_download();
    void newFolder(const QString &folderName);
    void removeFileOrFolder(const QString &path);
    void rename(const QString &newName, const QString &oldName);
    void renameError(RenameMsg r);
    void show_download_manage();
    void removeSelected();
    void copySelectFilesOrFolder(const QStringList &path);
    void moveSelectFilesOrFolder(const QStringList &path);
    void pasteSelected();
    void errorHandle(ErrorMsg msg);                                         // 错误处理
    void preview(const QString &path);
    void show_prview(PreviewArray previewMsg);
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
    DownloadManage *load;


    QLabel *sys_info;
    QPushButton *download_manage;
    QString m_stUserName;

    QStack<QString> path;// current dir
    QMap<QString, QFileInfo *> *m_pFileMap;
    QStringList wholeCopyPath; // 保存所有将要复制的文件的具体路径
    bool isCopy; // true是复制， false是移动
};

#endif // MAINWIDGET_H
