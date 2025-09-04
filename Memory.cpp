#include "Memory.h"

Memory::Memory()
{
    cells = gcnew array<uint8_t>(256);

    for (int i = 0; i < 256; i++)
    {
        cells[i] = 0;
    }

    Memory::setAddress(0);
}

uint8_t Memory::fetch(int address)
{
    if (address >= 0 && address < 256)
    {
        return cells[address];
    }
    else
    {
        System::Console::Error->WriteLine("Invalid memory address: {0}", address);
        return 0;
    }
}

void Memory::load(int address, uint8_t value)
{
    if (address >= 0 && address < 256)
    {
        cells[address] = value;
    }
    else
    {
        System::Console::Error->WriteLine("Invalid memory address: {0}", address);
    }
}

void Memory::clear()
{
    for (int i = 0; i < 256; i++)
    {
        cells[i] = 0;
    }

    Memory::setAddress(0);
}
