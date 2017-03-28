#include "Logger.h"
#include "Common.h"

#include <Windows.h>

namespace Kz
{
	Logger::Logger()
	{
		m_vsOutputLogTypes = LOG_TYPE_ALL;
	}

	Logger& Logger::Instance()
	{
		static Logger logger;
		return logger;
	}

	void Logger::CreateLogFile(const std::string& filename, short logTypes)
	{
		Logger inst = Instance();

		auto iterator = inst.m_logFiles.find(filename);
		if (MAP_ELEM_NOT_FOUND(iterator, inst.m_logFiles))
		{
			inst.m_logFiles[filename] = new LogFile(logTypes, filename);
		}
	}

	void Logger::EditLogFile(const std::string& filename, short logTypes)
	{
		Logger inst = Instance();

		auto iterator = inst.m_logFiles.find(filename);
		if (!MAP_ELEM_NOT_FOUND(iterator, inst.m_logFiles))
		{
			inst.m_logFiles[filename]->logTypes = logTypes;
		}
	}

	void Logger::RemoveLogFile(const std::string& filename, short logTypes)
	{
		Logger inst = Instance();

		auto iterator = inst.m_logFiles.find(filename);
		if (!MAP_ELEM_NOT_FOUND(iterator, inst.m_logFiles))
		{
			inst.m_logFiles.erase(iterator);
		}
	}

	void Logger::VSOutputEditLogTypes(short logTypes)
	{
		Logger inst = Instance();

		inst.m_vsOutputLogTypes = logTypes;
	}

	void Logger::Log(const std::string& message, LogType type)
	{
		Logger inst = Instance();

		std::stringstream sstream;
		sstream << message;
		OutputDebugString(sstream.str().c_str());

		for (auto& it : inst.m_logFiles)
		{
			LogFile logFile = *it.second;
			if ((logFile.logTypes & type )> 0)
			{
				logFile.fileWriter.WriteLine(message);
			}
		}
	}
}