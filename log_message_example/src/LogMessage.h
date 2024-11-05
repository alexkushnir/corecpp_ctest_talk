#pragma once

#include <sstream>
#include <limits>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "BaseMessage.h"

constexpr std::size_t MAX_LOG_MESSAGE_LENGTH = 100;
constexpr char VARIABLE_KEY{ '%' };

class RemoteLogMessage : public Message
{
public:
	enum class LogLevel : std::int32_t
	{
		ErrorLevel = 0,
		WarningLevel = 1,
		DebugLevel = 2,
		UNKNOWN = std::numeric_limits<std::int32_t>::max()
	};

	template <class ...Args>
	RemoteLogMessage(const std::int32_t _messageId,
		const LogLevel _severity, const std::string& _format, Args&&... _args)
		: m_messageType{ _severity }
		, m_messageID{ _messageId }
	{
		m_header.extension.messageCode = MessageID::LOG_DATA;
		m_contentHeader.commandType = CommandType::N;
		m_contentHeader.numOfAttributes = 1;
		m_content.subject = MessageSubject::UNKNOWN_SUBJECT;
		m_content.attribute = 0;
		m_content.index = 0;
		m_content.dataType = DataType::BLOCK;

		auto str = Format(_format, std::forward<Args>(_args)...);
		if (str.size() == 0)
		{
			m_numberOfElementsToRead = 0;
		}
		else
		{
			m_numberOfElementsToRead = (str.size() < MAX_LOG_MESSAGE_LENGTH - 1) 
				? static_cast<std::int32_t>(str.size() + 1) 
				: MAX_LOG_MESSAGE_LENGTH;
		}
		
		std::memcpy(m_logData, str.c_str(), m_numberOfElementsToRead);

		SetDataLength(static_cast<std::uint32_t>(sizeof(m_messageType) + 
			sizeof(m_messageID) + sizeof(m_numberOfElementsToRead) + 
			sizeof(m_logData)));
	}

	~RemoteLogMessage() = default;

	std::int32_t GetMessageID() const { return m_messageID; }
	LogLevel GetLogLevel() const { return m_messageType; }
	std::string GetLogText() { 
		return std::string{ reinterpret_cast<char*>(m_logData) }; 
	}
	std::int32_t GetLogMessageLength() const { 
		return m_numberOfElementsToRead; 
	}
	static std::size_t GetMaxLogMessageLength() { 
		return MAX_LOG_MESSAGE_LENGTH; 
	}

private:
	template<class Tuple, std::size_t N>
	struct TuplePrinter
	{
		static void Print(const std::string& _format, std::ostream& _os, 
			const Tuple& _t)
		{
			const size_t idx = _format.find_last_of(VARIABLE_KEY);
			TuplePrinter<Tuple, N - 1>::Print(
				std::string(_format, 0, idx), _os, _t);
			_os << std::get<N - 1>(_t) << std::string(_format, idx + 1);
		}
	};

	template<class Tuple>
	struct TuplePrinter<Tuple, 1>
	{
		static void Print(const std::string& _format, std::ostream& _os, 
			const Tuple& _t)
		{
			const size_t idx = _format.find_first_of(VARIABLE_KEY);
			_os << std::string(_format, 0, idx) << std::get<0>(_t) << 
				std::string(_format, idx + 1);
		}
	};

	template<class... Args>
	std::string Format(const std::string& _format, Args&&... _args)
	{
		std::stringstream ss;

		if constexpr (sizeof...(_args) == 0)
		{
			ss << _format;
		}
		else
		{
			if (IsFormatValid(_format, sizeof...(_args)))
			{
				if (_format.find_last_of(VARIABLE_KEY) != _format.npos)
				{
					const auto t = std::make_tuple(std::forward<Args>(_args)...);
					TuplePrinter<decltype(t), sizeof...(Args)>::Print(_format, 
						ss, t);
				}
			}
		}

		return ss.str();
	}

	bool IsFormatValid(const std::string& _format, int _numberOfArguments)
	{
		return(std::count(_format.begin(), _format.end(), VARIABLE_KEY) == 
			_numberOfArguments);
	}

	LogLevel m_messageType{ LogLevel::UNKNOWN };
	std::int32_t m_messageID{ 0 };
	std::int32_t m_numberOfElementsToRead{ 0 };
	std::uint8_t m_logData[MAX_LOG_MESSAGE_LENGTH]{ 0 };
};
