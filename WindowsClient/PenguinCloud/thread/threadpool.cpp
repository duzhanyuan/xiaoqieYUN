#include "ThreadPool.h"
#include "ThreadObject.h"

#include <iostream>

using namespace std;

ThreadPool::ThreadPool(int workerNumber) :
m_iBossInspectionCycle(1000),
m_bRun(true)
{
	//���������
	m_iMaxWorker = 0;
	//�����еĹ�������
	m_iWorking = 0;
	//�������й���������
	m_iTaskNumber = 0;

	setScale();

	createBoss();

	for (int i = 0; i < 3; ++i)
	{
		workers.emplace(&ThreadPool::workerJob, this);
	}
}


ThreadPool::~ThreadPool()
{
	m_bRun = false;
	boss.join();

	//�������й��˰����еĹ���������°���
	workerCondition.notify_all();
	while (workers.size())
	{
		workers.front().join();
		workers.pop();
	}
}

//��ӹ���
void ThreadPool::addJob(ThreadObject *job)
{
	cout << "addJob" << endl;

	taskQueueMutex.lock();

	tasks.push(job);

	++m_iTaskNumber;

	taskQueueMutex.unlock();

	//����һ��������Ϣ�Ĺ���
	workerCondition.notify_one();
}

//��Ϊ�ϰ�
void ThreadPool::createBoss()
{
	boss = thread(&ThreadPool::bossJob, this);
}

//��Ӷ����
void ThreadPool::hireWorker(const int &workerNumber)
{

}

//��Ƹ����
void ThreadPool::fireWorker(const int &workerNumber)
{

}

//�������ȴ���
void ThreadPool::setScale(const double &scale)
{
	m_dWWScale = scale;
}

//�ϰ�Ĺ���
void ThreadPool::bossJob()
{
	while (m_bRun)
	{
		//cout << "�ϰ�����" << "id = " << this_thread::get_id() << endl;

		this_thread::sleep_for(chrono::milliseconds(m_iBossInspectionCycle));
	}
}

//���˵Ĺ���
void ThreadPool::workerJob()
{
	ThreadObject *object;
	while (m_bRun)
	{
		//unique_lock<mutex> lock(workerMutex);

		workerMutex.lock();
		cout << "����ס��" << endl;
		//����������Ϊ�վ���ס
		while (this->m_bRun && this->tasks.empty())
			workerCondition.wait(workerMutex);

		workerMutex.unlock();

		cout << "������" << endl;

		taskQueueMutex.lock();

		object = tasks.front();
		tasks.pop();
		--m_iTaskNumber;
		++m_iWorking;

		taskQueueMutex.unlock();

		object->run();

		taskQueueMutex.lock();

		--m_iWorking;

		taskQueueMutex.unlock();

		cout << "m_iWorking = " << m_iWorking << endl;
		cout << "m_iTaskNumber = " << m_iTaskNumber << endl;
		cout << "id = " << this_thread::get_id() << endl;
	}
}
