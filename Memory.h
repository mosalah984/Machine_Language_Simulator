#pragma once

#include <cstdint>

using namespace System;

ref class Memory
{
private:
    array<uint8_t>^ cells;
    static int address;

public:
    Memory();
    uint8_t fetch(int address);
    void load(int address, uint8_t value);
    void clear();
    void setAddress(int value)
    {
        address = value;
    }
    int getAddrrss() 
    {
        return address;
    }
};
