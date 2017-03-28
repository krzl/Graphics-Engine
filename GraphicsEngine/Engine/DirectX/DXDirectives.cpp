#include "stdafx.h"
#include "DXDirectives.h"
#include "Utility\Logger.h"

#if defined(_DEBUG)
bool CheckDXError(HRESULT hr, const std::string& call, const std::string& filename, int line)
{
	if (FAILED(hr))
	{
		const std::string wErr(DXGetErrorString(hr));
		const std::string wErrDesc(DXGetErrorDescription(hr));
		std::string err(wErr.begin(), wErr.end());
		std::string errDesc(wErrDesc.begin(), wErrDesc.end());
		Kz::Logger::Log("DirectX Error: " + call + " in file " + filename + " on line " + 
			std::to_string(line) + ".\nError: " + err + ", Description: " + errDesc + "\n\n", 
			Kz::LogType::Error);
		return true;
	}
	return false;
}

#endif