#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <iostream>
#include <string> 
#include <vector>
#include <Windows.h>
#include <process.h>

void Info()
{
	std::cout << "Лабораторная работа 3." << std::endl;
	std::cout << "Автор: Шестаков Андрей БИ-31." << std::endl;
	std::cout << "Описание: данная программа запускает другую программу через execve" << std::endl << std::endl;
	system("PAUSE");
}

void ArgumentsCountCheck(int argc)
{
	if (argc < 3)
	{
		std::cout << "Usage: Lab3.1.exe <file to execute> <?> <params>" << std::endl
			<< "\t<file to execute> - path to file." << std::endl
			<< "\t<?> - optional parameter to display information." << std::endl
			<< "\t<params> - params to file execute." << std::endl;
		throw std::invalid_argument("invalid count arguments.");
	}
}

int main(int argc, const char* argv[], char** envp)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << "Привет, я 3.1" << std::endl;

	ArgumentsCountCheck(argc);

	if ((argc == 3) and (argv[2] == std::string("?"))) Info();

	std::vector<std::string> strings;

	for (size_t i = 0; i < argc - 1; i++) strings.push_back(argv[i + 1]);

	std::vector<char*> cstrings;
	cstrings.reserve(strings.size());

	for (auto& s : strings) cstrings.push_back(&s[0]);
	cstrings.push_back(NULL);

	execve(argv[1], cstrings.data(), envp);

	system("pause");
}