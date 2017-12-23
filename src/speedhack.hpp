#pragma once

#include <Windows.h>
#include <detours.h>

namespace Speedhack
{
	float speed = 1;

	typedef DWORD(WINAPI *_tGetTickCount)(void);
	_tGetTickCount _GTC;
	DWORD _GTC_BaseTime = 0;

	typedef ULONGLONG(WINAPI *_tGetTickCount64)(void);
	_tGetTickCount64 _GTC64;
	DWORD _GTC64_BaseTime = 0;

	typedef BOOL(WINAPI *_tQueryPerformanceCounter)(LARGE_INTEGER*);
	_tQueryPerformanceCounter _QPC;
	LARGE_INTEGER _QPC_BaseTime;

	typedef DWORD(WINAPI *_tSleepEx)(DWORD dwMilliseconds, BOOL bAlertable);
	_tSleepEx _SE;



	DWORD WINAPI _hGetTickCount()
	{
		return _GTC_BaseTime + ((_GTC() - _GTC_BaseTime) * speed);
	}

	DWORD WINAPI _hGetTickCount64()
	{
		return _GTC64_BaseTime + ((_GTC64() - _GTC64_BaseTime) * speed);
	}

	DWORD WINAPI _hQueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount)
	{
		LARGE_INTEGER li;
		_QPC(&li);
		lpPerformanceCount->QuadPart = _QPC_BaseTime.QuadPart + ((li.QuadPart - _QPC_BaseTime.QuadPart) * speed);
		return TRUE;
	}

	DWORD WINAPI _hSleepEx(DWORD dwMilliseconds, BOOL bAlertable)
	{
		return _SE(dwMilliseconds / speed, bAlertable);
	}



	void Setup()
	{
		_GTC = (_tGetTickCount)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "GetTickCount");
		_GTC_BaseTime = _GTC();

		_GTC64 = (_tGetTickCount64)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "GetTickCount64");
		_GTC64_BaseTime = _GTC64();

		_QPC = (_tQueryPerformanceCounter)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "QueryPerformanceCounter");
		_QPC(&_QPC_BaseTime);

		_SE = (_tSleepEx)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "SleepEx");
	}

	void Attach()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(PVOID&)_GTC, _hGetTickCount);
		DetourAttach(&(PVOID&)_GTC64, _hGetTickCount64);
		DetourAttach(&(PVOID&)_QPC, _hQueryPerformanceCounter);
		DetourAttach(&(PVOID&)_SE, _hSleepEx);

		DetourTransactionCommit();
	}
}