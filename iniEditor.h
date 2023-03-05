#pragma once
#include <iostream>
#include <fstream>
#include <string>

class IniEdit

{	//Overwrites all text in the file to the newText
	void overWrite(std::string filePath, std::string newText);

	//Returns the length of the string without \0 symbol
	int strLength(std::string str);

	//Returns the number of the position '[' + sectionName + ']' in the file name filePath if it exists
	int getSectionPos(std::string filePath, std::string sectionName);

	//Returns position varName in the file named filePath in the Section sectionName if it exists
	int getVarPos(std::string filePath, std::string sectionName, std::string varName);

	//Returns the line number of varName in the file starting from 0
	int getLineNumber(std::string filePath, std::string sectionName, std::string varName);

	//Returns the line number of section in the file starting from 0
	int getLineNumber(std::string filePath, std::string sectionName);

	//Returns the line number of next section follows the sectionName in the file
	int getNextSectionLineNumber(std::string filePath, std::string sectionName);

	//Returns the number of lines in the file
	int getLineCount(std::string filePath);

	//Returns true, if Section named sectionName is already exists in file filePath
	bool checkSection(std::string filePath, std::string sectionName);

	//Returns true, if Var named varName is already exists in Section sectionName
	bool checkVarInSection(std::string filePath, std::string varName, std::string sectionName);

	//Returns string contains all the text of the file with \n symbols
	std::string getAllTextInFile(std::string filePath);

	//Returns string separated from '='
	std::string separateString(std::string str, std::string mode);

public:

	//Creating new Section, named sectionNamed, if it doesn't exist
	void addSection(std::string filePath, std::string sectionName);

	//Making a var beneath a [SectionName] or replacing it value, if it already exists
	void addVarInSection(std::string filePath, std::string sectionName, std::string varName, std::string varValue);

	//Making all vars that are contained in Section sectionName equal to varValue
	void setVarsInSection(std::string filePath, std::string sectionName, std::string varValue);

	//Allow to refer to varName in sectionName by its index starting with 0 from sectionName
	void setValueByIndex(std::string filePath, std::string sectionName, int index, std::string varValue);

	//Returns the value of the var in Section named sectionName if varName exists
	std::string getValueFromVar(std::string filePath, std::string sectionName, std::string varName);

	//Allow to refer to varName in sectionName by its index starting with 0 from sectionName
	std::string getValueByIndex(std::string filePath, std::string sectionName, int index);

};
