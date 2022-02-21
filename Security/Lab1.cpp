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
	cout << "Available devices in the system: " << endl << endl;
	for (int i = 0; i < 16; i++) cout << (char)lpBuffer[i];
	cout << endl;
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
		cout << "Serial number: " << volumeSerialNumber << endl;
		cout << "File system: " << fileSystemNameBuffer << endl;
	}
	else cout << "Device not found." << endl;
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
		cout << "Total storage capacity = " << GB(totalNumberOfBytes) << "  GB" << endl;
		cout << "Free space = " << GB(totalNumberOfFreeBytes) << "  GB" << endl;
	}
	else 
	{
		cout << "No data." << endl;
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
		cout << "Device successfully removed." << endl;
	}
	else 
	{
		cout << "The device has not been ejected." << endl;
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
	cout << ToInt(MenuItem::DEVICE_LIST) << ". List all devices." << endl;
	cout << ToInt(MenuItem::DEVICE_TYPES) << ". Find out device type." << endl;
	cout << ToInt(MenuItem::DEVICE_INFO) << ". View device information." << endl;
	cout << ToInt(MenuItem::DEVICE_REMOVE) << ". Remove device." << endl;
	cout << ToInt(MenuItem::EXIT) << ". Exit." << endl;
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
	cout << "Select device name to view device type: " << endl;
	wcin >> deviceName;
	cout << getDeviceType(deviceName.c_str()) << endl;
}
void PrintDeviceInfo()
{
	string deviceName;
	cout << "Enter device name to view information: ";
	cin >> deviceName;
	getInfoAboutDevice(deviceName.c_str());
	getInforAboutDevicesSpace(deviceName.c_str());
}
void TryEnjectDevice()
{
	string deviceName;
	cout << "Enter device name to retrieve: ";
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