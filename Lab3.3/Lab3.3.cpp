#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>

void ArgumentsCountCheck(int argc)
{
	if (argc < 2)
	{
		std::cout << "Usage: Lab3.3.exe <file to execute> <?> <params>" << std::endl
			<< "\t<file to execute> - path to file." << std::endl
			<< "\t<?> - optional parameter to display information." << std::endl
			<< "\t<params> - optional params to file execute." << std::endl;
		throw std::invalid_argument("invalid count arguments.");
	}
}

void Info()
{
	std::cout << "Лабораторная работа 3." <<std::endl;
	std::cout << "Вариант - а." <<std::endl;
	std::cout << "Автор: Шестаков Андрей БИ-31." <<std::endl;
	std::cout << "Usage: Lab2.exe <file to execute> <?> <params>" << std::endl
		<< "\t<file to execute> - path to file." << std::endl
		<< "\t<?> - optional parameter to display information." << std::endl
		<< "\t<params> - params to file execute." << std::endl;
	std::cout << "Описание: данная программа запускает другую программу через CreateProcessA" <<std::endl <<std::endl;
	system("PAUSE");
}

int main(int argc, const char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << "Привет, я 3.3" << std::endl;

	try
	{
		ArgumentsCountCheck(argc);

		std::string argumentsS;
		STARTUPINFOA cif;
		ZeroMemory(&cif, sizeof(STARTUPINFOA));
		PROCESS_INFORMATION pi;

		for (size_t i = 1; i < argc; i++) argumentsS.append(argv[i]).append(" ");

		LPSTR arguments = const_cast<char*>(argumentsS.c_str());

		CreateProcessA(argv[1], arguments, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi);
		system("pause");
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
		return 1;
	}
}