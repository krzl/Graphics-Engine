#include "FileWriter.h"

namespace Kz
{
	FileWriter::FileWriter(const std::string& filePath)
	{
		fopen_s(&m_file, filePath.c_str(), "w");
	}

	void FileWriter::WriteLine(const std::string& line)
	{
		fputs(line.c_str(), m_file);
		fputs("\n", m_file);
	}

	bool FileWriter::IsFailed()
	{
		return m_file == NULL;
	}

	FileWriter::~FileWriter()
	{
		fclose(m_file);
	}
}