#include "system_under_test.h"

int main(int argc, char** argv)
{
    SystemUnderTest sut{ false, 150 };

    sut.DoSomething();

    return 0;
}