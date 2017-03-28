#pragma once

#include <map>

#include "FileWriter.h"

namespace Kz
{
	#define LOG_TYPE_NONE 0
	#define LOG_TYPE_ALL 31

	enum LogType
	{
		Info = 1,
		Debug = 2,
		Warning = 4,
		Error = 8,
		Assert = 16
	};

	struct LogFile
	{
		short		logTypes;
		FileWriter	fileWriter;

		LogFile(short types, const std::string& filename) :
			logTypes(types), 
			fileWriter(FileWriter(filename))
		{}
	};

	class Logger
	{
	private:
	
		short m_vsOutputLogTypes;
		std::map<std::string, LogFile*> m_logFiles;

		Logger();

		static Logger& Instance();

	public:

		static void CreateLogFile(const std::string& filename, short logTypes);
		static void EditLogFile  (const std::string& filename, short logTypes);
		static void RemoveLogFile(const std::string& filename, short logTypes);

		static void VSOutputEditLogTypes(short logTypes);

		static void Log(const std::string& message, LogType type);
	};
}