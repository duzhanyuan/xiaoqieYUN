/*


�ú�����ҪΪ�������ĺ��Ŀ��
Ϊ�ͻ����ṩ�������Ӧ


*/


void *serverForClient(Socket client)
{
	//����ö��
	Order order;
	char[5][20] orderPara;

	//�ȴ��û��ĵ�½
	//���������Ӧ�û��ĵ�½����
	//�յ㿪�ͻ���ֻ���������
	//����ֵΪ����ֵ������������ɲ���������û��ļ��ṹ�������ļ��ṹ��дһ�������ļ�
	if (waitForLogin(client) == false)
	{
		//��ش���
	}

	while (serverFlag)
	{
		//��ȡ�û�������ṩ�Ĳ���
		readOrder(client, orderPara);
		if (order.Load == toInt(orderPara[0]))
		{
			//��ز���
		}


		/*
		
		������ټӹ��ܣ����Ƽ����
		��Ҫ��֤һ������һ������
		��������ļ��ġ�����ԡ�
		��ֻ�Ǹ���ܣ����﷨�����
		
		
		*/
	}
}