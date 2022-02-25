#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FrameworkComponent
{
	class Log
	{
	public:
		static void init();
		
		template<typename T, typename... Args>
		inline static void engineInfo(T format, Args&&... args)
		{
			sEngineLogger->info(format, args...);
		}

	private:
		static std::shared_ptr<spdlog::logger> sEngineLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};


}