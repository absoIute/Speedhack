//example of usage

#include "speedhack.hpp"

DWORD WINAPI MainThread(LPVOID lpParam)
{
	Speedhack::Setup();
	Speedhack::SetSpeed(0.5); //slows down to half speed

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0x1000, &MainThread, NULL, 0, NULL);
	}

	return TRUE;
}