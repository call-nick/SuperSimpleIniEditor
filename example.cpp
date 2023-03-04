#include "iniEditor.h"

int main()
{
	system("chcp 1251");

	IniEdit ini;
	try {
		std::string section = "Nick5";
		std::string filePath = "File.ini";
		std::string btn = "btn1";



		ini.addSection(filePath, "Кинотеатр");
		ini.addSection(filePath, "Театр");
		ini.addSection(filePath, "Цирк");


		ini.addVarInSection(filePath, "Кинотеатр", "ключ1к", "значение1к");


		//ini.addVarInSection(filePath, "Заповедник", "ключ1", "значение1"); //Не добавиться, т.к. у нас нет поля заповедник

		ini.addVarInSection(filePath, "Цирк", "ключ1ц", "значение1ц");
		//ini.addVarInSection(filePath, "Цирк", "ключ1ц", "значение1ц");

		ini.addVarInSection(filePath, "Кинотеатр", "ключ2к", "значение2к");


		std::cout << ini.getValueFromVar(filePath, "Кинотеатр", "ключ2к") << std::endl;
		std::cout << ini.getValueFromVar(filePath, "Цирк", "ключ1ц") << std::endl;
	}
	catch (const char* msg)
	{
		std::cout << msg << std::endl;
	}
	std::cout << "В теории ошибок нет:)" << std::endl;

	return 0;
}
