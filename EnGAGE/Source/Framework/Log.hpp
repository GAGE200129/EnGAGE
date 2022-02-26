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
		inline static void info(T format, Args&&... args)
		{
#ifdef EN_DEBUG
			sLogger->info(format, args...);
#endif
		}

		template<typename T, typename... Args>
		inline static void warn(T format, Args&&... args)
		{
#ifdef EN_DEBUG
			sLogger->warn(format, args...);
#endif
		}

		template<typename T, typename... Args>
		inline static void error(T format, Args&&... args)
		{
#ifdef EN_DEBUG
			sLogger->error(format, args...);
#endif
		}
	private:
		static std::shared_ptr<spdlog::logger> sLogger;
	};


}