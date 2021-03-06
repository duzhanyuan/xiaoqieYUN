#include "serverOperating.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h> // mkdir
#include <dirent.h>
#include <stdbool.h>
#include "../cjson/cJSON.h"
#include "../message/nstdmsgio.h"
#include "../database/database.h"

/**
 * 获取目录下的文件
 * @param dir
 * @return
 */
char *getDirFileLists(char *userDir)
{
    char sysDir[1024] = "/var/penguin/";
    strcat(sysDir, userDir);

    DIR *dir = NULL;
    struct dirent *child = NULL;
    struct stat statBuf;

    dir = opendir(sysDir);
    if(!dir)
    {
#ifdef Debug
        fprintf(stderr, "getDirFileLists : %s \n", strerror(errno));
#endif
        return NULL;
    }

    cJSON *pRoot = cJSON_CreateObject();
    cJSON *pArray = cJSON_CreateArray();
    cJSON_AddItemToObject(pRoot, userDir, pArray);

    cJSON *pItem = NULL;

    chdir(sysDir);

    while((child = readdir(dir)) != NULL)
    {
        lstat(child->d_name, &statBuf);

        if (S_ISDIR(statBuf.st_mode))
        {
            //目录
            if(strcmp(child->d_name,".")==0 || strcmp(child->d_name,"..")==0)
                continue;

            pItem = cJSON_CreateObject();
            cJSON_AddStringToObject(pItem, "name", child->d_name);
            cJSON_AddStringToObject(pItem, "type", "folder");
            cJSON_AddNumberToObject(pItem, "lastmodifytime", statBuf.st_mtim.tv_sec);
        }
        else if(S_ISREG(statBuf.st_mode))
        {
            //文件
            pItem = cJSON_CreateObject();
            cJSON_AddStringToObject(pItem, "name", child->d_name);
            cJSON_AddStringToObject(pItem, "type", "file");
            cJSON_AddNumberToObject(pItem, "lastmodifytime", statBuf.st_mtim.tv_sec);
            cJSON_AddNumberToObject(pItem, "size", statBuf.st_size);
        }
        cJSON_AddItemToArray(pArray, pItem);
    }

    chdir("-");
    closedir(dir);

#ifdef Debug
    fprintf(stdout, "%s\n", cJSON_Print(pRoot));
#endif

    char* szOut = cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);

    return szOut;
}

//文件上传的线程
void *uploadFileThread(int sockfd, void *arg)
{
    char username[64]; 
    int i = 0;
    Msg *msg = (Msg *)arg;
  
    UploadMsg uploadMsg;
    memset(&uploadMsg, 0, sizeof(UploadMsg));

    memcpy(&uploadMsg, msg->m_aMsgData, msg->m_iMsgLen);

    char file[1024] = "/var/penguin/";
    strcat(file, uploadMsg.fileName);
	
    printf("fileName %s\n", uploadMsg.fileName);
 
//  获取用户名
    for (; ;++i)
    {
	if(uploadMsg.fileName[i] == '/')
	    break;
	username[i] = uploadMsg.fileName[i];
    }
//  在这里放入数据库
    addFileToDatabase(username, file);

    //二进制可写方式追加
    FILE *fp = fopen(file, "wb");

    if(fp == NULL)
    {
#ifdef Debug
        fprintf(stdout, "fopen %s\n", strerror(errno));
#endif
        return NULL;
    }

    //long long currentFileSeek = fseek(fp, 0, SEEK_END);
    long long currentFileSeek = 0LL;
    printf("currentFileSeek = %lld\n", currentFileSeek);

    char *recvBuf = (char *)malloc(MAX_RECV_BUF + 1);
    memset(recvBuf, 0, MAX_RECV_BUF);

    int ret = 0;

    //把当前文件服务器文件索引发送给客户端
    uploadMsg.m_llCurrentSize = 0;
    sendUploadMsg(sockfd, uploadMsg);

    printf("waitting \n");
    while((ret = recv(sockfd, recvBuf, MAX_RECV_BUF, 0)) > 0)
    {
        printf("ret = %d\n", ret);
        currentFileSeek += ret;
        fwrite(recvBuf, sizeof(char), ret, fp);
        memset(recvBuf, 0, ret);

        //把当前文件服务器文件索引发送给客户端
        uploadMsg.m_llCurrentSize = currentFileSeek;
        sendUploadMsg(sockfd, uploadMsg);
    }

    printf("stop \n");

    fclose(fp);
    free(recvBuf);
    shutdown(sockfd, 0);
    close(sockfd);
    return NULL;
}

