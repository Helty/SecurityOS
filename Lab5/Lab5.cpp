#include <iostream>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include <string>

namespace
{
	HANDLE hEvent;
	CRITICAL_SECTION cs;

	void Info()
	{
		std::cout << "������������ ������ #5." << std::endl
			<< "������ �������� ������� ������ ��-31 �������� ������." << std::endl
			<< "��������� ���������:" << std::endl
			<< "/? - � ���������" << std::endl
			<< "<count thread> - ����������� �������" << std::endl;
	}
}

DWORD WINAPI threadFun(LPVOID n) 
{
	EnterCriticalSection(&cs);

	srand(static_cast<unsigned>(time(NULL)));

	while (true)
	{
		if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0) 
		{
			std::cout << std::endl << "����� " << n << " �������� ������!";
			ExitThread(0);
		}
		std::cout << "����� " << n << std::endl;

		Sleep((rand() % 10) * 150);
		LeaveCriticalSection(&cs);
	}

	return 0;
}

int main(int argc, char* argv[]) 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	InitializeCriticalSection(&cs);

	if (argc > 1 and argv[1] == "/?") Info();

	CreateMutexW(NULL, FALSE, L"lab5");
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		std::cout << "��������� ��� ��������." << std::endl;
		system("pause");
		return -1;
	}

	hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);

	size_t countThread = (argc > 1 and argv[1] != "/?") ? atoi(argv[1]) : 2;

	for (size_t threadIndex = 1; threadIndex <= countThread; threadIndex++)
	{
		CreateThread(NULL, 0, threadFun, LPVOID(threadIndex), 0, NULL);
	}

	while (_getch() != 27) continue;
	
	SetEvent(hEvent);
	Sleep(2000);

	return 0;
}
