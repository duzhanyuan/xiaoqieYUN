#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

class ThreadObject;

class ThreadPool
{
public:
	//Ĭ��Ϊû��Ա��
	ThreadPool(int workerNumber = 0);
	~ThreadPool();

	//��ӹ���
	void addJob(ThreadObject *job);
	//�������ȴ���
	void setScale(const double &scale = 0.618);

private:
	//��Ϊ�ϰ�
	void createBoss();
	//��Ӷ����
	void hireWorker(const int &workerNumber = 1);
	//��Ƹ����
	void fireWorker(const int &workerNumber = 1);
	//�ϰ�Ĺ���
	void bossJob();
	//���˵Ĺ���
	void workerJob();

	//����
	std::queue<std::thread> workers;
	//������Ϣ����
	std::mutex workerMutex;
	//���˽�����Ϣ�������ź���
	std::condition_variable_any workerCondition;
	//��������
	std::queue<ThreadObject *> tasks;
	//����������
	std::mutex taskQueueMutex;
	//�ϰ�
	std::thread boss;
	//�ϰ������Ϣ�������ź���
	std::condition_variable bossCondition;

	//���������
	int m_iMaxWorker;
	//�����еĹ�������
	int m_iWorking;
	//�������й���������
	int m_iTaskNumber;
	//�ϰ嶨ʱ�Ӳ칤����ʱ��(ms)
	int m_iBossInspectionCycle;
	//�����еĹ��˺��ܶ��еı�ֵ
	//�����ֵ������Ա������
	//��Ա������
	//working : wait
	double m_dWWScale;
	//�̳߳ؽ�����־
	bool m_bRun;
};

