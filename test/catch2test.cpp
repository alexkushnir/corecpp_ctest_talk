#include "catch2/catch_test_macros.hpp"

#include "LogMessage.h"

class RemoteMessageTest
{
protected:
	template<class ...Args>
	RemoteLogMessage CreateLogMessage(const std::string& _format, Args&&... _args)
	{
		return RemoteLogMessage{ m_messageId, m_logLevel, _format, std::forward<Args>(_args)... };
	}

	void VerifyMetaData(const RemoteLogMessage& _message)
	{
		CHECK(_message.GetMessageID() == m_messageId);
		CHECK(_message.GetLogLevel() == m_logLevel);
	}

private:
	static constexpr std::int32_t m_messageId{ 10 };
	static constexpr RemoteLogMessage::LogLevel m_logLevel{ RemoteLogMessage::LogLevel::DebugLevel };
};

TEST_CASE_METHOD(RemoteMessageTest, "EmptyMessage", "RemoteMessageTest")
{
	const std::string testString{ "" };

	auto rlm = CreateLogMessage(testString);
	VerifyMetaData(rlm);

	CHECK(rlm.GetLogMessageLength() == testString.size());
	CHECK(rlm.GetLogText() == testString);
}

TEST_CASE_METHOD(RemoteMessageTest, "TextTruncation", "RemoteMessageTest")
{
	// 104 Symbols
	const std::string testString{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
	const std::string expectedResult{ testString.begin(), testString.begin() + RemoteLogMessage::GetMaxLogMessageLength() };

	auto rlm = CreateLogMessage(testString);
	VerifyMetaData(rlm);

	CHECK(rlm.GetLogMessageLength() == expectedResult.size());
}

TEST_CASE_METHOD(RemoteMessageTest, "VariadicArguments", "RemoteMessageTest")
{
	std::tuple<std::int32_t, std::string, char> testTuple = std::make_tuple(123, "A string", 'x');
	std::stringstream expectedResultStream;
	expectedResultStream << "A " << std::get<0>(testTuple) << " variadic " << std::get<1>(testTuple) << " message " << std::get<2>(testTuple) << " for test";

	auto rlm = CreateLogMessage("A % variadic % message % for test", std::get<0>(testTuple), std::get<1>(testTuple), std::get<2>(testTuple));
	VerifyMetaData(rlm);

	CHECK(expectedResultStream.str() == rlm.GetLogText());
}

TEST_CASE_METHOD(RemoteMessageTest, "NoKeyForArguments", "RemoteMessageTest")
{
	std::tuple<std::int32_t, std::string, char> testTuple = std::make_tuple(123, "A string", 'x');
	std::string format{ "A format without percents" };
	auto rlm = CreateLogMessage(format, std::get<0>(testTuple), std::get<1>(testTuple), std::get<2>(testTuple));
	CHECK(std::string{ rlm.GetLogText() } == std::string{});
}

TEST_CASE_METHOD(RemoteMessageTest, "IncompatibleKeys", "RemoteMessageTest")
{
	// More arguments than keys
	std::tuple<std::int32_t, std::string, char> testTuple = std::make_tuple(123, "A string", 'x');
	{
		std::string format{ "A format % with 2 % percents and 3 args" };
		auto rlm = CreateLogMessage(format, std::get<0>(testTuple), std::get<1>(testTuple), std::get<2>(testTuple));
		CHECK(std::string{ rlm.GetLogText() } == std::string{});
	}
	// More keys than arguments
	{
		std::string format{ "A format % with 4 % percents and % 3 args %" };
		auto rlm = CreateLogMessage(format, std::get<0>(testTuple), std::get<1>(testTuple), std::get<2>(testTuple));
		CHECK(std::string{ rlm.GetLogText() } == std::string{});
	}
}
