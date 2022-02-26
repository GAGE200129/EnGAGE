#include "pch.hpp"
#include "Log.hpp"

namespace FrameworkComponent
{

	std::shared_ptr<spdlog::logger> Log::sLogger = nullptr;
	void Log::init()
	{
		sLogger = spdlog::stdout_color_mt("EnGAGE");
	}
}
