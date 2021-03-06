﻿#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

#include <QTcpSocket>

#include"msgtype.h"


//#define SERVER_IP "182.254.219.254"
//#define SERVER_IP "192.168.1.155"
//#define SERVER_IP "192.168.89.129"
#define SERVER_IP "120.24.84.247"

#define MAX_SEND_BUF (64 * 1024)
#define MAX_READ_BUF MAX_SEND_BUF
#define FILE_SEND 1024

typedef struct _Msg{
    unsigned int m_uiCheckCrc;              //消息校验 0xAFAFAFAF
    MsgType m_eMsgType;				   		//消息类型
    int m_iMsgLen;							//消息长度
    char m_aMsgData[0];						//消息内容
}Msg, *pMsg;

class AbstractNetwork: public QTcpSocket
{
    Q_OBJECT
public:
    AbstractNetwork(QObject *parent = nullptr);
    virtual ~AbstractNetwork();

    //发送消息
    //确定成功消息
    virtual int sendAckOkMsg();
    //确定失败消息
    virtual int sendAckErrorMsg(ErrorMsg errorMsg);
    //准备就绪消息
    virtual int sendReadyMsg();
    //登录消息
    virtual int sendLoginMsg(LoginMsg loginMsg);
    //文件列表消息
    virtual int sendFileListMsg(FileListsMsg fileListsMsg);
    //预览信息消息
    virtual int sendPreviewMsg(PreviewMsg previewMsg);
    //下载信息消息
    virtual int sendDownloadMsg(DownloadMsg downloadMsg);
    //移动消息
    virtual int sendMoveMsg(MoveMsg moveMsg);
    //复制消息
    virtual int sendCopyMsg(CopyMsg copyMsg);
    //新建文件夹消息
    virtual int sendNewFolderMsg(NewFolderMsg newFolderMsg);
    //重命名消息
    virtual int sendRenameMsg(RenameMsg renameMsg);

    //上传操作消息
    virtual int sendUploadMsg(UploadMsg uploadMsg);
    //删除操作消息
    virtual int sendDeleteMsg(DeleteMsg deleteMsg);
    //分享操作消息
    virtual int sendShareMsg(ShareMsg shareMsg);
    //退出消息
    virtual int sendExitMsg(ExitMsg exitMsg);
    //发送注册信息
    virtual int sendRegisterMsg(RegisterMsg registerMsg);
    //发送类型文件 消息
    virtual int sendFileTypeMsg(FileTypeListMsg fileTypeListMsg);

    //收到消息
    //确定成功消息
    virtual void recvAckOkMsg(Msg *msg);
    //确定失败消息
    virtual void recvAckErrorMsg(Msg *msg);
    //准备就绪消息
    virtual void recvReadyMsg(Msg *msg);
    //登录消息
    virtual void recvLoginMsg(Msg *msg);
    //文件列表消息
    virtual void recvFileListMsg(Msg *msg);
    //下载消息
    virtual void recvDownloadMsg(Msg *msg);
    //移动消息
    virtual void recvMoveMsg(Msg *msg);
    //新建文件夹消息
    virtual void recvNewFolderMsg(Msg *msg);
    //重命名消息
    virtual void recvRenameMsg(Msg *msg);
    //上传操作消息
    virtual void recvUploadMsg(Msg *msg);
    //删除操作消息
    virtual void recvDeleteMsg(Msg *msg);
    //分享操作消息
    virtual void recvShareMsg(Msg *msg);
    //退出消息
    virtual void recvExitMsg(Msg *msg);
    //注册响应消息
    virtual void recvRegisterStatusMsg(Msg *msg);
    //预览响应详细
    virtual void recvPreviewStatusMsg(Msg *msg);

    virtual void recvFileTypeListReponse(Msg *msg);
signals:
    //消息信号
    void readyReadAckOKMsg();
    void readyReadAckErrorMsg(ErrorMsg);
    void readyReadReadyMsg(ReadyMsg);
    void readyReadLoginMsg(LoginMsg);
    void readyReadFileListMsg(QByteArray);
    void readyReadDownloadMsg(DownloadMsg);
    void readyReadMoveMsg(MoveMsg);
    void readyReadNewFolderMsg(NewFolderMsg);
    void readyReadRenameMsg(RenameMsg);
    void readyReadUploadMsg(UploadMsg);
    void readyReadDeleteMsg(DeleteMsg);
    void readyReadShareMsg(ShareMsg);
    void readyReadExitMsg(ExitMsg);
    void readyReadRegisterStatusMsg(RegisterStatus);
    void readyReadPreviewStatusMsg(PreviewStatus);
    void readyReadFileTypeListResponse(QByteArray);
protected:
    //sendMsg是纯虚函数，需要在子类中实现
    virtual int sendMsg(Msg *msg);
    virtual void recvMsg(Msg *msg);

    int m_iMsgStructLen;
};

#endif // ABSTRACTNETWORK_H
