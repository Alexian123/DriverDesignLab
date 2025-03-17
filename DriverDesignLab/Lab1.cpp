#include "Lab1.h"

#include <Windows.h>

Lab1::Lab1()
{
}

std::vector<std::pair<std::wstring, std::wstring>> Lab1::getImagePaths() const
{
	std::vector<std::pair<std::wstring, std::wstring>> imagePaths;
	HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD subKeyIndex = 0;
        WCHAR subKeyName[256];
        DWORD subKeyNameSize = sizeof(subKeyName) / sizeof(subKeyName[0]);

        while (RegEnumKeyExW(hKey, subKeyIndex, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            HKEY hSubKey;
            if (RegOpenKeyExW(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                WCHAR imagePath[1024];
                DWORD imagePathSize = sizeof(imagePath) / sizeof(imagePath[0]);
                DWORD type;

                if (RegQueryValueExW(hSubKey, L"ImagePath", NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS) {
                    imagePaths.emplace_back(subKeyName, imagePath);
                }
                else {
                    imagePaths.emplace_back(subKeyName, L"N/A");
                }
                RegCloseKey(hSubKey);
            }
            subKeyNameSize = sizeof(subKeyName) / sizeof(subKeyName[0]);
            subKeyIndex++;
        }
        RegCloseKey(hKey);
    }
    return imagePaths;
}