#ifndef LINUXSERVER_OPERATING_SERVEROPERATING_H
#define LINUXSERVER_OPERATING_SERVEROPERATING_H

#include "../message/nstdmsgio.h"

//获取目录下的文件
char *getDirFileLists(char *userDir);
//文件上传的线程
void *uploadFileThread(int sockfd, void *arg);
//新建文件夹
int createNewFolder(char *folderPath);
#endif //LINUXSERVER_OPERATING_SERVEROPERATING_H
