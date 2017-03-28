#include "FileReader.h"

#include <sstream>

namespace Kz
{
	FileReader::FileReader(const std::string& filePath) : 
		m_file(std::ifstream(filePath))
	{
		m_isFailed = !m_file.is_open();
	}

	std::string FileReader::GetData()
	{
		std::stringstream sstream;
		sstream << m_file.rdbuf();
		std::string ret = sstream.str();
		return ret;
	}

	std::string FileReader::GetLine()
	{
		std::string line;
		while (line.length() != 0)
		{
			std::getline(m_file, line);
		}
		return line;
	}

	bool FileReader::IsFinishedReading()
	{
		return !m_file;
	}

	bool FileReader::IsFailed()
	{
		return m_isFailed;
	}

	FileReader::~FileReader()
	{
		m_file.close();
	}
}