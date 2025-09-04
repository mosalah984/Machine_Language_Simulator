#include "Machine.h"
#include "Memory.h" 
#include "Register.h"
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <msclr\marshal_cppstd.h> // Include for converting System::String^ to std::string
#include <cmath>
#include <iomanip>
#include <bitset>



using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;


struct FloatingPoint
{
    float mantissa;
    int exponent{};

    // Default constructor
    FloatingPoint() : mantissa(0.0f), exponent(0) {}

    FloatingPoint(float value)
    {
        mantissa = frexp(value, &exponent);
    }

    float getValue() const
    {
        return mantissa * Math::Pow(2, exponent);
    }
};

FloatingPoint addFloats(FloatingPoint a, FloatingPoint b)
{
    int expDiff = a.exponent - b.exponent;

    if (expDiff > 0)
        b.mantissa /= Math::Pow(2, expDiff);
    else
        a.mantissa /= Math::Pow(2, -expDiff);

    FloatingPoint result;
    result.mantissa = a.mantissa + b.mantissa;
    result.exponent = a.exponent; // We assume both numbers have the same exponent

    // Normalize the result
    while (Math::Abs(result.mantissa) >= 1.0)
    {
        result.mantissa /= 2.0;
        result.exponent++;
    }

    return result;
}

//###########################

Machine::Machine(Memory^ memory, Register^ registers)
    : memory(memory), registers(registers), programCounter(0), running(true) {}

//##########################

void Machine::loadProgram(const string& filename)
{
    std::string filenameStd = msclr::interop::marshal_as<std::string>(filename);

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;

    int address = memory.getAddrrss();

    while (getline(inputFile, line))
    {
        istringstream ss(line);
        string hexOperand, hexR, hexXY;

        if (ss >> hexOperand >> hexR >> hexXY)
        {
            try
            {
                // Convert the hex strings to integers
                int operand = stoi(hexOperand, nullptr, 16);
                int R = stoi(hexR, nullptr, 16);
                int XY = stoi(hexXY, nullptr, 16);

                memory.load(address, static_cast<uint8_t>((operand << 4) | R)); // Store operand and R in one cell
                memory.load(address + 1, static_cast<uint8_t>(XY)); // Store XY in the next cell

                // Increment the memory address for the next instruction
                address += 2;
                memory.setAddress(address);
            }
            catch (const invalid_argument& e)
            {
                cerr << "Error converting hex to integer: " << e.what() << endl;
                break;  // Exit the loop or perform error handling
            }
        }
    }

    memory.setAddress(address);
}

//#########################

bool Machine::runProgram()
{
    if (!running)
    {
        Console::WriteLine("HALT instruction encountered. Program execution stopped.");
        return false;
    }

    Byte byte1, byte2;
    byte1 = memory->fetch(programCounter);
    byte2 = memory->fetch(programCounter + 1);
    step(byte1, byte2);

    return true;
}



//############################
void Machine::step(uint8_t byte1, uint8_t byte2)
{
    // Decode
    uint8_t opcode, R, XY, S, T;

    opcode = (byte1 >> 4) & 0x0F;  // Bits 4-7 represent operand
    R = byte1 & 0x0F;               // Bits 0-3 represent R

    XY = byte2;

    // Extract S and T from XY
    S = (XY >> 4) & 0x0F;  // Bits 4-7 represent S
    T = XY & 0x0F;         // Bits 0-3 represent T

    // Execute
    switch (opcode)
    {
    case 0x01:  // LOAD R with the bit pattern found in the memory cell whose address is XY.
        registers->load(R, memory->fetch(XY));
        break;
    case 0x02: // LOAD R with the bit pattern XY.
        registers->load(R, XY);
        break;
    case 0x03: // STORE
        if (XY == 0x00)
            // STORE the bit pattern found in register R  to location 00, which is a memory mapping for the screen.
            Console::WriteLine("Screen: {0}", registers->fetch(R));
        else
            // STORE the bit pattern found in register R in the memory cell whose address is XY.
            memory->load(XY, registers->fetch(R));
        break;
    case 0x04: // MOVE the bit pattern found in register R to register S.  0RS -> 0ST means S to T
        registers->load(T, registers->fetch(S));
        break;
    case 0x05: // ADD the bit patterns in registers S and T as two's complement values.
        registers->load(R, registers->fetch(S) + registers->fetch(T));
        break;
    case 0x06: // ADD the bit patterns in registers S and T as two's complement values.
    {
        // Convert binary values to floating-point
        FloatingPoint floatA(registers->fetch(S));
        FloatingPoint floatB(registers->fetch(T));

        // Perform floating-point addition
        FloatingPoint floatResult = addFloats(floatA, floatB);

        // Convert the result back to binary and store it in the destination register
        registers->load(R, floatResult.getValue());
    }
    break;
    case 0x0B: // JUMP to XY, if R0 is equal to R
        if (registers->fetch(R) == registers->fetch(0))
        {
            programCounter = XY;
        }
        break;
    case 0x0C: // HALT execution
        running = false;
        break;
    default:
        Console::Error->WriteLine("Unknown opcode: 0x{0:X}", opcode);
        break;
    }
    programCounter += 2;
}


//############################
void Machine::displayStatus()
{
    // Display register values
    Console::WriteLine("Registers contents:");
    Console::WriteLine("{0,-10}{1,-20}{2,-5}", "Address", "Binary", "Hex");

    for (int i = 0; i < 16; i++)
    {
        Console::Write("{0,-10}", String::Format("{0:X}", i));
        Console::Write("{0,-20}", gcnew String(Convert::ToString(registers->fetch(i), 2)->PadLeft(8, '0')));
        Console::WriteLine("{0,-5}", String::Format("{0:X}", registers->fetch(i)));
    }

    // Display the Program Counter (PC)
    Console::WriteLine("Program Counter (PC): {0:X}", programCounter);

    // Display current instruction
    uint8_t byte1, byte2;
    byte1 = (programCounter == 0) ? 0 : memory->fetch(programCounter - 2);
    byte2 = (programCounter == 0) ? 0 : memory->fetch(programCounter - 1);
    Console::WriteLine("Current Instruction: {0:X}{1:X}", byte1, byte2);
    Console::WriteLine("opcode: {0:X}", (byte1 >> 4) & 0x0F);
    Console::WriteLine("R: {0:X}", byte1 & 0x0F);
    Console::WriteLine("XY: {0:X}", byte2);

    // Display memory contents
    Console::WriteLine("Memory Contents:");
    Console::WriteLine("{0,-10}{1,-20}{2,-5}", "Address", "Binary", "Hex");

    for (int i = 0; i < 256; i++)
    {
        Console::Write("{0,-10}", String::Format("{0:X}", i));
        Console::Write("{0,-20}", gcnew String(Convert::ToString(memory->fetch(i), 2)->PadLeft(8, '0')));
        Console::WriteLine("{0,-5}", String::Format("{0:X}", memory->fetch(i)));
    }
}







