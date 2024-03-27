#include "iniEditor.h"

void IniEdit::overWrite(const std::string& filePath, std::string_view newText)
{
	std::ofstream out(filePath, std::ios::out);

	if (out.is_open())
	{
		out << newText;
	}
	else throw "can't open file. overWrite function";

	out.close();
}

void IniEdit::copyFromTo(const std::string& filePath, const std::string& newFilePath)
{
	std::string allText = getAllTextInFile(filePath);

	std::ofstream out(newFilePath, std::ios::out);

	if (out.is_open())
	{
		out << allText;
	}
	else throw "can't open file. overWrite function";

	out.close();
}

int IniEdit::strLength(std::string_view str)
{
	return str.size();
}

int IniEdit::getPos(const std::string& filePath, const std::string& sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	if (checkSection(filePath, sectionName)) return (int)allText.find('[' + sectionName + ']');
	throw "there is no section. getPos function";
}

int IniEdit::getPos(const std::string& filePath, const std::string& sectionName, const std::string& varName)
{
	std::string allText = getAllTextInFile(filePath);

	if (checkVarInSection(filePath, sectionName, varName))
	{
		return 1 + allText.find('\n' + varName + '=', (getPos(filePath, sectionName) + strLength('[' + sectionName + ']')));
	}// '\n' because I need ensure that I get my var, but not the part of another and +1 because I need compensate '\n' position
	throw "in section no var. getPos function";
}

int IniEdit::getLineNumber(const std::string& filePath, const std::string& sectionName)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer;
	int lineNumber = 0;

	if (in.is_open())
	{
		while (std::getline(in, buffer))
		{
			if ((buffer.find("[" + sectionName + "]") != std::string::npos)) {
				in.close();
				return lineNumber;
			}
			lineNumber++;
		}
	}
	else throw "can't open file. getLineNumber function (for sections)";

	in.close();
	return std::string::npos;
}

int IniEdit::getLineNumber(const std::string& filePath, const std::string& sectionName, std::string_view varName)
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
			else if (buffer.find("[" + sectionName + "]") != std::string::npos)
				sectionPassed = true;

			lineNumber++;
		}
	}
	else throw "can't open file. getLineNumber function (for vars)";

	in.close();
	return std::string::npos;
}

int IniEdit::getLineCount(const std::string& filePath)
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
	else throw "сan't open file. getLineCount function";

	in.close();
	return std::string::npos;
}

int IniEdit::getSectionsCount(const std::string& filePath)
{
	std::string buffer;
	int lineNumber = 0;

	std::ifstream in(filePath, std::ios::in);
	if (in.is_open())
	{
		while (std::getline(in, buffer))
			if (buffer.find("[") != std::string::npos)
				lineNumber++;
		return lineNumber;
	}
	else throw "сan't open file. getLineCount function";

	in.close();
	return std::string::npos;
}

bool IniEdit::checkSection(const std::string& filePath, const std::string& sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	return (allText.find("[" + sectionName + "]") != std::string::npos);
}

bool IniEdit::checkNextSection(const std::string& filePath, const std::string& sectionName)
{
	std::string allText = getAllTextInFile(filePath);
	int sectionPos = -1;
	if (checkSection(filePath, sectionName)) //Может убрать лишнюю проверерку?
		sectionPos = allText.find("[" + sectionName + "]");

	return allText.find("[", sectionPos + strLength(sectionName)) != std::string::npos;
}

bool IniEdit::checkVarInSection(const std::string& filePath, const std::string& sectionName, std::string_view varName)
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
			else if (sectionPassed && buffer.find("[") != std::string::npos) //This could be a vulnerability, because the [ symbol may be contained in var name or varValue name// Заменить на получение имени следующей секции
				return false;
			else if (buffer.find("[" + sectionName + "]") != std::string::npos)
				sectionPassed = true;
		}
		return false;
	}
	else throw "can't open file. checkVarInSection function";
}

void IniEdit::addSection(const std::string& filePath, const std::string& sectionName)
{
	std::ofstream out(filePath, std::ios::app);

	if (out.is_open() && !checkSection(filePath, sectionName))
	{
		out << "\n\n" << "[" + sectionName + "]";
	}

	out.close();
}

void IniEdit::addVarInSection(const std::string& filePath, const std::string& sectionName, const std::string& varName, const std::string& varValue)
{
	std::string allText = getAllTextInFile(filePath);
	int sectionPos = getPos(filePath, sectionName);

	if (sectionPos != std::string::npos && !checkVarInSection(filePath, sectionName, varName))
	{
		std::string partBefore = allText.substr(0, (sectionPos + strLength("[" + sectionName + "]" + "\n")));
		std::string partVar = varName + "=" + varValue;
		std::string partAfter = allText.substr(sectionPos + strLength("[" + sectionName + "]"));

		overWrite(filePath, partBefore + partVar + partAfter);
	}

	else if (sectionPos != std::string::npos && checkVarInSection(filePath, sectionName, varName))
	{
		std::string oldValue = getValueFromVar(filePath, sectionName, varName);
		int varPos = getPos(filePath, sectionName, varName);
		std::string partBefore = allText.substr(0, (varPos + strLength(varName))) + "=";
		std::string partVar = varValue;
		std::string partAfter = allText.substr(varPos + strLength(varName + "=") + strLength(oldValue)); //Review later

		overWrite(filePath, partBefore + partVar + partAfter);
	}

	else throw "maybe incorrect sectionName. addVarInSection";
}

