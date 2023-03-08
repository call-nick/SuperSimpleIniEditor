#include "iniEditor.h"

using std::string;
using std::cout;
using std::endl;
using std::to_string;

//testing addSection, addVarInSection,
bool test1()
{
	IniEdit  ini;
	string filePath = "test1.ini"; //You can find it at the root of the project.

	try
	{

		ini.addSection(filePath, "Section1");
		ini.addSection(filePath, "Section2");
		ini.addSection(filePath, "Section3 with spaces");
		ini.addSection(filePath, "Section4");

		for (int i = 0; i < 20; i++)
			ini.addVarInSection(filePath, "Section1", to_string(i), "0");

		for (int i = 19; i >= 0; i--)
			ini.addVarInSection(filePath, "Section2", to_string(i), to_string(i));

		for (int i = 0; i < 14; i++)
			ini.addVarInSection(filePath, "Section2", to_string(i), "1");

		for (int i = 0; i < 20; i++)
			ini.addVarInSection(filePath, "Section4", to_string(i), "0");
	}

	catch (const char* msg)
	{
		cout << msg << endl;
		return false;
	}

	return true;

}

//testing copyFromTO, getValueByIndex, addVarInSection, setValueByIndex, getValueByIndex, renameSection
bool test2()
{
	IniEdit  ini;
	string filePath = "test2.ini";


	ini.copyFromTo("test1.ini", filePath);

	string sectionIndex2 = ini.getValueByIndex(filePath, 3);

	try
	{
		for (int i = 0; i < 20; i++)
			ini.addVarInSection(filePath, "Section3 with spaces", to_string(i), "0");

		for (int i = 0; i < 20; i++)
			ini.setValueByIndex(filePath, "Section3 with spaces", i, to_string(i));

		for (int i = 0; i < 20; i++)
			cout << ini.getValueByIndex(filePath, "Section3 with spaces", i) << " ";

		for (int i = 19; i >= 0; i--)
			ini.setValueByIndex(filePath, sectionIndex2, i, to_string(i));

		for (int i = 0; i < 3; i++)
			cout << "\n" << ini.getValueByIndex(filePath, i) << " ";

		if (ini.getValueByIndex(filePath, 3) == "Section4")
			ini.renameSection(filePath, "Section4", "Renamed");
		else
			ini.renameSection(filePath, "Renamed", "Section4");

		ini.setValueByIndex(filePath, 0, "SuperSection");

	}

	catch (const char* msg)
	{
		cout << msg << endl;
		return false;
	}

	return true;
}

//testing copyFromTO, deleteSection, getSectionsCount, deleteVarInSection, getValueByIndex, renameSection
bool test3()
{
	IniEdit  ini;
	string filePath = "test3.ini";

	ini.copyFromTo("test2.ini", filePath);

	try
	{
		ini.deleteSection(filePath, "Section3 with spaces");

		cout << "\nNumber of sections: " << ini.getSectionsCount(filePath);

		ini.deleteVarInSection(filePath, "Renamed", "4");
		ini.deleteVarInSection(filePath, "SuperSection", "3");

		ini.deleteVarInSection(filePath, "Renamed", "17");
		ini.deleteVarInSection(filePath, "SuperSection", "8");

		ini.setVarsInSection(filePath, "SuperSection", "all equal");
	}

	catch (const char* msg)
	{
		cout << msg << endl;
		return false;
	}

	return true;
}

int main(int argc, char* arg[])
{

	system("chcp 1251");


	cout << "\nTest1:";
	test1() ? cout << "\nsucceed\n" : cout << "\nfailed";

	cout << "\nTest2:";
	test2() ? cout << "\nsucceed\n" : cout << "\nfailed";

	cout << "\nTest3:";
	test3() ? cout << "\nsucceed\n": cout << "\nfailed";

	return 0;
}