//文件下载
void *downloadFileThread(int sockfd, void *arg)
{
    Msg *msg = (Msg *)arg;

    DownloadMsg downloadMsg;
    memset(&downloadMsg, 0, sizeof(DownloadMsg));

    memcpy(&downloadMsg, msg->m_aMsgData, msg->m_iMsgLen);

    char file[1024] = "/var/penguin/";
    strcat(file, downloadMsg.fileName);

    printf("fileName %s\n", downloadMsg.fileName);
    printf("file %s\n", file);
    printf("m_llCurrentSize %d\n", downloadMsg.m_llCurrentSize);

    //可写权限打开
    FILE *fp = fopen(file, "rb");
    if(fp == NULL)
    {
#ifdef Debug
        fprintf(stdout, "fopen %s\n", strerror(errno));
#endif
        return NULL;
    }

    //long long currentFileSeek = lseek(fp, downloadMsg.m_llCurrentSize, SEEK_CUR);

    char *recvBuf = (char *)malloc(MAX_RECV_BUF + 1);
    memset(recvBuf, 0, MAX_RECV_BUF);

    int ret = 0;

    while(!feof(fp))
    {
        ret = fread(recvBuf, sizeof(char), MAX_RECV_BUF, fp);
        //currentFileSeek += ret;
        printf("ret = %d\n", ret);
        //printf("currentFileSeek = %d\n", currentFileSeek);
        send(sockfd, (void *)recvBuf, ret, 0);
        memset(recvBuf, 0, ret);
    }

    free(recvBuf);
    fclose(fp);
    shutdown(sockfd, 0);
    return NULL;
}

//新建文件夹
// 失败返回-1 成功返回0
int createNewFolder(char *folderPath)
{
    //用户及用户组可读写
    char sysDir[1024] = "/var/penguin/";
    strcat(sysDir, folderPath);

    printf("%s\n", sysDir);

    return mkdir(sysDir,  S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP);
}


/**
 *  删除操作  包括5个函数
 */

//判断是否为目录
bool is_dir(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat返回文件的信息，文件信息存放在stat结构中
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR宏，判断文件类型是否为目录
    }
    return false;
}

//判断是否为常规文件
bool is_file(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//判断文件是否为常规文件
    return false;
}

//判断是否是特殊目录
bool is_special_dir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径
void get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

//递归删除
int delete_file(const char *username, const char *path)
{
    DIR *dir;
    struct dirent *dir_info;
    char file_path[PATH_MAX];
    if(is_file(path))
    {
    
	delFileFromDatabase(username, path);
    	remove(path);
        return 0;
    }
    if(is_dir(path))
    {
        if((dir = opendir(path)) == NULL)
            return 0;
        while((dir_info = readdir(dir)) != NULL)
        {
            get_file_path(path, dir_info->d_name, file_path);
            if(is_special_dir(dir_info->d_name))
                continue;
            delete_file(username, file_path);
            rmdir(file_path);
        }
    }

    return rmdir(path);

}

//删除文件夹
int removeFolder(char *path)
{
    char sysDir[1024] = "/var/penguin/";
    char username[64];
    int i = 0;
//  获取用户名
    
    for (; ;++i)
    {
	if(path[i] == '/')
		 break;
	username[i] = path[i];
    }                                         
    strcat(sysDir, path);
    return delete_file(username, sysDir);
}


//重命名
int renameFileOrFolder(char *_oldpath, char *_newpath)
{
    char newpath[1024] = "/var/penguin/";
    char oldpath[1024] = "/var/penguin/";

    strcat(oldpath, _oldpath);
    strcat(newpath, _newpath);

    return rename(oldpath, newpath);
}

int moveSrcToDes(char *src, char *des)
{
    char srcPath[1024] = "/var/penguin/";
    char desPath[1024] = "/var/penguin/";
    char command[2048] = "mv ";

    strcat(srcPath, src);
    strcat(desPath, des);

    strcat(command, srcPath);
    strcat(command, " ");
    strcat(command, desPath);

    printf("%s\n", command);

    return system(command);
}


int copySrcToDes(char *src, char *des)
{
    char srcPath[1024] = "/var/penguin/";
    char desPath[1024] = "/var/penguin/";
    char command[2048] = "cp -r ";        //递归复制

    strcat(srcPath, src);
    strcat(desPath, des);

    strcat(command, srcPath);
    strcat(command, " ");
    strcat(command, desPath);

    printf("%s\n", command);

    return system(command);
}


