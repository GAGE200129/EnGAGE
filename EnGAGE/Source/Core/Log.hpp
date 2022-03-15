#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Core
{
	namespace Log
	{
		extern std::shared_ptr<spdlog::logger> sLogger;

		void init();
		
		template<typename T, typename... Args>
		inline static void info(T format, Args&&... args)
		{
			sLogger->info(format, args...);
		}

		template<typename T, typename... Args>
		inline static void warn(T format, Args&&... args)
		{
			sLogger->warn(format, args...);
		}

		template<typename T, typename... Args>
		inline static void error(T format, Args&&... args)
		{
			sLogger->error(format, args...);
		}
		
	};


#ifdef EN_DEBUG
#define EN_INFO(...) Core::Log::info(__VA_ARGS__)
#define EN_WARN(...) Core::Log::warn(__VA_ARGS__)
#define EN_ERROR(...) Core::Log::error(__VA_ARGS__)
#else
#define EN_INFO(...) 
#define EN_WARN(...) 
#define EN_ERROR(...)
#endif

}