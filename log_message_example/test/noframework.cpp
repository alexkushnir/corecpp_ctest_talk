#include "LogMessage.h"

#include "cxxopts.hpp"

#include <string>

static constexpr std::int32_t g_messageId{ 10 };
static constexpr RemoteLogMessage::LogLevel g_logLevel{ 
    RemoteLogMessage::LogLevel::DebugLevel };

template<class ...Args>
static RemoteLogMessage CreateLogMessage(const std::string& _format, 
    Args&&... _args)
{
    return RemoteLogMessage{ g_messageId, g_logLevel, _format, 
        std::forward<Args>(_args)... };
}

static void VerifyMetaData(const RemoteLogMessage& _message)
{
    if (_message.GetMessageID() != g_messageId || 
        _message.GetLogLevel() != g_logLevel)
    {
        std::exit(1);
    }
}

static void RemoteMessageTest_NoArguments()
{
	const std::string testString{ 
        "This is a test message without arguments" };
		
	auto rlm = CreateLogMessage(testString);
	VerifyMetaData(rlm);

	if (rlm.GetLogText() != testString)
    {
        std::exit(1);
    }

}

static void RemoteMessageTest_EmptyMessage()
{
	const std::string testString{ "" };

	auto rlm = CreateLogMessage(testString);
	VerifyMetaData(rlm);

	if (rlm.GetLogMessageLength() != testString.size() || 
        rlm.GetLogText() != testString)
    {
        std::exit(1);
    }
}

static auto GetParsedOptions(int argc, char** argv)
{
    cxxopts::Options options("no-frameworks-test", 
        "CTest unit tests with no framework");
    options.add_options()
    ("n, no-arguments", "Run RemoteMessageTest_NoArguments test") 
    ("e, empty-message", "Run RemoteMessageTest_EmptyMessage test")
    ;
    
    return options.parse(argc, argv);
}

int main(int argc, char** argv)
{
    auto options = GetParsedOptions(argc, argv);
    if (options.count("n"))
    {
        RemoteMessageTest_NoArguments();
    }
    else if (options.count("e"))
    {
        RemoteMessageTest_EmptyMessage();
    }
    
    return 0;
}
