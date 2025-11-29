# MicroCore Machine Language Simulator  
![C++](https://img.shields.io/badge/C++-17-blue.svg)  
![Platform](https://img.shields.io/badge/Platform-Console-lightgrey.svg)  
![Status](https://img.shields.io/badge/Project-Active-brightgreen.svg)

A lightweight **console-based machine language simulator** written in modern C++.  
It emulates a simplified CPU architecture including memory, registers, and a full instruction cycle.  
Ideal for understanding how low-level instructions work inside a CPU.

---

## 🔍 Introduction

MicroCore simulates the internal behavior of a basic CPU:

- Memory  
- Registers  
- Program Counter  
- Fetch → Decode → Execute cycle  

It allows loading machine-code programs, executing them step-by-step, and observing how the CPU modifies memory and registers during execution.

---

## ✨ Features

- **Memory Simulation**  
  Addressable memory cells with read/write support in binary, hex, or integer forms.

- **Register System**  
  Several general-purpose registers automatically updated every instruction.

- **Full Instruction Cycle**  
  - Fetch  
  - Decode  
  - Execute  
  - Update PC  

- **Manual Instruction Input**  
  Enter machine instructions directly from the console.

- **Load External Program Files**  
  Supports loading instructions from plain `.txt` files.

- **Console Interface**  
  Works on any OS with a C++ compiler (Linux, Windows, macOS).

---

## 🧩 Instruction Set

| Opcode | Format | Description |
|--------|--------|-------------|
| `1 RXY` | LOAD | Load register `R` with memory[XY]. |
| `2 RXY` | LOADI | Load register `R` with immediate value `XY`. |
| `3 RXY` | STORE | Store register `R` into memory[XY]. |
| `4 0RS` | MOVE | Copy register `R` into register `S`. |
| `5 RST` | ADD (Int) | Add `S + T` (two's complement) → `R`. |
| `6 RST` | ADD (Float) | Floating-point addition → `R`. |
| `7 RST` | OR | Bitwise OR of registers `S` and `T` → `R`. |
| `8 RST` | AND | Bitwise AND → `R`. |
| `9 RST` | XOR | Bitwise XOR → `R`. |
| `A R0X` | ROTR | Rotate register `R` right by `X`. |
| `B RXY` | JEQZ | Jump to `XY` if register `R == 0`. |
| `C 000` | HALT | Stop execution. |
| `D RXY` | JGZ | Jump to `XY` if register `R` > 0. |

---

## ▶️ Usage

1. Prepare or load a `.txt` file containing machine-code instructions.  
2. Run the simulator.  
3. Execute instructions step-by-step or continuously.  
4. Watch register and memory updates live.  
5. Reset the CPU to load a new program.

---

## 📚 What This Project Taught Me

- CPU internals: memory, registers, PC, cycles  
- Binary/hexadecimal data handling  
- Instruction encoding & decoding  
- Implementing CPU-like behavior in C++  
- Designing modular simulation components

---

## 🚀 Future Enhancements

- Add arithmetic instructions (SUB, MUL, DIV)  
- Add bit shifting (SHL/SHR)  
- Breakpoint system for debugging  
- Execution logging  
- Memory/register dumps  
- Sample machine-language programs included  

---

