#include <windows.h>
#include <iostream>
#include <map>
#include <vector>
#include <cwchar>
#include <istream>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <atlstr.h>

#define GB(x) (x.HighPart << 2) + (x.LowPart >> 20) / 1024

namespace 
{
	enum class MenuItem : int
	{
		DEVICE_LIST = 1,
		DEVICE_TYPES,
		DEVICE_INFO,
		DEVICE_REMOVE,
		EXIT
	};

	using namespace std;
}

void printOutLogicalDrives(LPWSTR lpBuffer)
{
	std::cout << "Available devices in the system: " << std::endl << std::endl;
	for (int i = 0; i < 16; i++) std::cout << (char)lpBuffer[i];
	std::cout <<std::endl;
}
void getInfoAboutDevice(LPCSTR deviceName) 
{
	CHAR volumeNameBuffer[100];
	CHAR fileSystemNameBuffer[100];
	DWORD volumeSerialNumber;

	BOOL informationExist = GetVolumeInformationA(
		deviceName /*LPCTSTR*/,
		volumeNameBuffer,
		100,
		&volumeSerialNumber,
		NULL /*MaximumComponentLength*/,
		NULL /*FileSystemFlags*/,
		fileSystemNameBuffer,
		100
	);

	if (informationExist)
	{
		std::cout << "Serial number: " << volumeSerialNumber <<std::endl;
		std::cout << "File system: " << fileSystemNameBuffer <<std::endl;
	}
	else std::cout << "Device not found." <<std::endl;
}
void getInforAboutDevicesSpace(LPCSTR directoryName)
{
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;

	BOOL haveSizeInformation = GetDiskFreeSpaceExA(
		directoryName,
		NULL /*lpFreeBytesAvailableToCaller*/,
		&totalNumberOfBytes,
		&totalNumberOfFreeBytes
	);

	if (haveSizeInformation)
	{
		std::cout << "Total storage capacity = " << GB(totalNumberOfBytes) << "  GB" <<std::endl;
		std::cout << "Free space = " << GB(totalNumberOfFreeBytes) << "  GB" <<std::endl;
	}
	else 
	{
		std::cout << "No data." <<std::endl;
	}
}
string getDeviceType(LPCTSTR driveName) 
{
	if (GetDriveType(driveName) == DRIVE_UNKNOWN ||
		GetDriveType(driveName) == DRIVE_NO_ROOT_DIR) { return "Unknown disk type."; }
	if (GetDriveType(driveName) == DRIVE_REMOVABLE) { return "Removable disk type."; }
	if (GetDriveType(driveName) == DRIVE_FIXED) { return "HDD."; }
	if (GetDriveType(driveName) == DRIVE_REMOTE) { return "Remote or network drive."; }
	if (GetDriveType(driveName) == DRIVE_CDROM) { return "CD ROM."; }
	if (GetDriveType(driveName) == DRIVE_RAMDISK) { return "RAM."; }
	else { return "Disk not found."; }
}
void ejectDrive(string name_s)
{
	string temp = "\\\\.\\";
	temp.push_back(name_s[0]);
	temp.push_back(name_s[1]);
	DWORD dwRet = 0;
	HANDLE hVol = CreateFileA(temp.c_str(), GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hVol == INVALID_HANDLE_VALUE)
		return;

	if (!DeviceIoControl(hVol, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &dwRet, 0))
		return;

	if (!DeviceIoControl(hVol, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &dwRet, 0))
		return;

	bool ejactSucceeded = DeviceIoControl(hVol, IOCTL_STORAGE_EJECT_MEDIA, 0, 0, 0, 0, &dwRet, 0);
	if (ejactSucceeded) 
	{
		std::cout << "Device successfully removed." <<std::endl;
	}
	else 
	{
		std::cout << "The device has not been ejected." <<std::endl;
	}

	CloseHandle(hVol);
}
int ToInt(MenuItem item)
{
	return static_cast<int>(item);
}

void PrintMenu()
{
	system("cls");
	std::cout << ToInt(MenuItem::DEVICE_LIST) << ". List all devices." <<std::endl;
	std::cout << ToInt(MenuItem::DEVICE_TYPES) << ". Find out device type." <<std::endl;
	std::cout << ToInt(MenuItem::DEVICE_INFO) << ". View device information." <<std::endl;
	std::cout << ToInt(MenuItem::DEVICE_REMOVE) << ". Remove device." <<std::endl;
	std::cout << ToInt(MenuItem::EXIT) << ". Exit." <<std::endl;
}
void PrintDeviceList()
{
	wchar_t disks[256];
	wchar_t* disk;
	DWORD sizebuf = 256;

	if (GetLogicalDriveStringsW(sizebuf, disks) != 0)
	{
		disk = disks;
		while (*disk)
		{
			wprintf(L"%s\n", disk);
			disk = disk + wcslen(disk) + 1;
		}
	}
}
void PrintDeviceType()
{
	wstring deviceName;
	std::cout << "Select device name to view device type: " <<std::endl;
	wcin >> deviceName;
	std::cout << getDeviceType(deviceName.c_str()) <<std::endl;
}
void PrintDeviceInfo()
{
	string deviceName;
	std::cout << "Enter device name to view information: ";
	cin >> deviceName;
	getInfoAboutDevice(deviceName.c_str());
	getInforAboutDevicesSpace(deviceName.c_str());
}
void TryEnjectDevice()
{
	string deviceName;
	std::cout << "Enter device name to retrieve: ";
	cin >> deviceName;
	ejectDrive(deviceName);
}
void StartMenu()
{
	bool nextCycle = true;
	int userChoice;

	while (nextCycle) 
	{
		PrintMenu();
		cin >> userChoice;
		system("cls");

		switch (static_cast<MenuItem>(userChoice))
		{
		case MenuItem::DEVICE_LIST:
			PrintDeviceList();
			break;
		case MenuItem::DEVICE_TYPES:
			PrintDeviceType();
			break;
		case MenuItem::DEVICE_INFO:
			PrintDeviceInfo();
			break;
		case MenuItem::DEVICE_REMOVE:
			TryEnjectDevice();
			break;
		case MenuItem::EXIT:
			nextCycle = false;
			break;
		default:
			break;
		}
		system("PAUSE");
	}
}

int main()
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		StartMenu();
	}
	catch (const exception exp)
	{
	 	cerr << exp.what();
	 	return 1;
	}
	return 0;
}