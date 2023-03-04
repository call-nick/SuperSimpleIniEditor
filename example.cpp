#include "iniEditor.h"

int main()
{
	system("chcp 1251");

	IniEdit ini;
	try {
		std::string section = "Nick5";
		std::string filePath = "File.ini";
		std::string btn = "btn1";



		ini.addSection(filePath, "���������");
		ini.addSection(filePath, "�����");
		ini.addSection(filePath, "����");


		ini.addVarInSection(filePath, "���������", "����1�", "��������1�");


		//ini.addVarInSection(filePath, "����������", "����1", "��������1"); //�� ����������, �.�. � ��� ��� ���� ����������

		ini.addVarInSection(filePath, "����", "����1�", "��������1�");
		//ini.addVarInSection(filePath, "����", "����1�", "��������1�");

		ini.addVarInSection(filePath, "���������", "����2�", "��������2�");


		std::cout << ini.getValueFromVar(filePath, "���������", "����2�") << std::endl;
		std::cout << ini.getValueFromVar(filePath, "����", "����1�") << std::endl;
	}
	catch (const char* msg)
	{
		std::cout << msg << std::endl;
	}
	std::cout << "� ������ ������ ���:)" << std::endl;

	return 0;
}
