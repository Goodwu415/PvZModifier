#pragma once
#include <Windows.h>
#include <list>
#include <algorithm>

class CMemFinder
{
public:
	CMemFinder()
	{
		SYSTEM_INFO info; //获得一页内存的大小
		GetSystemInfo(&info);
		m_dwPageSize = info.dwPageSize;
	}
	~CMemFinder()
	{
		SafeCloseHandle();
	}
	//打开进程
	bool OpenProcess(DWORD dwProcessId)
	{//如果进程是打开的，则关闭进程
		if(IsValidHandle())
			SafeCloseHandle();
		m_hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE |
			PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION,
			FALSE, dwProcessId);
		if(IsValidHandle())//进程打开成功
			return true;
		else
		{
			SafeCloseHandle();
			return false;
		}
	}
	/* 初次扫描
	   1. 打开进程
	   2. 扫描范围内的内存,寻找可读的,使用中的内存
	   3. 与目标值进行比较
   */
	template<typename T>
	bool FindFirst(DWORD dwProcessId, DWORD dwBegin, DWORD dwEnd, T value)
	{
		m_arList.clear();
		return _FindFirst(dwProcessId, dwBegin, dwEnd, value);
	}
	/*
		在 FindFirst结果的基础上, 继续进行比较
	*/
	template<typename T>
	bool FindNext(T value)
	{
		return _FindNext(value);
	}
	//读内存
	template<typename T>
	bool Read(DWORD dwAddr, T& val)
	{
		if(::ReadProcessMemory(m_hProcess, (LPCVOID)dwAddr, &val, sizeof(val), NULL))
			return true;
		return false;
	}
private:
	//查找函数
	template<typename T>
	bool _FindFirst(DWORD dwProcessId, DWORD dwBegin, DWORD dwEnd, T value)
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, dwProcessId);
		m_hProcess = hProcess;


	}
	//看看有没有消息，有就取出来，并分发
	void WaitForldle()
	{
		MSG msg;
		while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//PM_REMOVE:消息处理后，从消息队列中移除
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	//是否是合法的进程句柄
	bool IsValidHandle()
	{
		return (m_hProcess && INVALID_HANDLE_VALUE != m_hProcess);
	}
	//关闭进程句柄
	void SafeCloseHandle()
	{
		CloseHandle(m_hProcess);
		m_hProcess = INVALID_HANDLE_VALUE;
	}
	//目标进程句柄
	HANDLE m_hProcess{ INVALID_HANDLE_VALUE };
	//搜索结果
	std::list<DWORD> m_arList;
	//回调函数
	typedef bool(__stdcall *PFUN_CALLBACK)(void* pArgs, size_t nPageCount, size_t index);
	//首次扫描 回调函数
	PFUN_CALLBACK m_pGoonFirst{ nullptr };
	//下次扫描回调函数
	PFUN_CALLBACK m_pGoonNext{ nullptr };
	//回调函数的参数：为用户设置回调函数时,传过来的指针,当本类内调用回调函数时,原封不动传回去
	void* m_pArgsFirst{ nullptr };
	void* m_pArgsNext{ nullptr };

	DWORD m_dwPageSize;
};
