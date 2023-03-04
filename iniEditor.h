#pragma once
#include <iostream>
#include <fstream>
#include <string>

class IniEdit
{
	void overWrite(std::string filePath, std::string newText);

	int strLength(std::string str);

	int getSectionPos(std::string filePath, std::string sectionName);

	int getVarPos(std::string filePath, std::string sectionName,  std::string varName );

	int getVarRowNumber(std::string filePath, std::string sectionName, std::string varName);

	bool checkSection(std::string filePath, std::string sectionName);

	bool checkVarInSection(std::string filePath, std::string varName, std::string sectionName);

	std::string getAllTextInFile(std::string filePath);
		
public:

	void addSection(std::string filePath, std::string sectionName);

	void addVarInSection(std::string filePath, std::string sectionName, std::string varName, std::string varValue);

	std::string getValueFromVar(std::string filePath, std::string sectionName, std::string varName);

};