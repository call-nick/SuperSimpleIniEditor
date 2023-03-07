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
		return 1 + allText.find('\n' + varName + '=', (getSectionPos(filePath, sectionName) + strLength('[' + sectionName + ']')));
	}// '\n' because I need ensure that I get my var, but not the part of another and +1 because I need compensate '\n' position
	throw "In section no var. getVarPos function";
}

int IniEdit::getLineNumber(std::string filePath, std::string sectionName, std::string varName)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	bool sectionPassed = false;
	int lineNumber = 0;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{
			if (sectionPassed && separateString(buffer, "before") == varName) {
				in.close();
				return lineNumber;
			}
			else if (buffer.find('[' + sectionName + ']') != std::string::npos) //This could be a vulnerability, because the [ symbol may be contained in var name or varValue name
				sectionPassed = true;

			lineNumber++;
		}
	}
	else throw "Can't open file. getVarLineNumber function for vars";

	in.close();
}

int IniEdit::getLineNumber(std::string filePath, std::string sectionName)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	int lineNumber = 0;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{
			if ((buffer.find(sectionName) != std::string::npos)) {
				in.close();
				return lineNumber;
			}
			lineNumber++;
		}
	}
	else throw "Can't open file. getLineNumber function for sections";

	in.close();
	return std::string::npos;
}

int IniEdit::getNextSectionLineNumber(std::string filePath, std::string sectionName)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	bool sectionPassed = false;
	int lineNumber = 0;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{
			if (sectionPassed && buffer.find("[") != std::string::npos) return lineNumber;
			else if ((buffer.find(sectionName) != std::string::npos))
			{
				sectionPassed = true;
			}
			lineNumber++;
		}
		return lineNumber;
	}
	else throw "Can't open file. getNextSectionLineNumber function for sections";

	in.close();
	return std::string::npos;
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
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	bool sectionPassed = false;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{
			if (sectionPassed && separateString(buffer, "before") == varName)
			{
				in.close();
				return true;
			}
			else if (sectionPassed && buffer.find('[') != std::string::npos)
				return false;
			else if (buffer.find('[' + sectionName + ']') != std::string::npos) //This could be a vulnerability, because the [ symbol may be contained in var name or varValue name
				sectionPassed = true;
		}
		return false;
	}
	else throw "Can't open file. checkVarInSection function";

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

std::string IniEdit::separateString(std::string str, std::string mode)
{
	if (mode == "after")
		return str.substr(str.find('=') + 1);
	return str.substr(0, str.find('='));
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
		std::string partBefore = allText.substr(0, (sectionPos + strLength('[' + sectionName + ']' + '\n')));
		std::string partVar = varName + '=' + varValue;
		std::string partAfter = allText.substr(sectionPos + strLength('[' + sectionName + ']'));

		overWrite(filePath, partBefore + partVar + partAfter);
	}

	else if (sectionPos != std::string::npos && checkVarInSection(filePath, sectionName, varName))
	{
		std::string oldValue = getValueFromVar(filePath, sectionName, varName);
		int varPos = getVarPos(filePath, sectionName, varName);
		std::string partBefore = allText.substr(0, (varPos + strLength(varName))) + '=';
		std::string partVar = varValue;
		std::string partAfter = allText.substr(varPos + strLength(varName + '=') + strLength(oldValue));

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

			if (buffer == "" && sectionPassed) return;
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

void IniEdit::setVarsInSection(std::string filePath, std::string sectionName, std::string varValue)
{
	int sectionLineNumber = getLineNumber(filePath, sectionName);
	int nextSectionLineNumber = getNextSectionLineNumber(filePath, sectionName);
	for (int i = 0; i < nextSectionLineNumber - sectionLineNumber; i++)
		setValueByIndex(filePath, sectionName, i, varValue);

}

void IniEdit::deleteSection(std::string filePath, std::string sectionName)
{
	//Добавить проверку, а существует ли такая секция вообще

	std::string allText = getAllTextInFile(filePath);

	std::string partBefore = allText.substr(0, getSectionPos(filePath, sectionName) - strLength("[")) + "\n";
	std::string partAfter = "";
	if (getNextSectionName(filePath, sectionName) == "there is no next section")
		partAfter = "";
	else
		partAfter = allText.substr(getSectionPos(filePath, getNextSectionName(filePath, sectionName)));

	overWrite(filePath, partBefore + partAfter);
}

std::string IniEdit::getValueFromVar(std::string filePath, std::string sectionName, std::string varName)
{
	std::ifstream in(filePath, std::ios::in);
	std::string allText = getAllTextInFile(filePath);

	std::string buffer;

	if (checkVarInSection(filePath, sectionName, varName))
	{
		for (int i = 0; i <= getLineNumber(filePath, sectionName, varName); i++)
			std::getline(in, buffer);

		return separateString(buffer, "after");
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
			if (sectionPassed && index == lineNumber) return buffer.substr(buffer.find('=') + 1);

			lineNumber++;
		}
		in.close();
		throw "Maybe out of range or can't be found. getValueByIndex";
	}
	in.close();
	throw "Maybe out of range. getValueByIndex";
}
	std::string IniEdit::getNextSectionName(std::string filePath, std::string sectionName)
	{
		std::ifstream in(filePath, std::ios::in);

		std::string buffer;
		bool sectionPassed = false;
		int lineNumber = 0;

		if (in.is_open())
		{
			while (std::getline(in, buffer))
			{
				if (sectionPassed && buffer.find("[") != std::string::npos) return buffer.substr(1, strLength(buffer)-2); //Изменить так, чтобы не пришлось ставить минус два
				else if ((buffer.find(sectionName) != std::string::npos))
				{
					sectionPassed = true;
				}
				lineNumber++;
			}
			in.close();
			return "there is no next section";
		}
		else throw "Can't open file. getNextSectionName function";

	}
