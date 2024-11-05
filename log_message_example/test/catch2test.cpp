#include "catch2/catch_test_macros.hpp"

#include "LogMessage.h"

class RemoteMessageTest
{
protected:
	template<class ...Args>
	RemoteLogMessage CreateLogMessage(const std::string& _format, 
		Args&&... _args)
	{
		return RemoteLogMessage{ m_messageId, m_logLevel, _format, 
			std::forward<Args>(_args)... };
	}

	void VerifyMetaData(const RemoteLogMessage& _message)
	{
		CHECK(_message.GetMessageID() == m_messageId);
		CHECK(_message.GetLogLevel() == m_logLevel);
	}

private:
	static constexpr std::int32_t m_messageId{ 10 };
	static constexpr RemoteLogMessage::LogLevel m_logLevel{ 
		RemoteLogMessage::LogLevel::DebugLevel };
};

TEST_CASE_METHOD(RemoteMessageTest, "catch2_NoKeyForArgs", "[RemoteMsg]")
{
	std::tuple<std::int32_t, std::string, char> testTuple = 
		std::make_tuple(123, "A string", 'x');
	std::string format{ "A format without percents" };
	auto rlm = CreateLogMessage(format, std::get<0>(testTuple), 
		std::get<1>(testTuple), std::get<2>(testTuple));
	CHECK(std::string{ rlm.GetLogText() } == std::string{});
}

TEST_CASE_METHOD(RemoteMessageTest, "catch2_IncompatibleKeys", "[RemoteMsg]")
{
	// More arguments than keys
	std::tuple<std::int32_t, std::string, char> testTuple = 
		std::make_tuple(123, "A string", 'x');
	{
		std::string format{ "A format % with 2 % percents and 3 args" };
		auto rlm = CreateLogMessage(format, std::get<0>(testTuple), 
			std::get<1>(testTuple), std::get<2>(testTuple));
		CHECK(std::string{ rlm.GetLogText() } == std::string{});
	}
	// More keys than arguments
	{
		std::string format{ "A format % with 4 % percents and % 3 args %" };
		auto rlm = CreateLogMessage(format, std::get<0>(testTuple), 
		std::get<1>(testTuple), std::get<2>(testTuple));
		CHECK(std::string{ rlm.GetLogText() } == std::string{});
	}
}
