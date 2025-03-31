#pragma once

#include <windows.h>
#include <setupapi.h>
#include <vector>
#include <string>

class Lab3
{
private:
	static std::wstring GetDeviceProperty(HDEVINFO hDevInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty);
public:
	Lab3();
	void listAllUSBDevices();
};

