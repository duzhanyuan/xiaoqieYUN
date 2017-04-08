#ifndef LINUXSERVER_MESSAGE_MSGTYPE_H
#define LINUXSERVER_MESSAGE_MSGTYPE_H

//消息类型
typedef enum _MsgType{
    Ack_OK = 0,         //确定成功
    Ack_Error,          //确定错误
    Ack_Ready,          //准备就绪
    Put_Login,          //登录消息
    Get_FileLists,      //文件列表
    Get_Preview,        //文件预览
    Get_Download,       //文件下载
    Put_Move,           //移动操作
    Put_Copy,           //复制操作
    Put_NewFolder,      //新建文件夹
    Put_Rename,         //重命名
    Put_Upload,         //上传操作
    Put_Delete,         //删除操作
    Put_Share,          //分享操作
    Ack_Exit            //退出
}MsgType;

//错误类型
typedef enum _ErrorType{
    DataPackError = 0,        //错误得数据包
    NoSuchFileOrDirectory,    //没有这个文件或者目录
    CreateFolderFailed,       //创建文件夹失败
    RenameError               //重命名错误， 客户端收到这个错误时，做文件名以存在处理
}ErrorType;
//发生了错误
typedef struct _ErrorMsg{
    ErrorType m_eErrorType;
}ErrorMsg, *pErrorMsg;

//准备就绪得消息结构
typedef struct _ReadyMsg{
    int m_iClientSockfd;                 //客户端描述字
    unsigned int m_uiClientnaddress;     //客户端网络地址
}ReadyMsg, *pReadyMsg;


#define LOGIN_SUCCESS 1
#define PUT_LOGIN 0
#define LOGIN_FAILED_PASSWD -1
#define LOGIN_FAILED_USERNAME -2
//登录消息结构
typedef struct _LoginMsg{
    int m_iLoginStatus;         //登录状态
    char m_aUserName[10];       //用户名
    char m_aUserPass[16];       //登录密码
}LoginMsg, *pLoginMsg;

#define MAX_PATH 1024
#define MAX_JSON_SIZE 1024
//文件列表消息结构
typedef struct _FileListsMsg{
    char m_aFolderPath[MAX_PATH + 1];
    char m_jsonFileList[MAX_JSON_SIZE + 1];
}FileListsMsg, *pFileListsMsg;

//预览信息消息结构
typedef struct _PreviewMsg{

}PreviewMsg, *pPreviewMsg;

//下载信息消息结构
typedef struct _DownloadMsg{
    int m_iLoginStatus;            //下载状态
    char filePath[64];           //文件路径
    char fileName[64];             //文件名
    unsigned int serverFileIP;     //服务器地址
    unsigned short serverFilePort; //服务器端口
}DownloadMsg, *pDownloadMsg;

//移动消息结构
typedef struct _MoveMsg{
    char sourcePath[MAX_PATH + 1];          // 源路径
    char DestinationPath[MAX_PATH + 1];    //目的地址
}MoveMsg, *pMoveMsg;

//复制消息结构
typedef struct _CopyMsg{
    char sourcePath[MAX_PATH + 1];          // 源路径
    char DestinationPath[MAX_PATH + 1];    //目的地址
}CopyMsg, *pCopyMsg;

//新建文件夹消息结构
typedef struct _NewFolderMsg{
    char folderName[MAX_PATH + 1]; // 新建文件的路径（包括文件夹名）
}NewFolderMsg, *pNewFolderMsg;

//重命名
typedef struct _RenameMsg{
    char newName[MAX_PATH + 1]; // 重命名文件(文件夹)
    char oldName[MAX_PATH + 1]; //old name
}RenameMsg, *pRenameMsg;

//上传操作消息结构
typedef struct _UploadMsg{
    int m_iLoginStatus;            //上传状态
    char uploadPath[64];           //上传路径
    char fileName[64];             //文件名
    unsigned int serverFileIP;     //服务器地址
    unsigned short serverFilePort; //服务器端口
}UploadMsg, *pUploadMsg;

//删除操作消息结构
typedef struct _DeleteMsg{
    char path[MAX_PATH + 1];
}DeleteMsg, *pDeleteMsg;

//分享操作消息结构
typedef struct _ShareMsg{

}ShareMsg, *pShareMsg;

//退出消息结构
typedef struct _ExitMsg{

}ExitMsg, *pExitMsg;


#endif //LINUXSERVER_MESSAGE_MSGTYPE_H
