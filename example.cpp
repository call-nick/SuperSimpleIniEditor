#include "iniEditor.h"

int main()
{
	system("chcp 1251"); //'cause I need to change code page for my native language

	IniEdit ini;
	try {

		std::string filePath = "File.ini";

		ini.addSection(filePath, "Section1");
		ini.addSection(filePath, "Section2");
		ini.addSection(filePath, "Section3");
		ini.addSection(filePath, "Section4");


		ini.addVarInSection(filePath, "Section1", "key1k", "value1k");
		ini.addVarInSection(filePath, "Section1", "key1k", "newValue1k");
		ini.addVarInSection(filePath, "Section1", "key2к", "value2k");
		ini.addVarInSection(filePath, "Section1", "key3k", "value3k");

		ini.addVarInSection(filePath, "Section2", "key1c", "value1c");
		ini.addVarInSection(filePath, "Section2", "key2c", "newValue2c");
		ini.addVarInSection(filePath, "Section2", "key1c", "value1c");

		ini.addVarInSection(filePath, "Section4", "кey1o", "value1o");


		std::cout << ini.getValueFromVar(filePath, "Section1", "key1k") << std::endl;
		std::cout << ini.getValueFromVar(filePath, "Section2", "key2c") << std::endl;
		std::cout << ini.getValueByIndex(filePath, "Section1", 1) << std::endl;

		ini.setValueByIndex(filePath, "Section1", 2, "settedValue");

		std::cout << ini.getValueByIndex(filePath, "Section1", 2) << std::endl;
	}
	catch (const char* msg)
	{
		std::cout << msg << std::endl;
	}

	return 0;
}
