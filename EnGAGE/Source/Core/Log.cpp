#include "pch.hpp"
#include "Log.hpp"

namespace Core
{
	namespace Log
	{
		std::shared_ptr<spdlog::logger> sLogger = nullptr;

		void Log::init()
		{
			sLogger = spdlog::stdout_color_mt("EnGAGE");
		}
	}
}
