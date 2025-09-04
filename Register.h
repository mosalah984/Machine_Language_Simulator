#pragma once

#include <cstdint>

using namespace System;

ref class Register
{
private:
    array<uint8_t>^ registers;

public:
    Register();
    uint8_t fetch(int index);
    void load(int index, uint8_t value);
    void clear();
};
