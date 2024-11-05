#pragma once 

#include <vector>
#include <string>
#include <cstdint>

class SystemUnderTest
{
public:
    explicit SystemUnderTest(const bool bool_flag, 
        const std::size_t container_size);
    
    auto& GetContainer() const { return m_container; }
    auto& GetFlagValue() const { return m_flag_value; }

    std::int32_t DoSomething(); 
    
private:
    std::vector<std::int32_t> m_container;
    std::string m_flag_value;
};