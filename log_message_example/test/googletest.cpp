#include <string>

#include "LogMessage.h"
#include "gtest/gtest.h"

class RemoteMessageTest : public ::testing::Test
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
		ASSERT_EQ(_message.GetMessageID(), m_messageId);
		ASSERT_EQ(_message.GetLogLevel(), m_logLevel);
	}

	static constexpr std::int32_t m_messageId{ 10 };
	static constexpr RemoteLogMessage::LogLevel m_logLevel{ 
		RemoteLogMessage::LogLevel::DebugLevel };
};

TEST_F(RemoteMessageTest, gtest_TextTruncation)
{
	// 104 Symbols
	const std::string testString{ 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz" };
	const std::string expectedResult{ testString.begin(), 
		testString.begin() + 
		RemoteLogMessage::GetMaxLogMessageLength() };

	auto rlm = CreateLogMessage(testString);
	VerifyMetaData(rlm);

	ASSERT_EQ(rlm.GetLogMessageLength(), expectedResult.size());
}

TEST_F(RemoteMessageTest, gtest_VariadicArguments)
{
	std::tuple<std::int32_t, std::string, char> testTuple = 
		std::make_tuple(123, "A string", 'x');
	std::stringstream expectedResultStream;
	expectedResultStream << "A " << std::get<0>(testTuple) << 
		" variadic " <<  std::get<1>(testTuple) << " message " << 
		std::get<2>(testTuple) <<  " for test";

	auto rlm = CreateLogMessage("A % variadic % message % for test", 
		std::get<0>(testTuple), std::get<1>(testTuple), 
		std::get<2>(testTuple));
	
	VerifyMetaData(rlm);

	ASSERT_EQ(expectedResultStream.str(), rlm.GetLogText());
}

