// MOI_Loader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

typedef int(__stdcall* f_MoiMainWin)();

int main()
{
	HINSTANCE DLLHandle = LoadLibraryW(L"moi_lib.dll");
	if (DLLHandle)
	{
		f_MoiMainWin MoiMainWin = (f_MoiMainWin)GetProcAddress(DLLHandle, "MoiMainWin");
		if (MoiMainWin) {
			MoiMainWin();
		}
	}
}

