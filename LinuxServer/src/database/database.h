#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <winsock2.h> 
#include "mysql.h"/*ע��Ҫ�������ͷ�ļ�*/ 
#pragma comment(lib,"libmysql") 
/*������һЩ���ݿ�������Ҫ�ĺ�*/

#ifndef MY_DATABASE	

#define HOST "localhost"
#define USERNAME "sa"
#define PASSWORD "123456"
#define DATABASE "server"

typedef int DATABASE_INFO;

#define SERVER_ERROR 0
#define NAME_ERROR 1
#define PASSWORD_ERROR 2
#define USER_LOGIN_SUCCESS 3

typedef int bool;
#define false 0
#define true 1

#define MY_DATABASE

#endif
