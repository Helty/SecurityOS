#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <Windows.h>
#include <Shellapi.h>

void Info()
{
	std::cout << "Лабораторная работа 3." << std::endl;
	std::cout << "Автор: Шестаков Андрей БИ-31." << std::endl;
	std::cout << "Usage: Lab2.exe <file to execute> <?> <ShowCmd int> <params>" << std::endl
		<< "\t<file to execute> - path to file." << std::endl
		<< "\t<ShowCmd int> - optional parameter" << std::endl
		<< "\t<?> - optional parameter to display information." << std::endl
		<< "\t<params> - params to file execute." << std::endl;
	std::cout << "Описание: данная программа запускает другую программу через ShellExecute" << std::endl << std::endl;
	system("PAUSE");
}

void ArgumentsCountCheck(int argc)
{
	if (argc < 3)
	{
		std::cout << "Usage: Lab3.2.exe <file to execute> <?> <ShowCmd int> <params>" << std::endl
			<< "\t<file to execute> - path to file." << std::endl
			<< "\t<ShowCmd int> - optional parameter" << std::endl
			<< "\t<?> - optional parameter to display information." << std::endl
			<< "\t<params> - params to file execute." << std::endl;
		throw std::invalid_argument("invalid count arguments.");
	}
}

int main(int argc, const char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		ArgumentsCountCheck(argc);

		int ShowCmd = 9;

		if (argv[2] == std::string("?")) Info();

		std::string params;
		for (size_t i = 0; i < argc - 1; i++)
		{
			if (argv[i + 1] == std::string("?")) continue;
			if (std::isdigit(argv[i + 1][0]))
			{
				ShowCmd = atoi(argv[i + 1]);
				continue;
			}
			params.append(argv[i + 1]).append(" ");
		}

		ShellExecute(NULL, "open", argv[1], ((params.size() == 1) ? NULL : params.c_str()), NULL, ShowCmd);
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
		return 1;
	}
}