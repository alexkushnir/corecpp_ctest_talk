#pragma once 

#include <cstdint>
#include <limits>

using MessageAttribute = std::uint8_t;

enum class MessageID : std::uint16_t
{
    LOG_DATA = 100
};

enum class DataType : uint8_t
{
    BYTE    = 0,
    SHORT   = 1,
    LONG    = 2,
    BLOCK   = 3,
    FLOAT   = 4,
    UNKNOWN = std::numeric_limits<std::uint8_t>::max()
};

enum class CommandType : uint16_t
{
    G = 0,  
    S = 1,  
    R = 2,  
    N = 3,  
    UNKNOWN = std::numeric_limits<std::uint16_t>::max()
};

enum class MessageSubject : std::uint8_t
{
	UNKNOWN_SUBJECT = std::numeric_limits<std::uint8_t>::max()	    
};

struct MessageExtension
{
    MessageID  messageCode;
    std::uint8_t control{ 0 };
    std::uint8_t version{ 1 };
};

struct MessageHeader
{
    std::uint32_t 	 magicNumber1{ 0x12345678 };
    std::uint32_t  	 magicNumber2{ 0x87654321 };
    MessageExtension extension;
    std::uint32_t 	 messageLength; 
    std::uint32_t	 messageUniqueId;
};

struct MessageContentHeader
{
    CommandType    commandType;
    std::uint16_t  numOfAttributes;
};

struct MessageContent
{
    MessageSubject 		subject;
    MessageAttribute   	attribute;
    std::uint8_t   		index;
    DataType       		dataType;   
    std::uint32_t  		dataLength { 0 }; 
};

class Message
{
public:
    Message() = default;
    
    explicit Message(MessageID _id)
    {
        m_header.extension.messageCode = _id;
    }	

    ~Message() = default;

    void SetDataLength(const uint32_t _dataLength)
    {
        m_header.messageLength = GetMetaDataLengthWithoutHeader() + _dataLength;
        m_content.dataLength = _dataLength;
    }

    uint32_t GetTotalMessageLength()
    {
        return GetMetaDataLength() + m_content.dataLength;
    }

    void SetMessageUniqueID(const uint32_t _uniqueId)
    {
        m_header.messageUniqueId = _uniqueId;
    }

    uint32_t GetMessageUniqueID() const
    {
        return m_header.messageUniqueId;
    }

    MessageID GetMessageID() const
    {
        return m_header.extension.messageCode;
    }

    MessageSubject GetSubject() const { return m_content.subject; }
    MessageAttribute GetAttribute() const { return m_content.attribute; }
    void SetAttribute(const MessageAttribute _attribute) {

        m_content.attribute = _attribute; 
    }
    CommandType GetCommand() const { return m_contentHeader.commandType; }
    void SetCommand(const CommandType _command) 
    { 
        m_contentHeader.commandType = _command; 
    }

protected:
    MessageHeader 		 m_header{};
    MessageContentHeader m_contentHeader{};
    MessageContent		 m_content{};

private:
    std::uint32_t GetMetaDataLength() { return sizeof(Message);}
    std::uint32_t GetMetaDataLengthWithoutHeader() 
    { 
        return sizeof(Message) - sizeof(MessageHeader);
    }
};


