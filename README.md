# version 4.0 downloaded 06.02.2023

Main goal of this material is to see mistake in using Themida WinLicense.

MoI.exe is just loader of moi_lib.dll which is packed by WinLicence packer.

After little time of bianry investigation its clear that it snot necessary to fully unpack the dll the final solution is to rewrite MoI.exe and add in memory patching procedures such es:

```
typedef int(__stdcall* f_MoiMainWin)(HINSTANCE, LPWSTR, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DWORD oldprotect;

    HINSTANCE DLLHandle = LoadLibraryW(L"moi_lib.dll");
    if (DLLHandle)
    {
        f_MoiMainWin MoiMainWin = (f_MoiMainWin)GetProcAddress(DLLHandle, "MoiMainWin");
        if (MoiMainWin) {

            __int64 dllBase = (__int64)GetModuleHandleW(L"moi_lib.dll");

            VirtualProtect((LPVOID)(dllBase + 0x1000), 0x200000, PAGE_EXECUTE_READWRITE, &oldprotect);

            *(__int8*)(dllBase + 0x1000 + 0x17EA56) = 0xB8;
            *(__int8*)(dllBase + 0x1000 + 0x17EA57) = 0x02;
            *(__int8*)(dllBase + 0x1000 + 0x17EA58) = 0x00;
            *(__int8*)(dllBase + 0x1000 + 0x17EA59) = 0x00;
            *(__int8*)(dllBase + 0x1000 + 0x17EA5a) = 0x00;
            
                VirtualProtect((LPVOID)(dllBase + 0x1000), 0x200000, oldprotect, &oldprotect);

            return MoiMainWin(hInstance, lpCmdLine, nCmdShow);


        }
    }
    return 0;
}
```

moi_lib.dll unpcaked
```
________:00007FFA4FF8FA30 48 8B C4                                            mov     rax, rsp
________:00007FFA4FF8FA33 55                                                  push    rbp
________:00007FFA4FF8FA34 57                                                  push    rdi
________:00007FFA4FF8FA35 41 54                                               push    r12
________:00007FFA4FF8FA37 41 56                                               push    r14
________:00007FFA4FF8FA39 41 57                                               push    r15
________:00007FFA4FF8FA3B 48 8B EC                                            mov     rbp, rsp
________:00007FFA4FF8FA3E 48 83 EC 70                                         sub     rsp, 70h
________:00007FFA4FF8FA42 48 C7 45 B0 FE FF FF FF                             mov     [rbp+var_50], 0FFFFFFFFFFFFFFFEh
________:00007FFA4FF8FA4A 48 89 58 18                                         mov     [rax+18h], rbx
________:00007FFA4FF8FA4E 48 89 70 20                                         mov     [rax+20h], rsi
________:00007FFA4FF8FA52 B3 01                                               mov     bl, 1
________:00007FFA4FF8FA54 33 C9                                               xor     ecx, ecx
here is exatc patch from above c code,
instead of call i am puting mov eax, 0x02
________:00007FFA4FF8FA56 E8 62 74 46 01                                      call    sub_7FFA513F6EBD <-- is expired
________:00007FFA4FF8FA5B 90                                                  nop
________:00007FFA4FF8FA5C 0F B6 F3                                            movzx   esi, bl
________:00007FFA4FF8FA5F 45 33 E4                                            xor     r12d, r12d
________:00007FFA4FF8FA62 83 F8 01                                            cmp     eax, 1
________:00007FFA4FF8FA65 41 0F 44 F4                                         cmovz   esi, r12d
________:00007FFA4FF8FA69 E8 C6 97 46 01                                      call    sub_7FFA513F9234 <-- get numers of days left
________:00007FFA4FF8FA6E 90                                                  nop
________:00007FFA4FF8FA6F 89 05 87 F2 1C 01                                   mov     cs:dword_7FFA5115ECFC, eax
________:00007FFA4FF8FA75 40 84 F6                                            test    sil, sil
________:00007FFA4FF8FA78 0F 85 F4 01 00 00                                   jnz     loc_7FFA4FF8FC72
________:00007FFA4FF8FA7E 41 8B DC                                            mov     ebx, r12d
________:00007FFA4FF8FA81 48 89 5D 38                                         mov     [rbp+arg_8], rbx

```