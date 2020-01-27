#pragma once
#include <Windows.h>
#include <list>
#include <algorithm>

class CMemFinder
{
public:
	CMemFinder()
	{
		SYSTEM_INFO info; //���һҳ�ڴ�Ĵ�С
		GetSystemInfo(&info);
		m_dwPageSize = info.dwPageSize;
	}
	~CMemFinder()
	{
		SafeCloseHandle();
	}
	//�򿪽���
	bool OpenProcess(DWORD dwProcessId)
	{//��������Ǵ򿪵ģ���رս���
		if(IsValidHandle())
			SafeCloseHandle();
		m_hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE |
			PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION,
			FALSE, dwProcessId);
		if(IsValidHandle())//���̴򿪳ɹ�
			return true;
		else
		{
			SafeCloseHandle();
			return false;
		}
	}
	/* ����ɨ��
	   1. �򿪽���
	   2. ɨ�跶Χ�ڵ��ڴ�,Ѱ�ҿɶ���,ʹ���е��ڴ�
	   3. ��Ŀ��ֵ���бȽ�
   */
	template<typename T>
	bool FindFirst(DWORD dwProcessId, DWORD dwBegin, DWORD dwEnd, T value)
	{
		m_arList.clear();
		return _FindFirst(dwProcessId, dwBegin, dwEnd, value);
	}
	/*
		�� FindFirst����Ļ�����, �������бȽ�
	*/
	template<typename T>
	bool FindNext(T value)
	{
		return _FindNext(value);
	}
	//���ڴ�
	template<typename T>
	bool Read(DWORD dwAddr, T& val)
	{
		if(::ReadProcessMemory(m_hProcess, (LPCVOID)dwAddr, &val, sizeof(val), NULL))
			return true;
		return false;
	}
private:
	//���Һ���
	template<typename T>
	bool _FindFirst(DWORD dwProcessId, DWORD dwBegin, DWORD dwEnd, T value)
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, dwProcessId);
		m_hProcess = hProcess;


	}
	//������û����Ϣ���о�ȡ���������ַ�
	void WaitForldle()
	{
		MSG msg;
		while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//PM_REMOVE:��Ϣ����󣬴���Ϣ�������Ƴ�
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	//�Ƿ��ǺϷ��Ľ��̾��
	bool IsValidHandle()
	{
		return (m_hProcess && INVALID_HANDLE_VALUE != m_hProcess);
	}
	//�رս��̾��
	void SafeCloseHandle()
	{
		CloseHandle(m_hProcess);
		m_hProcess = INVALID_HANDLE_VALUE;
	}
	//Ŀ����̾��
	HANDLE m_hProcess{ INVALID_HANDLE_VALUE };
	//�������
	std::list<DWORD> m_arList;
	//�ص�����
	typedef bool(__stdcall *PFUN_CALLBACK)(void* pArgs, size_t nPageCount, size_t index);
	//�״�ɨ�� �ص�����
	PFUN_CALLBACK m_pGoonFirst{ nullptr };
	//�´�ɨ��ص�����
	PFUN_CALLBACK m_pGoonNext{ nullptr };
	//�ص������Ĳ�����Ϊ�û����ûص�����ʱ,��������ָ��,�������ڵ��ûص�����ʱ,ԭ�ⲻ������ȥ
	void* m_pArgsFirst{ nullptr };
	void* m_pArgsNext{ nullptr };

	DWORD m_dwPageSize;
};
