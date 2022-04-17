#include "pch.hpp"
#include "Log.hpp"

#include <spdlog/sinks/base_sink.h>


namespace Core
{
	namespace Log
	{
		std::shared_ptr<spdlog::logger> sLogger = nullptr;
        static constexpr unsigned int MAX_LINES = 300;
        static std::vector<std::string> gStringBuffer;

        template<typename Mutex>
        class CustomSink : public spdlog::sinks::base_sink <Mutex>
        {

        protected:
            void sink_it_(const spdlog::details::log_msg& msg) override
            {

                // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
                // msg.raw contains pre formatted log

                // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
                spdlog::memory_buf_t formatted;
                spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
                gStringBuffer.push_back(fmt::to_string(formatted));

                if (gStringBuffer.size() > MAX_LINES)
                {
                    gStringBuffer.erase(gStringBuffer.begin());
                }
            }

            void flush_() override
            {
                gStringBuffer.clear();
            }
        };

		void Log::init()
		{
			sLogger = spdlog::stdout_color_mt("EnGAGE");
            spdlog::get("EnGAGE")->sinks().emplace_back(std::make_shared<CustomSink<std::mutex>>());

			spdlog::flush_every(std::chrono::seconds(30));
		}
        const std::vector<std::string>& getStringLines()
        {
            return gStringBuffer;
        }
	}
}