int registerUser(const char *username, const char *password)
{
	UserStatus us = regis(username, password);
	if(us == MYSQL_REGISTER_SUCCESS)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int loginUser(const char *username, const char *password)
{
	UserStatus us = login(username, password);

	if (us = MYSQL_LOGIN_SUCCESS)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

//preview

//thread 

typedef struct _argument
{
	int sockfd;
	char user[512];
	char path[1024];
}argument;

void *thread_convert(void *arg)
{
	int ret;
	char command[1024];
	argument *argu;
	
	argu = (argument*)arg;
	sprintf(command, "soffice --headless --invisible --convert-to pdf %s --outdir /tmp/%s ", argu->path, argu->user);

	ret = system(command);	
	
	//send convert OK message
	PreviewStatus s;
	memset(&s, 0, sizeof(PreviewStatus));
	
	if (ret == -1 || ret == 127)
	{
		s.status = Failed;
	}	
	else
	{
		s.status = Success;
	}
	s.filetype = Office;
	sendPreviewMsg(argu->sockfd, s);		
	printf("convert success\n");	
	pthread_exit((void *)0);
}
int convertOfficeToPdf(int sockfd, const char *filename)
{
	pthread_t t;
	argument  arg;
	int err, i;
	char path[1024] = "/var/penguin/";
	char user[512];
    	strcat(path, filename);

	for(i = 0; i < strlen(filename); ++i)
	{
		if(filename[i] == '/')
			break;
		user[i] = filename[i];
	}
	user[i] = '\0';
	arg.sockfd = sockfd;
	strcpy(arg.path, path);
	strcpy(arg.user, user);
	printf("start convert thread\n");
	err = pthread_create(&t, NULL, thread_convert, (void *)&arg);	
	if(err != 0) {
#ifdef Debug
	fprintf(stdout, "create thread error\n");
#endif	
	return -1;	
	}
	
	return 0;
}

/*
int previewPdf(int sockfd, const char *filename)
{
	int ret, i;
        char path[1024] = "/var/penguin/";
        char user[512] = "/XQEY/";
        char command[512] = "";
	strcat(path, filename);

        for(i = 0; i < strlen(filename); ++i)
        {
                if(filename[i] == '/')
                        break;
                user[i+6] = filename[i];
        }
        user[i+6] = '\0';
	
	sprintf(command, "cp %s %s", path, user);
	printf("%s\n", command);	
	ret = system(command);

	PreviewStatus s;
        memset(&s, 0, sizeof(PreviewStatus));

        if (ret == -1 || ret == 127)
        {
                s.status = Failed;
        }
        else
        {
                s.status = Success;
        }
        s.filetype = Pdf;
        sendPreviewMsg(sockfd, s);
        printf("copy success\n");
	return 0;
}

int previewMusic(int sockfd, const char*filename)
{
	int ret, i;
        char path[1024] = "/var/penguin/";
        char user[512] = "/XQEY/";
        char command[512] = "";
        strcat(path, filename);

        for(i = 0; i < strlen(filename); ++i)
        {
                if(filename[i] == '/')
                        break;
                user[i+6] = filename[i];
        }
        user[i+6] = '\0';

        sprintf(command, "cp %s %s", path, user);
        printf("%s\n", command);
        ret = system(command);

        PreviewStatus s;
        memset(&s, 0, sizeof(PreviewStatus));

        if (ret == -1 || ret == 127)
        {
                s.status = Failed;
        }
        else
        {
                s.status = Success;
        }
        s.filetype = Music;
        sendPreviewMsg(sockfd, s);
        printf("copy success\n");	
	return 0;
}

int previewVideo(int sockfd, const char *filename)
{
	int ret, i;
        char path[1024] = "/var/penguin/";
        char user[512] = "/XQEY/";
        char command[512] = "";
        strcat(path, filename);

        for(i = 0; i < strlen(filename); ++i)
        {
                if(filename[i] == '/')
                        break;
                user[i+6] = filename[i];
        }
        user[i+6] = '\0';

        sprintf(command, "cp %s %s", path, user);
        printf("%s\n", command);
        ret = system(command);

        PreviewStatus s;
        memset(&s, 0, sizeof(PreviewStatus));

        if (ret == -1 || ret == 127)
        {
                s.status = Failed;
        }
        else
        {
                s.status = Success;
        }
        s.filetype = Video;
        sendPreviewMsg(sockfd, s);
        printf("copy success\n");
        return 0;
}*/

int preview(int sockfd, const char *filename, FileType type)
{
	int ret, i;
        char path[1024] = "/var/penguin/";
        char user[512] = "/XQEY/";
        char command[512] = "";
        strcat(path, filename);

        for(i = 0; i < strlen(filename); ++i)
        {
                if(filename[i] == '/')
                        break;
                user[i+6] = filename[i];
        }
        user[i+6] = '\0';

        sprintf(command, "cp %s %s", path, user);
        printf("%s\n", command);
        ret = system(command);

        PreviewStatus s;
        memset(&s, 0, sizeof(PreviewStatus));

        if (ret == -1 || ret == 127)
        {
                s.status = Failed;
        }
        else
        {
                s.status = Success;
        }
        s.filetype = type;
        sendPreviewMsg(sockfd, s);
        printf("copy success\n");
        return 0;

}
