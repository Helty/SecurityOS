#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <Windows.h>

namespace
{
	static const int SIZE = 4096;

	static const std::vector<std::string> serviceStatusInfos =
	{
		"",
		"The service is not running.",
		"The service is starting.",
		"The service is stopping.",
		"The service is running.",
		"The service continue is pending.",
		"The service pause is pending.",
		"The service is paused."
	};

	enum class SERVICE
	{
		WIN_32 = 0,
		DRIVER
	};

	static const std::unordered_map<SERVICE, std::string> SERVICE_TABLE = {
		{SERVICE::WIN_32, "������� ������ �����: "},
		{SERVICE::DRIVER, "������� ������ ���������: "}
	};

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);

	ENUM_SERVICE_STATUS serviceStatus[SIZE];
	ENUM_SERVICE_STATUS_PROCESS serviceStatusProcess[SIZE];

	DWORD sizeStatus = sizeof(serviceStatus), needed = 0, returned = 0, handle = 0;
}

void PrintArguments(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
	}
	std::cout << std::endl;
}

void Info()
{
	std::cout << "������������ ������ #4." << std::endl
			    << "������ �������� ������� ������ ��-31 �������� ������." << std::endl
				<< "��������� ���������:" << std::endl 
				<< "/? - � ���������" << std::endl 
			    << "/d - ����� ������ ���������" << std::endl 
			    << "/i \"���_������\"- ����� ������ � ������" << std::endl;
	
}

void PrintServicesOrDrivers(SERVICE service)
{
	std::cout << SERVICE_TABLE.find(service)->second << std::endl;

	for (size_t i = 0; i < returned; i++)
	{
		std::wcout << std::setw(40) << std::left << serviceStatus[i].lpServiceName;
		std::cout << ' ' << std::setw(40) << std::left << serviceStatusInfos[serviceStatus[i].ServiceStatus.dwCurrentState];
		std::wcout << ' ' << serviceStatus[i].lpDisplayName << std::endl;
	}
}

void StartServicesStatus(SERVICE service)
{
	EnumServicesStatus(
		schSCManager,
		(service == SERVICE::DRIVER) ? SERVICE_DRIVER : SERVICE_WIN32,
		SERVICE_STATE_ALL,
		(LPENUM_SERVICE_STATUSW)&serviceStatus,
		sizeStatus,
		&needed,
		&returned,
		&handle
	);
}

int IsServiceNameExistens(std::string serviceName, bool& isServiceNameExist)
{
	std::wstring serviceNameW = std::wstring(serviceName.begin(), serviceName.end());

	EnumServicesStatusEx(
		schSCManager,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32 | SERVICE_DRIVER,
		SERVICE_STATE_ALL,
		(LPBYTE)serviceStatusProcess,
		sizeStatus,
		&needed,
		&returned,
		&handle,
		NULL
	);

	for (int index = 0; index < static_cast<int>(returned); index++)
	{
		if (serviceStatusProcess[index].lpServiceName == serviceNameW)
		{
			isServiceNameExist = true;
			return index;
		}
	}

	return -1;
}

bool PrintInfoAboutService(int indexService)
{
	if (indexService == -1) return false;

	std::cout << std::setw(25) << std::left << "������� ��������:";
	std::wcout << serviceStatusProcess[indexService].lpServiceName << std::endl;
	std::cout << std::setw(25) << std::left << "������ ��������:";
	std::wcout << serviceStatusProcess[indexService].lpDisplayName << std::endl;
	std::cout << std::setw(25) << std::left << "��� ������:";

	if (serviceStatusProcess[indexService].ServiceStatusProcess.dwServiceType == SERVICE_KERNEL_DRIVER)
		std::cout << "The service is a device driver." << std::endl;
	else if (serviceStatusProcess[indexService].ServiceStatusProcess.dwServiceType == SERVICE_FILE_SYSTEM_DRIVER)
		std::cout << "The service is a file system driver." << std::endl;
	else if (serviceStatusProcess[indexService].ServiceStatusProcess.dwServiceType == SERVICE_WIN32_OWN_PROCESS)
		std::cout << "The service runs in its own process." << std::endl;
	else
		std::cout << "The service shares a process with other services." << std::endl;

	std::cout << std::setw(25) << std::left << "������ ������:" << serviceStatusInfos[serviceStatusProcess[indexService].ServiceStatusProcess.dwCurrentState] << std::endl;
	std::cout << std::setw(25) << std::left << "������������� ��������:" << serviceStatusProcess[indexService].ServiceStatusProcess.dwProcessId << std::endl;

	return true;
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	{
		PrintArguments(argc, argv);
	}

	{
		SERVICE serviceParam = (argv[1] == "/d") ? SERVICE::DRIVER : SERVICE::WIN_32;
		StartServicesStatus(serviceParam);
		PrintServicesOrDrivers(serviceParam);
		return 0;
	}

	{
		if (argv[1] == "/?") Info();
	}

	{
		if (argv[1] == "/i")
		{
			bool isServiceNameExist;
			int indexService = IsServiceNameExistens(argv[2], isServiceNameExist);

			if (!PrintInfoAboutService(indexService))
			{
				std::cout << "������ " << argv[2] << " �� �������." << std::endl;
			}
		}
	}
	return 0;
}