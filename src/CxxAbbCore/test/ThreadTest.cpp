/** 
 *                                                             _|        _|        
 *                       _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|    
 *                     _|          _|_|      _|_|    _|    _|  _|    _|  _|    _|  
 *                     _|        _|    _|  _|    _|  _|    _|  _|    _|  _|    _|  
 *                       _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|
 * 
 *                              CxxABB - C++ Application Building Blocks
 *
 *                     Copyright (C) 2017 Prabodha Srimal <prabodha007@gmail.com>
 *  
 *
 * ThreadTest.cpp
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Dec 20, 2012
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : <Library Name>
 * Module      : <Module Name>
 * Comment     : <General Comment>
 *
 */


#include <CxxAbb/Sys/Thread.h>
#include <CxxAbb/Sys/SignalToException.h>
#include <CxxAbb/Timespan.h>

#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <gtest/gtest.h>


namespace
{

extern "C" void MyHandler(int _iSigNum)
{
	std::cout << "Test " << _iSigNum << std::endl;
	raise(_iSigNum);
}

class TestRunnable: public CxxAbb::Runnable
{
public:
	void Run()
	{
		CxxAbb::Sys::SignalToException::SetupHandler(SIGFPE, MyHandler);
		ThrowOnSignal();

		std::cout << "Im running ..." << std::endl;
		sleep(10);
		//throw CxxAbb::SystemException("Test exception", 5);
		int i=0;
		float f = 3/i;
		std::cout << "Im done" << std::endl;
	}
private:
};

class MyRunnable: public CxxAbb::Runnable
{
public:
	MyRunnable(): _ran(false)
	{
	}

	void Run()
	{
		CxxAbb::Sys::Thread* pThread = CxxAbb::Sys::Thread::Current();
		if (pThread)
			_threadName = pThread->Name();
		_ran = true;
		_event.Wait();
	}

	bool Ran() const
	{
		return _ran;
	}

	const std::string& ThreadName() const
	{
		return _threadName;
	}

	void Notify()
	{
		_event.Set();
	}

	static void StaticFunc()
	{
		++_staticVar;
	}

	static int _staticVar;

private:
	bool _ran;
	std::string _threadName;
	CxxAbb::Sys::SigEvent _event;
};


int MyRunnable::_staticVar = 0;

void freeFunc()
{
	++MyRunnable::_staticVar;
}


void freeFunc(void* pData)
{
	MyRunnable::_staticVar += *reinterpret_cast<int*>(pData);
}

}
 
TEST(ThreadTest, SingleRun)
{
	CxxAbb::Sys::Thread thread;
	MyRunnable r;
	ASSERT_TRUE(!thread.IsRunning());
	thread.Start(r);
	CxxAbb::Sys::Thread::Sleep(200);
	ASSERT_TRUE (thread.IsRunning());
	r.Notify();
	thread.Join();
	ASSERT_TRUE (!thread.IsRunning());
	ASSERT_TRUE (r.Ran());
	ASSERT_TRUE (!r.ThreadName().empty());
	COUT_LOG() << r.ThreadName();
}

TEST(ThreadTest, Name)
{
	CxxAbb::Sys::Thread thread("MyThread");
	MyRunnable r;
	thread.Start(r);
	r.Notify();
	thread.Join();
	ASSERT_TRUE (r.Ran());
	ASSERT_TRUE (r.ThreadName() == "MyThread");
}

TEST(ThreadTest, Current)
{
	ASSERT_TRUE (CxxAbb::Sys::Thread::Current() == CxxAbb::NullPtr);
}

TEST(ThreadTest, Multiple)
{
	CxxAbb::Sys::Thread thread1("Thread1");
	CxxAbb::Sys::Thread thread2("Thread2");
	CxxAbb::Sys::Thread thread3("Thread3");
	CxxAbb::Sys::Thread thread4("Thread4");

	MyRunnable r1;
	MyRunnable r2;
	MyRunnable r3;
	MyRunnable r4;
	ASSERT_TRUE (!thread1.IsRunning());
	ASSERT_TRUE (!thread2.IsRunning());
	ASSERT_TRUE (!thread3.IsRunning());
	ASSERT_TRUE (!thread4.IsRunning());
	thread1.Start(r1);
	CxxAbb::Sys::Thread::Sleep(200);
	ASSERT_TRUE (thread1.IsRunning());
	ASSERT_TRUE (!thread2.IsRunning());
	ASSERT_TRUE (!thread3.IsRunning());
	ASSERT_TRUE (!thread4.IsRunning());
	thread2.Start(r2);
	thread3.Start(r3);
	thread4.Start(r4);
	CxxAbb::Sys::Thread::Sleep(200);
	ASSERT_TRUE (thread1.IsRunning());
	ASSERT_TRUE (thread2.IsRunning());
	ASSERT_TRUE (thread3.IsRunning());
	ASSERT_TRUE (thread4.IsRunning());
	r4.Notify();
	thread4.Join();
	ASSERT_TRUE (!thread4.IsRunning());
	ASSERT_TRUE (thread1.IsRunning());
	ASSERT_TRUE (thread2.IsRunning());
	ASSERT_TRUE (thread3.IsRunning());
	r3.Notify();
	thread3.Join();
	ASSERT_TRUE (!thread3.IsRunning());
	r2.Notify();
	thread2.Join();
	ASSERT_TRUE (!thread2.IsRunning());
	r1.Notify();
	thread1.Join();
	ASSERT_TRUE (!thread1.IsRunning());
	ASSERT_TRUE (r1.Ran());
	ASSERT_TRUE (r1.ThreadName() == "Thread1");
	ASSERT_TRUE (r2.Ran());
	ASSERT_TRUE (r2.ThreadName() == "Thread2");
	ASSERT_TRUE (r3.Ran());
	ASSERT_TRUE (r3.ThreadName() == "Thread3");
	ASSERT_TRUE (r4.Ran());
	ASSERT_TRUE (r4.ThreadName() == "Thread4");
}

TEST(ThreadTest, Join)
{
	CxxAbb::Sys::Thread thread;
	MyRunnable r;
	ASSERT_TRUE (!thread.IsRunning());
	thread.Start(r);
	CxxAbb::Sys::Thread::Sleep(200);
	ASSERT_TRUE (thread.IsRunning());
	ASSERT_TRUE (!thread.TryJoin(100));
	r.Notify();
	ASSERT_TRUE (thread.TryJoin(500));
	ASSERT_TRUE (!thread.IsRunning());
}

TEST(ThreadTest, ThreadFunc)
{

	CxxAbb::Sys::Thread thread;

	ASSERT_TRUE (!thread.IsRunning());
	int tmp = MyRunnable::_staticVar;

	thread.Start(freeFunc, &tmp);
	thread.Join();
	ASSERT_TRUE (tmp * 2 == MyRunnable::_staticVar);
	ASSERT_TRUE (!thread.IsRunning());

	tmp = MyRunnable::_staticVar = 0;
	thread.Start(freeFunc, &tmp);
	thread.Join();
	ASSERT_TRUE (0 == MyRunnable::_staticVar);

}

TEST(ThreadTest, Sleep)
{
	CxxAbb::Timestamp start;
	CxxAbb::Sys::Thread::Sleep(200);
	CxxAbb::Timespan elapsed = start.Elapsed();
	ASSERT_TRUE (elapsed.TotalMilliseconds() >= 190 && elapsed.TotalMilliseconds() < 250);
}

//int main()
//{
//	CxxAbb::Sys::Thread thread;
//	TestRunnable runnale;
//	thread.Start(runnale);
//	//thread.Join();
//	sleep(100);
//	return 0;
//}
