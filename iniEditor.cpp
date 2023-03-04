#include "iniEditor.h"

//It 100% works. Creating new Section, named sectionNamed, if it doesn't exist
void IniEdit::addSection(std::string filePath, std::string sectionName)
{
	std::ofstream out(filePath, std::ios::app);

	if (out.is_open() && !checkSection(filePath, sectionName))
	{
		out << "\n" << '[' + sectionName + ']' << "\n";
	}
	//else throw "Error occured in addSection Function";

	out.close();
}

//It 100% works. Returns true, if Section named sectionName is already exists
bool IniEdit::checkSection(std::string filePath, std::string sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	if (allText.find('[' + sectionName + ']') == std::string::npos)
		return false;

	return true;
}

//It 100% works. Returns string contains all the text of the file
std::string IniEdit::getAllTextInFile(std::string filePath)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer = "";
	std::string allText = "";

	if (in.is_open())
	{
		while (std::getline(in, buffer))
			allText += buffer + "\n";
	}
	else throw "Error occured in getAllTextInFile Function";

	in.close();

	return allText;
}

//It 100% works. Returns the number of the position [sectionName] in the string
int IniEdit::getSectionPos(std::string filePath, std::string sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	if (allText.find(sectionName) == std::string::npos) throw "Error occured in getSectionPos";
	return (int) allText.find('[' + sectionName + ']');
}

//It 100% works. Overwrites text in the file to the newText 
void IniEdit::overWrite(std::string filePath, std::string newText)
{
	std::ofstream out(filePath, std::ios::out);

	if (out.is_open())
	{
		out << newText;
	}
	else throw "Error occured in overWrite Function";

	out.close();
}

//It 100% works. Returns the length of the string without \0
int IniEdit::strLength(std::string str)
{
	for (int i = 0; ; i++)
	{
		if (str[i] == '\0' && i != 0) return --i;
		if (str[i] == '\0' && i == 0) throw "Error occured in strLength function";
	}
}

//It 100% works. Returns position varName in the sectionName in the string
int IniEdit::getVarPos(std::string filePath, std::string sectionName, std::string varName)
{
	std::string allText = getAllTextInFile(filePath);

	if (checkVarInSection(filePath, varName, sectionName))
	{
		int sectionPos = getSectionPos(filePath, sectionName);
		return allText.find(varName, (sectionPos + strLength(sectionName)));
	}
	else throw "Error occured in getVarPos function";
}

//It 100% works. Returns the row number of varName in the file
int IniEdit::getVarRowNumber(std::string filePath, std::string sectionName, std::string varName)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	bool sectionPassed = false;
	int rowNumber = 0;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{
			rowNumber++;
			if (sectionPassed && (buffer.find(varName) !=  std::string::npos)) {
				in.close();
				return rowNumber;
			}
			else if (buffer.find('[' + sectionName + ']') != std::string::npos)
				sectionPassed = true;
		}		
	}
	else throw "Error occured in getVarRowNumber function";

	in.close();
}

//Maybe it works. Making a var beneath a [SectionName] or replacing it value, if it already exists
void IniEdit::addVarInSection(std::string filePath, std::string sectionName, std::string varName, std::string varValue)
{
	std::string allText = getAllTextInFile(filePath);
	int pos = getSectionPos(filePath, sectionName);
	if (getSectionPos(filePath, sectionName) != std::string::npos && !checkVarInSection(filePath, sectionName, varName))
	{
		std::string partBefore = allText.substr(0, (pos + strLength(sectionName)) + 3) + "\n";
		std::string partVar = varName + "=" + varValue;
		std::string partAfter = allText.substr(pos + strLength(sectionName) + 3);
		std::string newText = partBefore + partVar + partAfter;

		overWrite(filePath, newText);
	}

	else if (getSectionPos(filePath, sectionName) != std::string::npos && checkVarInSection(filePath, sectionName, varName))
	{
		pos = getVarPos(filePath, sectionName, varName);
		std::string partBefore = allText.substr(0, (pos + strLength(varName)) + 1) + "\n";
		std::string partVar = varValue;
		std::string partAfter = allText.substr(pos + strLength(varName) + 1 + strLength(varValue));
		std::string newText = partBefore + partVar + partAfter;

		overWrite(filePath, newText);
	}

	else throw "Error occured in addVarInSection";
}

//It 100% works. Returns true, if Var named varName is already exists in Section sectionName
bool IniEdit::checkVarInSection(std::string filePath, std::string sectionName, std::string varName)
{
	std::string allText = getAllTextInFile(filePath);

	int sectionPos = allText.find(varName, 0);
	int nextSectionPos = allText.find("[", (sectionPos + strLength(sectionName) + 2));
	int varPos = allText.find(varName, sectionPos);

	if (varPos != std::string::npos)
	{
		if (nextSectionPos == std::string::npos) return true;
		else if (nextSectionPos > varPos) return true;
		else return false;
	}
	else return false;
}

//It 100% works. Returns the value of the var in Secion named sectionName
std::string IniEdit::getValueFromVar(std::string filePath, std::string sectionName, std::string varName)
{
	std::ifstream in(filePath, std::ios::in);
	std::string allText = getAllTextInFile(filePath);

	std::string varValue;
	std::string buffer;

	if (checkVarInSection(filePath, sectionName, varName))
	{
		for (int i = 0; i < getVarRowNumber(filePath, sectionName, varName); i++)
		{
			std::getline(in, buffer);
		}

		return buffer.substr((buffer.find('=') + 1));
	}
	else throw "Error occured in getValueFromVar";
}