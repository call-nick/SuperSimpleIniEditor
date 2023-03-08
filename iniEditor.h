#pragma once
#include <iostream>
#include <fstream>
#include <string>

class IniEdit

{	//Overwrites all text in the file to the newText
	void overWrite(std::string filePath, std::string newText);

	//Returns the length of the string excluding \0 symbol
	int strLength(std::string str);

	//Returns the number of the position "[" + sectionName + "]" in the file (string, contains all file data) named filePath if it exists
	int getPos(std::string filePath, std::string sectionName);

	//Returns position varName in the string, contains all file data named filePath in the Section sectionName if it exists
	int getPos(std::string filePath, std::string sectionName, std::string varName);

	//Returns the line number of varName in the file starting from 0
	int getLineNumber(std::string filePath, std::string sectionName, std::string varName);

	//Returns the line number of sectionName in the file starting from 0
	int getLineNumber(std::string filePath, std::string sectionName);

	//Returns the number of lines in the file starting FROM 0
	int getLineCount(std::string filePath);

	//Returns true, if Section named sectionName is already exists in file filePath
	bool checkSection(std::string filePath, std::string sectionName);

	//Returns true, if section after sectionName is already exists in file filePath
	bool checkNextSection(std::string filePath, std::string sectionName);

	//Returns true, if Var named varName is already exists in Section sectionName
	bool checkVarInSection(std::string filePath, std::string varName, std::string sectionName);

	//Returns string contains all the text of the file with \n symbols
	std::string getAllTextInFile(std::string filePath);

	//Returns string separated from '='. Mode "after" returns string after '=' symbol, other - before '=' symbol
	std::string separateString(std::string str, std::string mode);

	//Returns section name located after sectionName
	std::string getNextSectionName(std::string filePath, std::string sectionName);

public:

	//Creating new Section, named sectionName, if it doesn't exist
	void addSection(std::string filePath, std::string sectionName);

	//Making a var beneath a [SectionName] or replacing it value, if it already exists
	void addVarInSection(std::string filePath, std::string sectionName, std::string varName, std::string varValue);

	//Making all vars that are contained in Section sectionName equal to varValue
	void setVarsInSection(std::string filePath, std::string sectionName, std::string varValue);

	//Allow to refer to sectionName in file by its index starting with 0
	void setValueByIndex(std::string filePath, int index, std::string value);

	//Allow to refer to varName in sectionName by its index starting with 0 from sectionName and set its value
	void setValueByIndex(std::string filePath, std::string sectionName, int index, std::string varValue);

	//Delete Section named sectionName and all vars in it
	void deleteSection(std::string filePath, std::string sectionName);

	//Delete var named varName and its value
	void deleteVarInSection(std::string filePath, std::string sectionName, std::string varName);

	//Rename Section named sectionName and all vars in it
	void renameSection(std::string filePath, std::string sectionName, std::string newSectionName);

	//Copy text from one file to another
	void copyFromTo(std::string filePath, std::string newFilePath);

	//Returns the number of sections starting FROM 1
	int getSectionsCount(std::string filePath);

	//Returns the value of the var in Section named sectionName if varName exists
	std::string getValueFromVar(std::string filePath, std::string sectionName, std::string varName);

	//Allow to refer to sectionName in file by its index starting with 0
	std::string getValueByIndex(std::string filePath, int index);

	//Allow to refer to varName in sectionName by its index starting with 0 from sectionName and get its value
	std::string getValueByIndex(std::string filePath, std::string sectionName, int index);
};
