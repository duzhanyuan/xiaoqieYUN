#pragma once
class ThreadObject
{
public:
	ThreadObject();
	~ThreadObject();

	//������Ҫ��д�������
	virtual void run();
	virtual void start();
	virtual void pause();
	virtual void stop();
};

