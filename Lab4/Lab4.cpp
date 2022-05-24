#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <locale>

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
		{SERVICE::WIN_32, "Получен список служб: "},
		{SERVICE::DRIVER, "Получен список драйверов: "}
	};

	SC_HANDLE schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);

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
	std::cout << "Лабораторная работа #4." << std::endl
		<< "Работу выполнил студент группы БИ-31 Шестаков Андрей." << std::endl
		<< "Аргументы программы:" << std::endl
		<< "/? - о программе" << std::endl
		<< "/d - вывод списка драйверов" << std::endl
		<< "/i \"имя_службы\"- вывод данных о службе" << std::endl;

}

void PrintServicesOrDrivers(SERVICE service)
{
	std::cout << SERVICE_TABLE.find(service)->second << std::endl;

	for (size_t i = 0; i < returned; i++)
	{
		std::cout << serviceStatus[i].lpServiceName << " - ";
		std::cout << serviceStatus[i].lpDisplayName << " - ";
		std::cout << serviceStatusInfos[serviceStatus[i].ServiceStatus.dwCurrentState] << std::endl;
	}
}

bool PrintInfoAboutServiceByIndex(int indexService)
{
	if (indexService == -1) return false;

	std::cout << std::setw(25) << std::left << "Краткое название:";
	std::wcout << serviceStatusProcess[indexService].lpServiceName << std::endl;
	std::cout << std::setw(25) << std::left << "Полное название:";
	std::wcout << serviceStatusProcess[indexService].lpDisplayName << std::endl;
	std::cout << std::setw(25) << std::left << "Тип службы:";

	if (serviceStatusProcess[indexService].ServiceStatusProcess.dwServiceType == SERVICE_KERNEL_DRIVER)
		std::cout << "The service is a device driver." << std::endl;
	else if (serviceStatusProcess[indexService].ServiceStatusProcess.dwServiceType == SERVICE_FILE_SYSTEM_DRIVER)
		std::cout << "The service is a file system driver." << std::endl;
	else if (serviceStatusProcess[indexService].ServiceStatusProcess.dwServiceType == SERVICE_WIN32_OWN_PROCESS)
		std::cout << "The service runs in its own process." << std::endl;
	else
		std::cout << "The service shares a process with other services." << std::endl;

	std::cout << std::setw(25) << std::left << "Статус службы:" << serviceStatusInfos[serviceStatusProcess[indexService].ServiceStatusProcess.dwCurrentState] << std::endl;
	std::wcout << std::setw(25) << std::left << "Идентификатор процесса:" << serviceStatusProcess[indexService].ServiceStatusProcess.dwProcessId << std::endl;

	return true;
}

void StartServicesStatus(SERVICE service)
{
	if (!EnumServicesStatusA(
		schSCManager,
		(service == SERVICE::DRIVER) ? SERVICE_DRIVER : SERVICE_WIN32,
		SERVICE_STATE_ALL,
		serviceStatus,
		sizeStatus,
		&needed,
		&returned,
		&handle
	))
	{
		throw std::exception("Ошибка получения списка сервисов.");
	}
}

int GetIndexServiceNameIfExistens(std::string serviceName)
{
	std::wstring serviceNameW = std::wstring(serviceName.begin(), serviceName.end());

	if (!EnumServicesStatusExA(
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
	))
	{
		throw std::exception("Ошибка получения списка сервисов.");
	}

	for (int index = 0; index < static_cast<int>(returned); index++)
	{
		if (serviceStatusProcess[index].lpServiceName == serviceName) return index;
	}

	return -1;
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	PrintArguments(argc, argv);
	
	SERVICE serviceParam = (argv[1] == "/d") ? SERVICE::DRIVER : SERVICE::WIN_32;
	StartServicesStatus(serviceParam);
	PrintServicesOrDrivers(serviceParam);

	if (argv[1] == "/?") Info();
	
	if (argv[1] == "/i")
	{
		if (!PrintInfoAboutServiceByIndex(GetIndexServiceNameIfExistens(argv[2])))
		{
			std::cout << "Служба " << argv[2] << " не найдена." << std::endl;
		}
	}
	
	return 0;
}