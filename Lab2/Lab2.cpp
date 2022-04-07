#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <regex>

namespace
{
	static const std::regex WORD_SEARCH("[a-zA-Z�-��-�]+");
	static const std::regex NEWLINE_SEARCH("(?=\n|$)");
}

void Info() 
{
	std::cout << "������������ ������ 2." << std::endl;
	std::cout << "������� - �." << std::endl;
	std::cout << "�����: �������� ������ ��-31." << std::endl;
	std::cout << "��������: ������ ��������� ������������� ������ �� ������������ �������� ����� ������." << std::endl << std::endl;
	system("PAUSE");
}

std::string ReadFromFile(std::string const& fileName)
{
	std::ifstream fileIn(fileName);
	std::string str((std::istreambuf_iterator<char>(fileIn)),
		std::istreambuf_iterator<char>());
	fileIn.close();
	return str;
}

std::string EnterText()
{
	std::string result;
	std::cout << "������� �����: " << std::endl;
	std::getline(std::cin, result, '.');
	return result;
}

int CountInStrByRegExp(std::string const& str, std::regex rx)
{
	auto words_begin = std::sregex_iterator(str.begin(), str.end(), rx);
	auto words_end = std::sregex_iterator();
	auto count = std::distance(words_begin, words_end);
	return static_cast<int>(count);
}

void ResultingCount(std::string str)
{
	std::cout << "����� �������� � ������: " << str.size() << std::endl;
	std::cout << "����� ���� � ������: " << CountInStrByRegExp(str, WORD_SEARCH) << std::endl;
	std::cout << "����� �����: " << CountInStrByRegExp(str, NEWLINE_SEARCH) << std::endl << std::endl;
}

void PrintMenu()
{
	system("cls");
	std::cout << "�������� ��������: " << std::endl;
	std::cout << "1 - ��������� �� �����." << std::endl;
	std::cout << "2 - ������ � ����������." << std::endl;
	std::cout << "3 - �������� �����." << std::endl;
	std::cout << "4 - ������� ������� ��������, ���� � �����." << std::endl;
	std::cout << "0 - ����� �� ���������." << std::endl;
}

void Start(std::string const& filePath)
{
	std::string txtFromFile;
	std::string usersTxt;
	bool exitFromProgram = false;
	bool strWasReadFromFile = false;
	bool strWasEnteredByUser = false;

	do
	{
		PrintMenu();

		int userChoice;
		std::cout << "��� ����� ��������: ";
		std::cin >> userChoice;

		switch (userChoice) 
		{
		case 1:
		{
			system("cls");
			std::cout << "������ �� �����";
			txtFromFile = ReadFromFile(filePath);
			if (!txtFromFile.empty())
			{
				std::cout << " ������ �������." << std::endl;
				strWasReadFromFile = true;
			}
			else std::cout << " ��������� ���������. ���� �� ������." << std::endl;
			break;
		}
		case 2:
		{
			system("cls");
			std::cout << "���� ������ ������������:" <<std::endl;
			usersTxt = EnterText();
			if (!usersTxt.empty()) strWasEnteredByUser = true;
			else std::cout << "����� ������������ �� ��� ������." <<std::endl;
			break;
		}
		case 3:
		{
			system("cls");
			if (strWasReadFromFile)
			{
				std::cout << "�����, ��������� �� �����:" <<std::endl << std::endl;
				std::cout << txtFromFile <<std::endl;
			}
			if (strWasEnteredByUser)
			{
				std::cout << "�����, ��������� �������������:" <<std::endl << std::endl;
				std::cout << usersTxt <<std::endl;
			}
			if (!strWasEnteredByUser and !strWasReadFromFile)
			{
				std::cout << "����� �� ���������. ������� ��� �������� �� �����." <<std::endl;
			}
			break;
		}
		case 4:
		{
			system("cls");
			if (strWasReadFromFile)
			{
				std::cout << "�����, ��������� �� �����: " << std::endl;
				ResultingCount(txtFromFile);
			}
			if (strWasEnteredByUser)
			{
				std::cout << "�����, ��������� �������������: " <<std::endl;
				ResultingCount(usersTxt);
			}
			if (!strWasEnteredByUser and !strWasReadFromFile)
			{
				std::cout << "����� �� ���������. ������� ��� �������� �� �����." <<std::endl;
			}
			break;
		}
		case 0:
		{
			system("cls");
			exit(0);
		}
		};
		system("PAUSE");

	} while (!exitFromProgram);
}

void ArgumentsCountCheck(int argc)
{
	if (argc < 2 or argc > 3)
	{
		std::cout << "Usage: Lab2.exe <input filePath> <?>" << std::endl
			<< "\t<input filePath> - path to file with text." << std::endl
			<< "\t<?> - optional parameter to display information" << std::endl;
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
		if ((argc == 3) and (argv[2] == std::string("?"))) Info();
		Start(argv[1]);
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
		return 1;
	}
}

