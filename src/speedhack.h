#pragma once

#include <Windows.h>
#include <detours.h>

namespace Speedhack
{
	extern double speed;
	extern bool attatched;

	typedef DWORD(WINAPI *_tGetTickCount)(void);
	extern _tGetTickCount _GTC;
	extern DWORD _GTC_BaseTime;

	typedef ULONGLONG(WINAPI *_tGetTickCount64)(void);
	extern _tGetTickCount64 _GTC64;
	extern DWORD _GTC64_BaseTime;

	typedef BOOL(WINAPI *_tQueryPerformanceCounter)(LARGE_INTEGER *);
	extern _tQueryPerformanceCounter _QPC;
	extern LARGE_INTEGER _QPC_BaseTime;


	DWORD WINAPI _hGetTickCount();

	DWORD WINAPI _hGetTickCount64();

	DWORD WINAPI _hQueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);


	void Setup();

	void Detach();

	void SetSpeed(float relSpeed);
}
