//
// Created by cjy on 2017/3/21.
//

#ifndef MESSAGE_LINUXSERVER_MSG_H
#define MESSAGE_LINUXSERVER_MSG_H

#include "msgType.h"

typedef struct _Msg{
    unsigned int m_uiCheckCrc;            //消息校验 0xAFAFAFAF
    MsgType m_eMsgType;				   		//消息类型
    int m_iMsgLen;							//消息长度
    char m_aMsgData[0];						//消息内容
}Msg, *pMsg;

//确定成功消息
int sendAckOkMsg(int sockfd);
//确定失败消息
int sendAckErrorMsg(int sockfd);

//发送消息
int sendMsg(int sockfd, Msg *msg);

//收到消息
int recvMsg(Msg *msg);

#endif //MESSAGE_LINUXSERVER_MSG_H