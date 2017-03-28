#pragma once

#include <fstream>
#include <sstream>

namespace Kz
{
	class FileWriter
	{
	private:

		FILE*           m_file;

	public:

		FileWriter(const std::string& filePath);
		~FileWriter();

		void            WriteLine(const std::string& line);
		bool            IsFailed();
	};
}