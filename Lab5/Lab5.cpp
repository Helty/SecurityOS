#include <iostream>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include <string>

namespace
{
	HANDLE hEvent1;
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

DWORD WINAPI fun_thread(LPVOID n) 
{
	EnterCriticalSection(&cs);
	srand(static_cast<unsigned>(time(NULL)));

	while (true)
	{
		if (WaitForSingleObject(hEvent1, 0) == WAIT_OBJECT_0) 
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

	HANDLE const hMutex = CreateMutexW(NULL, FALSE, L"lab5");

	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		std::cout << "��������� ��� ��������." << std::endl;
		system("pause");
		return -1;
	}

	HANDLE hEvent1 = CreateEventA(NULL, TRUE, FALSE, NULL);

	int countThread = 2;

	if (argc > 1 and argv[1] != "/?") countThread = atoi(argv[1]);

	for (size_t i = 1; i <= countThread; i++)
	{
		HANDLE thread1 = CreateThread(NULL, 0, fun_thread, LPVOID(i), 0, NULL);
	}

	while (true) 
	{
		if (_getch() == 27) 
		{
			SetEvent(hEvent1);
			Sleep(2000);
			break;
		}
	}

	return 0;
}
