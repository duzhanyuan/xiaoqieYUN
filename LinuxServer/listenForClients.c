/*

���������Ҫ���ڼ����û�������
ʵ������NIO�����ڷ���˵Ĺ���


*/



void *listenForClients()
{
	//��������˵��׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		//�������ͱ���
	}

	//��IP�Ͷ˿�  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		//�������ͱ���
	}

	//��ʼ����  
	if (listen(slisten) == SOCKET_ERROR)
	{
		//�������ͱ���
	}


	//ѭ����������  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	while (serverFlag)
	{

		//����û��׽���
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);

		if (sClient == INVALID_SOCKET)
		{
			//�������ͱ���
			continue;
		}

		//���û��׽��ּ�������������
		addSocket(clientOnline, sClient);

		//����serverForClient����ʼΪ�û����񣬵���дһ���ļ�
		//�����Ŀ���Ĳ���
		if (pthread_create(null, null, serverForClient, sClient) == 0)
		{
			//�Դ���Ĵ���ͱ���
		}
	}

	//�������ɾ��
	freeSockets(clientOnline);
}