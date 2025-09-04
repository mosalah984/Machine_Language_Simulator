#include "Register.h"

Register::Register()
{
    registers = gcnew array<uint8_t>(16);

    for (int i = 0; i < 16; i++)
    {
        registers[i] = 0;
    }
}

uint8_t Register::fetch(int index)
{
    if (index < 0 || index >= 16)
    {
        System::Console::Error->WriteLine("Invalid register index: {0}", index);
        return 0;
    }
    else
    {
        return registers[index];
    }
}

void Register::load(int index, uint8_t value)
{
    if (index < 0 || index >= 16)
    {
        System::Console::Error->WriteLine("Invalid register index: {0}", index);
    }
    else
    {
        registers[index] = value;
    }
}

void Register::clear()
{
    for (int i = 0; i < 16; i++)
    {
        registers[i] = 0;
    }
}
