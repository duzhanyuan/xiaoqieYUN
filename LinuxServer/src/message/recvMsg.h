#ifndef LINUXSERVER_MESSAGE_RECVMSG_H
#define LINUXSERVER_MESSAGE_RECVMSG_H

#include "msg.h"
#include "msgType.h"


//收到消息
void recvMsg(int sockfd, Msg *msg);

//确定成功消息
void recvAckOkMsg(int sockfd, Msg *msg);
//确定失败消息
void recvAckErrorMsg(int sockfd, Msg *msg);
//收到准备就绪消息
void recvReadyMsg(int sockfd, Msg *msg);
//登录消息
void recvLoginMsg(int sockfd, Msg *msg);
//文件列表消息
void recvFileListMsg(int sockfd, Msg *msg);
//预览信息消息
void recvPreviewMsg(int sockfd, Msg *msg);
//下载消息
void recvDownloadMsg(int sockfd, Msg *msg);
//移动消息
void recvMoveMsg(int sockfd, Msg *msg);
//新建文件夹消息
void recvNewFolderMsg(int sockfd, Msg *msg);
//上传操作消息
void recvUploadMsg(int sockfd, Msg *msg);
//删除操作消息
void recvDeleteMsg(int sockfd, Msg *msg);
//分享操作消息
void recvShareMsg(int sockfd, Msg *msg);
//退出消息
void recvExitMsg(int sockfd, Msg *msg);

#endif //LINUXSERVER_MESSAGE_RECVMSG_H
