#include "system_under_test.h"

SystemUnderTest::SystemUnderTest(const bool bool_flag, const std::size_t container_size)
    : m_container(container_size)
{
    auto idx = 0;
    if (bool_flag)
    {
        m_flag_value = "true";
    
        for (auto& entry : m_container)
        {
            entry = idx++;
        }
    }

    else
    {
        m_flag_value = "false";
    
        for (auto& entry : m_container)
        {
            entry = (idx++ + 42);
        }
    }
}

std::int32_t SystemUnderTest::DoSomething()
{
    auto val = 0;

    if (m_flag_value == "false")
    {
        m_container.at(3) = 555;
        val = 13;
    }

    if (m_container.at(2) == 55)
    {
        m_container.at(2) = 42;
        val = 42;
    }
   
    return val;
}