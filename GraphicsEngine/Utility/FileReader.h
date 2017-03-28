#pragma once

#include <fstream>

namespace Kz
{
	class FileReader
	{
	private:

		std::ifstream		m_file;
		bool				m_isFailed;

	public:

		FileReader(const std::string& filePath);
		~FileReader();

		std::string			GetData();
		std::string			GetLine();
		bool				IsFinishedReading();
		bool				IsFailed();
	};
}
