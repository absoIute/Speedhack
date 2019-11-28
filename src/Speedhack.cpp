#include "Speedhack.h"

namespace Speedhack
{
	double speed = 1.0;
	bool attatched = false;

	extern _tGetTickCount _GTC = nullptr;
	extern DWORD _GTC_BaseTime = 0;

	extern _tGetTickCount64 _GTC64 = nullptr;
	extern DWORD _GTC64_BaseTime = 0;

	extern _tQueryPerformanceCounter _QPC = nullptr;
	extern LARGE_INTEGER _QPC_BaseTime = LARGE_INTEGER();

	DWORD WINAPI _hGetTickCount()
	{
		return _GTC_BaseTime + ((_GTC() - _GTC_BaseTime) * speed);
	}

	DWORD WINAPI _hGetTickCount64()
	{
		return _GTC64_BaseTime + ((_GTC64() - _GTC64_BaseTime) * speed);
	}

	DWORD WINAPI _hQueryPerformanceCounter(LARGE_INTEGER * lpPerformanceCount)
	{
		LARGE_INTEGER x;
		_QPC(&x);
		lpPerformanceCount->QuadPart = _QPC_BaseTime.QuadPart + ((x.QuadPart - _QPC_BaseTime.QuadPart) * speed);
		return TRUE;
	}

	void Setup()
	{
		if (attatched)
			return;

		HMODULE hMod = GetModuleHandle(L"Kernel32.dll");

		if (!hMod)
			return;

		_GTC = (_tGetTickCount)GetProcAddress(hMod, "GetTickCount");
		_GTC_BaseTime = _GTC();

		_GTC64 = (_tGetTickCount64)GetProcAddress(hMod, "GetTickCount64");
		_GTC64_BaseTime = _GTC64();

		_QPC = (_tQueryPerformanceCounter)GetProcAddress(hMod, "QueryPerformanceCounter");
		_QPC(&_QPC_BaseTime);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(PVOID &)_GTC, _hGetTickCount);
		DetourAttach(&(PVOID &)_GTC64, _hGetTickCount64);
		DetourAttach(&(PVOID &)_QPC, _hQueryPerformanceCounter);
		
		DetourTransactionCommit();

		attatched = true;
	}

	void Detach()
	{
		if (!attatched)
			return;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourDetach(&(PVOID &)_GTC, _hGetTickCount);
		DetourDetach(&(PVOID &)_GTC64, _hGetTickCount64);
		DetourDetach(&(PVOID &)_QPC, _hQueryPerformanceCounter);

		DetourTransactionCommit();

		attatched = false;
	}

	void SetSpeed(float relSpeed)
	{
		if (attatched)
		{
			_GTC_BaseTime = _hGetTickCount();
			_GTC64_BaseTime = _hGetTickCount64();
			_hQueryPerformanceCounter(&_QPC_BaseTime);
		}

		speed = relSpeed;
	}
}
