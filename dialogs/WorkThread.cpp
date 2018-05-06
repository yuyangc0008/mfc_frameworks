#include "stdafx.h"
#include "WorkThread.h"


CWorkThread::CWorkThread()
	: m_hThreadId(NULL)
	, m_hStartEvent(NULL)
	, m_hQuitEvent(NULL)
	, m_bThreadRun(FALSE)
	, m_bThreadQuit(TRUE)
	, m_pWorkFunc(NULL)
	, m_pWorkParams(NULL)
{
	// 初始化
	m_hStartEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hQuitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::InitializeCriticalSection(&m_cLocker);
	// 启动线程
	m_hThreadId = (HANDLE)::_beginthreadex(NULL, 0, Worker, (void *)this, 0, NULL);
	do {
		::Sleep(100);
	} while (m_bThreadQuit);
}

CWorkThread::~CWorkThread()
{
	::SetEvent(m_hQuitEvent);
	::SetEvent(m_hStartEvent);
	while (!m_bThreadQuit)  // 等待线程退出
		::Sleep(100);

	::CloseHandle(m_hThreadId);
	m_hThreadId = NULL;
	::CloseHandle(m_hStartEvent);
	m_hStartEvent = NULL;
	::CloseHandle(m_hQuitEvent);
	m_hQuitEvent = NULL;
	::DeleteCriticalSection(&m_cLocker);
}

BOOL CWorkThread::Start(WorkFunc workFunc, void *params)
{
	if (!m_bThreadQuit && !m_bThreadRun)
	{
		// 设置参数
		lock();
		m_pWorkFunc = workFunc;
		m_pWorkParams = params;
		unlock();
		SetEvent(m_hStartEvent);  // 启动
		do {
			::Sleep(1);
		} while (/*!m_bThreadRun*/0);
		return TRUE;
	}
	return FALSE;
}

unsigned __stdcall CWorkThread::Worker(void *params)
{
	CWorkThread *pThis = (CWorkThread *)params;
	if (!pThis)
		return 1;

	pThis->lock();
	pThis->m_bThreadQuit = FALSE;
	pThis->m_bThreadRun = FALSE;
	pThis->unlock();

	WorkFunc workFunc = NULL;
	void *workParams = NULL;
	while (WAIT_OBJECT_0 == ::WaitForSingleObject(pThis->m_hStartEvent, INFINITE))  // 等待启动
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(pThis->m_hQuitEvent, 1))  // 线程退出
			break;
		// 设置参数
		pThis->lock();
		workFunc = pThis->m_pWorkFunc;
		workParams = pThis->m_pWorkParams;
		pThis->m_bThreadRun = TRUE;
		pThis->unlock();

		// 运行
		if (workFunc)
			(workFunc)(workParams);

		pThis->lock();
		pThis->m_bThreadRun = FALSE;
		pThis->unlock();
	}

	pThis->lock();
	pThis->m_bThreadQuit = TRUE;
	pThis->unlock();
	return 0;
}



