/*

����ļ�Ϊ�������Ļ�����ܣ���Ҫ�ָ����ͬģ�飬��һЩ�������б�Ҫ��˵����


*/
#include<...>

//���ڼ�¼�Ѿ����ӵĵ��û���������������������棬�����������ʱ�����ȽϺ�ʱ����������ݽṹΪmap����c��û���������ݽṹ��ʱ�������cʵ������java��map
//����дһ��sockets��c�ļ��������ݽṹ�Ͳ���
Sockets clientOnline = null;

//���ڼ�¼�������Ƿ�ʼ������ͬʱ���Ʒ������Ŀ���
bool serverFlag = false;

int main()
{


	//������һ���߳��������û�������
	//listenForClients��������дһ���ļ�,�ú�����
	if (pthread_create(null, null, listenForClients, null) == 0)
	{
		//�Դ���Ĵ���ͱ���
	}


	//����������д���棬�����û��Ĺ���

	//�ص���listenForCLients�к��������õ�serverForClientForClients
}