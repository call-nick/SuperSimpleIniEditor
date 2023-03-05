#include "iniEditor.h"

using std::string;
using std::cout;
using std::endl;
using std::to_string;


bool test1()
{
	IniEdit  ini;
	string filePath = "file.ini"; //You can find it at the root of the project.

	try
	{

		ini.addSection(filePath, "Device settings");
		ini.addSection(filePath, "Other");

		ini.addVarInSection(filePath, "Device settings", "brightness ", " 100%");
		ini.addVarInSection(filePath, "Device settings", "volume ", " 60%");

		ini.addVarInSection(filePath, "Other", "cpu ", " 40%");
		ini.addVarInSection(filePath, "Device settings", "weight ", " 200g");

		ini.addVarInSection(filePath, "Other", "cpu ", " 70%");
		ini.addVarInSection(filePath, "Device settings", "weight ", " 250g");

		//ini.addVarInSection(filePath, "Device settings", "brightness ", " 80%");
		ini.addVarInSection(filePath, "Device settings", "volume ", " 45%");

		ini.addVarInSection(filePath, "Other", "memory ", " 30%");

		string str1 = ini.getValueFromVar(filePath, "Device settings", "brightness ");
		string str2 = ini.getValueFromVar(filePath, "Other", "memory ");


		cout << "Settings -> brightness " << str1 << endl;
		cout << "Other -> memory " << str2 << endl;


		ini.addSection(filePath, "Loop test1");

		for (int i = 0; i < 15; i++)
			ini.addVarInSection(filePath, "Loop test1", to_string(i), "0");

		ini.addSection(filePath, "Loop test2");
		for (int i = 0; i < 15; i++)
			ini.addVarInSection(filePath, "Loop test2", to_string(i), "0");


		ini.addSection(filePath, "Loop test2");
		for (int i = 14; i >= 0; i--)
			ini.addVarInSection(filePath, "Loop test2", to_string(i), "1");

		ini.addSection(filePath, "Reverse loop test");
		for (int i = 14; i >= 0; i--)
			ini.addVarInSection(filePath, "Reverse loop test", to_string(i), to_string(i));

		for (int i = 0; i < 15; i++)
			cout << ini.getValueByIndex(filePath, "Reverse loop test", i) << endl;


		for (int i = 14; i >= 0; i--)
			ini.setValueByIndex(filePath, "Loop test1", i, to_string(i));


		ini.setVarsInSection(filePath, "Loop test1", "0");


	}

	catch (const char* msg)
	{
		cout << msg << endl;
	}

	return true;

}

bool test2()
{
	IniEdit  ini;
	string filePath = "file2.ini"; //You can find it at the root of the project.

	try
	{

		ini.addSection(filePath, "Loop test1");

		for (int i = 0; i < 20; i++)
			ini.addVarInSection(filePath, "Loop test1", to_string(i), "0");

		ini.addSection(filePath, "Loop test2");
		for (int i = 19; i >= 0; i--)
			ini.addVarInSection(filePath, "Loop test2", to_string(i), to_string(i));

		//for (int i = 1; i > 15; i++)
		//ini.addVarInSection(filePath, "Loop test2", to_string(i), "1");


		ini.addSection(filePath, "Reverse loop test");
		for (int i = 19; i >= 0; i--)
			ini.addVarInSection(filePath, "Reverse loop test", to_string(i), to_string(i));

		//for (int i = 0; i < 20; i++)
			//cout << ini.getValueByIndex(filePath, "Reverse loop test", i) << endl;


		//for (int i = 14; i >= 0; i--)
			//ini.setValueByIndex(filePath, "Loop test1", i, to_string(i));


		ini.addSection(filePath, "New section");
		for (int i = 19; i >= 0; i--)
			ini.addVarInSection(filePath, "New section", to_string(i), to_string(i-5));

		for (int i = 0; i < 20; i++)
			ini.setValueByIndex(filePath, "New section", i, ini.getValueByIndex(filePath, "New section", i));
	
		ini.setVarsInSection(filePath, "Loop test2", "You all equal me");

	}

	catch (const char* msg)
	{
		cout << msg << endl;
	}

	return true;
}

int main(int argc, char* arg[])
{
	if (test1()) cout << "\nTest1 succeed";
	if (test2()) cout << "\nTest2 succeed";
	

	return 0;
}
