#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

class IniEdit

{	//Overwrites all text in the file to the newText
	void overWrite(const std::string& filePath, std::string_view newText);

	//Returns the length of the string excluding \0 symbol
	int strLength(std::string_view str);

	//Returns the number of the position "[" + sectionName + "]" in the file (string, contains all file data) named filePath if it exists
	int getPos(const std::string& filePath, const std::string& sectionName);

	//Returns position varName in the string, contains all file data named filePath in the Section sectionName if it exists
	int getPos(const std::string& filePath, const std::string& sectionName, const std::string& varName);

	//Returns the line number of varName in the file starting from 0
	int getLineNumber(const std::string& filePath, const std::string& sectionName, std::string_view varName);

	//Returns the line number of sectionName in the file starting from 0
	int getLineNumber(const std::string& filePath, const std::string& sectionName);

	//Returns the number of lines in the file starting FROM 0
	int getLineCount(const std::string& filePath);

	//Returns true, if Section named sectionName is already exists in file filePath
	bool checkSection(const std::string& filePath, const std::string& sectionName);

	//Returns true, if section after sectionName is already exists in file filePath
	bool checkNextSection(const std::string& filePath, const std::string& sectionName);

	//Returns true, if Var named varName is already exists in Section sectionName
	bool checkVarInSection(const std::string& filePath, const std::string& varName, std::string_view sectionName);

	//Returns string contains all the text of the file with \n symbols
	std::string getAllTextInFile(const std::string& filePath);

	//Returns string separated from '='. Mode "after" returns string after '=' symbol, other - before '=' symbol
	std::string separateString(const std::string& str, std::string_view mode);

	//Returns section name located after sectionName
	std::string getNextSectionName(const std::string& filePath, const std::string& sectionName);

public:

	//Creating new Section, named sectionName, if it doesn't exist
	void addSection(const std::string& filePath, const std::string& sectionName);

	//Making a var beneath a [SectionName] or replacing it value, if it already exists
	void addVarInSection(const std::string& filePath, const std::string& sectionName, const std::string& varName, const std::string& varValue);

	//Making all vars that are contained in Section sectionName equal to varValue
	void setVarsInSection(const std::string& filePath, const std::string& sectionName, const std::string& varValue);

	//Allow to refer to sectionName in file by its index starting with 0
	void setValueByIndex(const std::string& filePath, int index, const std::string& value);

	//Allow to refer to varName in sectionName by its index starting with 0 from sectionName and set its value
	void setValueByIndex(const std::string& filePath, const std::string& sectionName, int index, const std::string& varValue);

	//Delete Section named sectionName and all vars in it
	void deleteSection(const std::string& filePath, const std::string& sectionName);

	//Delete var named varName and its value
	void deleteVarInSection(const std::string& filePath, const std::string& sectionName, const std::string& varName);

	//Rename Section named sectionName and all vars in it
	void renameSection(const std::string& filePath, const std::string& sectionName, const std::string& newSectionName);

	//Copy text from one file to another
	void copyFromTo(const std::string& filePath, const std::string& newFilePath);

	//Returns the number of sections starting FROM 1
	int getSectionsCount(const std::string& filePath);

	//Returns the value of the var in Section named sectionName if varName exists
	std::string getValueFromVar(const std::string& filePath, const std::string& sectionName, const std::string& varName);

	//Allow to refer to sectionName in file by its index starting with 0
	std::string getValueByIndex(const std::string& filePath, int index);

	//Allow to refer to varName in sectionName by its index starting with 0 from sectionName and get its value
	std::string getValueByIndex(const std::string& filePath, const std::string& sectionName, int index);
};