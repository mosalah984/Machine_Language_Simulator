// Machine.h
#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <tuple>
#include "Register.h"
#include "Memory.h"

using namespace System;

ref class Machine
{
private:
    Memory^ memory;
    Register^ registers;
    int programCounter;
    bool running;

public:
    Machine(Memory^ memory, Register^ registers);
    void loadProgram(String^ filename);
    bool runProgram();
    void step(uint8_t byte1, uint8_t byte2);
    void displayStatus();
};
