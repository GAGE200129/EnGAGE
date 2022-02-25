#include "pch.hpp"
#include "Log.hpp"

namespace FrameworkComponent
{

	std::shared_ptr<spdlog::logger> Log::sEngineLogger = nullptr;
	std::shared_ptr<spdlog::logger> Log::sClientLogger = nullptr;
	void Log::init()
	{
		sEngineLogger = spdlog::stdout_color_mt("EnGAGE");
		sClientLogger = spdlog::stdout_color_mt("Client");

	}
}
