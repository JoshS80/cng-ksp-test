// KSPEnum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <ncrypt.h>
#include <string>

void EnumProviderKeys(NCRYPT_KEY_HANDLE provHandle, DWORD flags)
{
    NCryptKeyName* pKey = nullptr;
    PVOID pEnumState = nullptr;
    SECURITY_STATUS status = ERROR_SUCCESS;

    while ((status = NCryptEnumKeys(provHandle, nullptr, &pKey, &pEnumState, NCRYPT_SILENT_FLAG | flags) == ERROR_SUCCESS))
    {
        printf("Key: %S AlgID: %S KeySpec %d Flags %d\n", pKey->pszName, pKey->pszAlgid, pKey->dwLegacyKeySpec, pKey->dwFlags);
        NCryptFreeBuffer(pKey);
        pKey = nullptr;
    }

    NCryptFreeBuffer(pEnumState);
    pEnumState = nullptr;
}

void EnumProviderKeys(const std::wstring& p)
{
    NCRYPT_KEY_HANDLE provKey = 0;
    auto status = NCryptOpenStorageProvider(&provKey, p.c_str(), 0);

    if (status != ERROR_SUCCESS)
    {
        printf("Failed to open provider '%S', error 0x%08x\n", p.c_str(), status);
        return;
    }

    printf("\nProvider enum: %S\n", p.c_str());
    EnumProviderKeys(provKey, NCRYPT_MACHINE_KEY_FLAG);
    EnumProviderKeys(provKey, 0);

    NCryptFreeObject(provKey);
    provKey = 0;
}

int main()
{
    DWORD provCount = 0;
    NCryptProviderName* provList = nullptr;
    auto status = NCryptEnumStorageProviders(&provCount, &provList, 0);

    printf("Status: 0x%08x\n", status);
    printf("Provider count: %d\n", provCount);

    for (DWORD i = 0; i < provCount; ++i)
    {
        printf("Name: %S, comment: %S\n", provList[i].pszName, provList[i].pszComment);
        EnumProviderKeys(provList[i].pszName);
    }

    NCryptFreeBuffer(provList);
    provList = nullptr;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
