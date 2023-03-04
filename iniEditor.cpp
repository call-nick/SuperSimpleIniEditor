#include "iniEditor.h"

void IniEdit::overWrite(std::string filePath, std::string newText)
{
	std::ofstream out(filePath, std::ios::out);

	if (out.is_open())
	{
		out << newText;
	}
	else throw "Can't open file. overWrite function";

	out.close();
}

int IniEdit::strLength(std::string str)
{
	for (int i = 0; ; i++)
		if (str[i] == '\0' && i != 0) return i;
}

int IniEdit::getSectionPos(std::string filePath, std::string sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	if (checkSection(filePath, sectionName)) return (int) allText.find('[' + sectionName + ']');
	throw "There is no section. getSectionPos function";
}

int IniEdit::getVarPos(std::string filePath, std::string sectionName, std::string varName)
{
	std::string allText = getAllTextInFile(filePath);

	if (checkVarInSection(filePath, sectionName, varName))
	{
		return allText.find(varName, (getSectionPos(filePath, sectionName) + strLength('[' + sectionName + ']')));
	}
	throw "In section no var. getVarPos function";
}

int IniEdit::getVarLineNumber(std::string filePath, std::string sectionName, std::string varName)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	bool sectionPassed = false;
	int lineNumber = 0;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{			
			if (sectionPassed && (buffer.find(varName) != std::string::npos)) {
				in.close();
				return lineNumber;
			}
			else if (buffer.find('[' + sectionName + ']') != std::string::npos) //This could be a vulnerability, because the [ symbol may be contained in var name or varValue name
				sectionPassed = true;

			lineNumber++;
		}
	}
	else throw "Can't open file. getVarLineNumber function";

	in.close();
}

int IniEdit::getLineCount(std::string filePath)
{
	std::string buffer;
	int lineNumber = 0;

	std::ifstream in(filePath, std::ios::in);
	if (in.is_open())
	{
		while (std::getline(in, buffer))
			lineNumber++;
		return lineNumber;
	}
	throw "Can't open file. getLineCount function";

	in.close();
}

bool IniEdit::checkSection(std::string filePath, std::string sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	if (allText.find('[' + sectionName + ']') == std::string::npos)
		return false;

	return true;
}

bool IniEdit::checkVarInSection(std::string filePath, std::string sectionName, std::string varName)
{
	std::string allText = getAllTextInFile(filePath);

	int sectionPos = getSectionPos(filePath, sectionName);
	int nextSectionPos = allText.find("[", (sectionPos + strLength('[' + sectionName + ']')));  //This could be a vulnerability, because the [ symbol may be contained in var name or varValue name
	int varPos = allText.find(varName, sectionPos);

	if (varPos != std::string::npos)
	{
		if (nextSectionPos == std::string::npos) return true;
		else if (nextSectionPos > varPos) return true;
	}
	return false;
}

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
	else throw "Can't open file. getAllTextInFile Function";

	in.close();

	return allText;
}

void IniEdit::addSection(std::string filePath, std::string sectionName)
{
	std::ofstream out(filePath, std::ios::app);

	if (out.is_open() && !checkSection(filePath, sectionName))
	{
		out << "\n\n" << '[' + sectionName + ']';
	}
	//else throw "Error occured in addSection Function";

	out.close();
}

void IniEdit::addVarInSection(std::string filePath, std::string sectionName, std::string varName, std::string varValue)
{
	std::string allText = getAllTextInFile(filePath);
	int sectionPos = getSectionPos(filePath, sectionName);

	if (sectionPos != std::string::npos && !checkVarInSection(filePath, sectionName, varName))
	{
		std::string partBefore = allText.substr(0, (sectionPos + strLength('[' + sectionName + ']'))) + '\n';
		std::string partVar = varName + '=' + varValue + '\n';
		std::string partAfter = allText.substr(sectionPos + strLength('[' + sectionName + ']' + '\n'));
		
		overWrite(filePath, partBefore + partVar + partAfter);
	}
	
	else if (sectionPos != std::string::npos && checkVarInSection(filePath, sectionName, varName))
	{
		std::string oldValue = getValueFromVar(filePath, sectionName, varName);
		int varPos = getVarPos(filePath, sectionName, varName);
		std::string partBefore = allText.substr(0, (varPos + strLength(varName))) + '=';
		std::string partVar = varValue + '\n';
		std::string partAfter = allText.substr(varPos + strLength(varName + '=') + strLength(oldValue + '\n'));

		overWrite(filePath, partBefore + partVar + partAfter);
	}

	else throw "Maybe incorrect sectionName. addVarInSection";
}

void IniEdit::setValueByIndex(std::string filePath, std::string sectionName, int index, std::string varValue)
{
	std::string buffer;

	bool sectionPassed = false;

	int lineNumber = 0;

	std::ifstream in(filePath, std::ios::in);
	if (in.is_open())
	{
		for (int i = 0; ; i++)
		{
			std::getline(in, buffer);
			if (buffer.find('[' + sectionName + ']') != std::string::npos)
			{
				lineNumber = -1;
				sectionPassed = true;
			}

			if (buffer == "" && sectionPassed) throw "Out of range. getValueByIndex";
			if (sectionPassed && index == lineNumber)
			{
				in.close();
				addVarInSection(filePath, sectionName, buffer.substr(0, buffer.find('=')), varValue);
				break;
			}

			lineNumber++;
		}
	}
	else throw "Maybe out of range. getValueByIndex";

}

std::string IniEdit::getValueFromVar(std::string filePath, std::string sectionName, std::string varName)
{
	std::ifstream in(filePath, std::ios::in);
	std::string allText = getAllTextInFile(filePath);

	std::string buffer;

	if (checkVarInSection(filePath, sectionName, varName))
	{
		for (int i = 0; i <= getVarLineNumber(filePath, sectionName, varName); i++)
		{
			std::getline(in, buffer);
		}

		return buffer.substr((buffer.find('=') + 1));
	}
	throw "Maybe no var in section. getValueFromVar";
}

std::string IniEdit::getValueByIndex(std::string filePath, std::string sectionName, int index)
{
	std::string buffer;

	bool sectionPassed = false;

	int lineNumber = 0;

	std::ifstream in(filePath, std::ios::in);
	if (in.is_open())
	{
		for (int i = 0; i < getLineCount(filePath); i++)
		{
			std::getline(in, buffer);
			if (buffer.find('[' + sectionName + ']') != std::string::npos)
			{
				lineNumber = -1;
				sectionPassed = true;
			}

			if (buffer == "" && sectionPassed) throw "Out of range. getValueByIndex";
			if (sectionPassed && index == lineNumber) return buffer.substr(buffer.find('=')+1);

			lineNumber++;
		}
		in.close();
		throw "Maybe out of range or can't be found. getValueByIndex";
	}
	in.close();
	throw "Maybe out of range. getValueByIndex";

}