void IniEdit::setVarsInSection(const std::string& filePath, const std::string& sectionName, const std::string& varValue)
{
	int sectionLineNumber = getLineNumber(filePath, sectionName);
	int nextSectionLineNumber = -1;
	int numberVarsInSection = getLineCount(filePath) - 1 - sectionLineNumber;
	if (checkNextSection(filePath, sectionName))
	{
		nextSectionLineNumber = getLineNumber(filePath, getNextSectionName(filePath, sectionName));
		for (int i = 0; i < nextSectionLineNumber - sectionLineNumber - 2; i++) //-2 because before every section \n\n symbols
			setValueByIndex(filePath, sectionName, i, varValue);
	}
	else
		for (int i = 0; i < numberVarsInSection; i++)
			setValueByIndex(filePath, sectionName, i, varValue);
}

void IniEdit::setValueByIndex(const std::string& filePath, const std::string& sectionName, int index, const std::string& varValue)
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
			if (buffer.find("[" + sectionName + "]") != std::string::npos)
			{
				lineNumber = -1;
				sectionPassed = true;
			}

			else if (buffer == "" && sectionPassed) throw "there is no var in section. setValueByIndex";

			else if (sectionPassed && index == lineNumber)
			{
				in.close();
				addVarInSection(filePath, sectionName, separateString(buffer, "before"), varValue);
				break;
			}

			lineNumber++;
		}
	}
	else throw "maybe out of range. setValueByIndex";
}

void IniEdit::setValueByIndex(const std::string& filePath, int index, const std::string& newSectionName)
{
	std::string allText = getAllTextInFile(filePath);
	const std::string& sectionName = getValueByIndex(filePath, index);

	renameSection(filePath, sectionName, newSectionName);
}

void IniEdit::deleteSection(const std::string& filePath, const std::string& sectionName)
{
	std::string allText = getAllTextInFile(filePath);

	std::string partBefore;
	std::string partAfter;

	if (checkSection(filePath, sectionName))
	{
		partBefore = allText.substr(0, getPos(filePath, sectionName) - strLength("[")) + "\n";//Review

		if (checkNextSection(filePath, sectionName))
			partAfter = allText.substr(getPos(filePath, getNextSectionName(filePath, sectionName)));
		else partAfter = "";

	}
	else throw "no section. deleteSection func";
	overWrite(filePath, partBefore + partAfter);
}

void IniEdit::deleteVarInSection(const std::string& filePath, const std::string& sectionName, const std::string& varName)
{
	std::string allText = getAllTextInFile(filePath);

	if (checkSection(filePath, sectionName) && checkVarInSection(filePath, sectionName, varName))
	{
		allText.replace(getPos(filePath, sectionName, varName), strLength(varName + getValueFromVar(filePath, sectionName, varName) + "=\n"), "");
	}
	else throw "no section or var. deleteVarInSection func";
	overWrite(filePath, allText);
}

void IniEdit::renameSection(const std::string& filePath, const std::string& sectionName, const std::string& newSectionName)
{
	std::string allText = getAllTextInFile(filePath);

	std::string partBefore;
	std::string partSec;
	std::string partAfter;

	if (checkSection(filePath, sectionName))
	{
		partBefore = allText.substr(0, getPos(filePath, sectionName) + 1);
		partSec = newSectionName;
		partAfter = allText.substr(getPos(filePath, sectionName) + strLength("[" + sectionName));

	}
	else throw "no section. renameSection func";
	overWrite(filePath, partBefore + partSec + partAfter);
}

std::string IniEdit::getValueFromVar(const std::string& filePath, const std::string& sectionName, const std::string& varName)
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

std::string IniEdit::getValueByIndex(const std::string& filePath, int index)
{
	std::string buffer;

	bool sectionPassed = false;

	int lineNumber = -1;

	std::ifstream in(filePath, std::ios::in);
	if (in.is_open())
	{
		for (int i = 0; i < getLineCount(filePath); i++)
		{
			std::getline(in, buffer);
			if (buffer.find("[") != std::string::npos)
				lineNumber++;

			if (index == lineNumber) return buffer.substr(1, strLength(buffer) - 2);

		}
		in.close();
		throw "Maybe out of range or can't be found. getValueByIndex (for sections)";
	}
	in.close();
	throw "Maybe out of range. getValueByIndex (for sections)";
}

std::string IniEdit::getValueByIndex(const std::string& filePath, const std::string& sectionName, int index)
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

std::string IniEdit::getNextSectionName(const std::string& filePath, const std::string& sectionName)
{
	std::string buffer;
	bool sectionPassed = false;
	int lineNumber = 0;

	std::ifstream in(filePath, std::ios::in);
	if (in.is_open() && checkNextSection(filePath, sectionName))
	{
		while (std::getline(in, buffer))
		{
			if (sectionPassed && buffer.find("[") != std::string::npos) return buffer.substr(1, strLength(buffer) - 2); //-2 because of "[" "]" symbols
			else if (buffer.find("[" + sectionName + "]") != std::string::npos)
			{
				sectionPassed = true;
			}
			lineNumber++;
		}
		in.close();
		return "there is no next section. getNextSectionName function";
	}
	else throw "can't open file or no next section. getNextSectionName function";
}

std::string IniEdit::getAllTextInFile(const std::string& filePath)
{
	std::ifstream in(filePath, std::ios::in);

	std::string buffer = "";
	std::string allText = "";

	if (in.is_open())
	{
		while (std::getline(in, buffer))
			allText += buffer + "\n";
	}
	else throw "can't open file. getAllTextInFile Function";

	in.close();

	return allText;
}

std::string IniEdit::separateString(const std::string& str, std::string_view mode)
{
	if (mode == "after")
		return str.substr(str.find("=") + 1);
	return str.substr(0, str.find("="));
}