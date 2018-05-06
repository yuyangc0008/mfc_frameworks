#ifndef _WORK_THREAD_H_V100_
#define _WORK_THREAD_H_V100_


// 工作线程
class CWorkThread
{
public:
	CWorkThread();
	virtual ~CWorkThread();

	typedef void (__stdcall *WorkFunc)(void *);
	BOOL Start(WorkFunc, void *);

private:
	inline void lock() { ::EnterCriticalSection(&m_cLocker); }
	inline void unlock() { ::LeaveCriticalSection(&m_cLocker); }
	static unsigned __stdcall Worker(void *);

private:
	HANDLE             m_hThreadId;  // 线程id
	HANDLE             m_hStartEvent;// 启动事件
	HANDLE             m_hQuitEvent; // 通知线程退出事件
	CRITICAL_SECTION   m_cLocker;    // 临界区，用作互斥锁
	BOOL               m_bThreadRun; // 线程是否已运行
	BOOL               m_bThreadQuit;// 线程是否已退出

	WorkFunc           m_pWorkFunc;  // 实际运行函数
	void*              m_pWorkParams;// 参数
};


#endif /* _WORK_THREAD_H_V100_ */