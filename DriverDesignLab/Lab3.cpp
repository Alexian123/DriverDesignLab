#include "Lab3.h"

#pragma comment(lib, "Setupapi.lib")

#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <tchar.h>

std::wstring Lab3::GetDeviceProperty(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty)
{
    DWORD dwType = 0;
    DWORD dwSize = 0;
    SetupDiGetDeviceRegistryProperty(hDevInfo, pDevInfoData, dwProperty, &dwType, NULL, 0, &dwSize);

    if (dwSize == 0) {
        return L""; // Empty/Non-existent property
    }
    std::vector<BYTE> buffer(dwSize);
    if (SetupDiGetDeviceRegistryProperty(hDevInfo, pDevInfoData, dwProperty, &dwType, buffer.data(), dwSize, &dwSize)) {
        if (dwType == REG_SZ || dwType == REG_MULTI_SZ) {
            return std::wstring(reinterpret_cast<wchar_t*>(buffer.data()));
        }
    }
    return L"";
}

Lab3::Lab3()
{
}

void Lab3::listAllUSBDevices()
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD i;

    hDevInfo = SetupDiGetClassDevs(NULL, // All clasele
        NULL, // Enumerator (ex: "USB", "PCI") - NULL
        NULL, // HWND parent
        DIGCF_PRESENT | DIGCF_ALLCLASSES); // Only present devices

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error at SetupDiGetClassDevs: %lu\n", GetLastError());
        return;
    }

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++) {
        std::wstring hardwareId = GetDeviceProperty(hDevInfo, &deviceInfoData, SPDRP_HARDWAREID);
        std::wstring compatibleId = GetDeviceProperty(hDevInfo, &deviceInfoData, SPDRP_COMPATIBLEIDS);
        std::wstring enumeratorName = GetDeviceProperty(hDevInfo, &deviceInfoData, SPDRP_ENUMERATOR_NAME);

        // Check if device is USB
        // Search for "USB\" în HardwareID, CompatibleID or if Enumerator is "USB"
        bool isUsbDevice = false;
        if (hardwareId.find(L"USB\\") != std::wstring::npos ||
            compatibleId.find(L"USB\\") != std::wstring::npos ||
            enumeratorName == L"USB")
        {
            isUsbDevice = true;
        }

        if (isUsbDevice) {
            std::wstring devDesc = GetDeviceProperty(hDevInfo, &deviceInfoData, SPDRP_DEVICEDESC);
            std::wstring manufacturer = GetDeviceProperty(hDevInfo, &deviceInfoData, SPDRP_MFG);


            _tprintf(_T("Description: %s\n"), devDesc.empty() ? L"(Unknown)" : devDesc.c_str());
            _tprintf(_T("  Producer: %s\n"), manufacturer.empty() ? L"(Unknown)" : manufacturer.c_str());
            _tprintf(_T("  Hardware ID(s): %s\n"), hardwareId.empty() ? L"(Unknown)" : hardwareId.c_str());
            _tprintf(_T("---\n"));
        }
    }

    if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS) {
        fprintf(stderr, "Device query error: %lu\n", GetLastError());
    }

    // Free resources
    SetupDiDestroyDeviceInfoList(hDevInfo);
}
