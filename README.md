# 🖩 UART-Based Calculator on LPC2129

A **bare-metal embedded C project** that implements a real-time arithmetic calculator on the **LPC2129 (ARM7TDMI)** microcontroller. The calculator receives mathematical expressions via **UART serial communication**, evaluates them, and transmits the result back — all without any operating system, HAL, or middleware.

---

## 📁 Repository Structure

```
UART-Calculator/
│
├── calc.c              # Main application — expression parser & arithmetic logic
├── UART0_Driver.c      # Custom bare-metal UART0 peripheral driver
├── header.h            # Shared typedefs, macros & extern function declarations
│
├── Calculator.uvproj   # Keil uVision project file (open this to load the project)
├── Calculator.sct      # ARM linker scatter file (memory map configuration)
└── Calculator.hex      # Compiled firmware — ready to flash onto LPC2129
```

---

## ⚙️ How It Works

```
  [PC Terminal]                         [LPC2129]
       │                                     │
       │──── Send: "12+34*2" + Enter ───────▶│
       │                                     │  Parse input char-by-char via UART RX
       │                                     │  Evaluate: 12 + 34 * 2
       │◀─── Receive: Result ───────────────│
       │                                     │  Transmit result via UART TX
```

1. User types an arithmetic expression in a serial terminal (e.g. PuTTY / Tera Term)
2. LPC2129 reads characters one-by-one over **UART0**
3. On pressing **Enter (CR = 0x0D)**, the expression is evaluated
4. The result is sent back over UART and displayed in the terminal

---

## ✨ Features

- ➕ Supports **5 arithmetic operators**: `+`, `-`, `*`, `/`, `%`
- 🔢 Handles **multi-operand expressions** (e.g. `10+20-5*2`)
- 🛡️ **Overflow detection** using `LLONG_MAX` boundary checks
- ⚠️ **Divide-by-zero** error handling with user-friendly UART message
- 📡 **Custom UART0 driver** — baud rate config, TX, RX, string & integer output
- 🔁 **Continuous loop** — ready for next input immediately after result
- 🧱 Fully **bare-metal** — no RTOS, no HAL, no middleware

---

## 🛠️ Tech Stack

| Component | Details |
|---|---|
| **Microcontroller** | NXP LPC2129 (ARM7TDMI-S core) |
| **Language** | Embedded C |
| **IDE** | Keil uVision (ARM MDK) |
| **Communication** | UART0 — 9600 baud, 8N1 |
| **Driver** | Custom bare-metal UART0 driver |
| **Toolchain** | ARM-GCC / Keil Compiler, .hex flashing |

---

## 📄 File Descriptions

### `calc.c` — Main Application
The core logic file. Handles:
- Character-by-character UART input processing
- Number parsing and multi-digit integer accumulation
- Operator detection and expression storage
- Arithmetic evaluation loop with safety checks
- Result output via UART

```c
// Reads each character from UART and builds the expression
t = UART0_RX();
if(t >= '0' && t <= '9')           // Accumulate digits
    nn = (nn * 10) + (t - '0');
else if(t == '+' || t == '-' ...)  // Store operator
    num[ni++] = nn; op[oi++] = t;
else if(t == 13)                   // Enter key → evaluate & transmit result
```

---

### `UART0_Driver.c` — Custom Peripheral Driver
Written entirely from scratch using **direct register access** on the LPC2129. No HAL or library used.

| Function | Description |
|---|---|
| `UART0_INIT(baud)` | Configures UART0: sets PINSEL0, calculates & loads baud divisors into U0DLL/U0DLM |
| `UART0_TX(data)` | Transmits a single byte; polls THRE flag in U0LSR before sending |
| `UART0_RX()` | Receives a byte; polls RDR flag in U0LSR until data is available |
| `UART_str(ptr)` | Sends a null-terminated string byte-by-byte |
| `UART0_int(num)` | Converts integer to ASCII and transmits digit-by-digit |
| `UART0_float(num)` | Splits float into integer + fractional part and transmits |

**Baud Rate Configuration (Register-Level):**
```c
PCLK = a[VPBDIV] * 1000000;   // Determine peripheral clock
result = PCLK / (16 * baud);   // Calculate divisor
U0DLL = result & 0xFF;         // Load lower byte
U0DLM = (result >> 8) & 0xFF;  // Load upper byte
```

---

### `header.h` — Shared Header
Contains:
- LPC2129 register definitions via `#include <lpc21xx.h>`
- **MISRA-C aligned typedefs** for portable type safety:

```c
typedef unsigned char       u8;
typedef unsigned short int  u16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef signed short int    s16;
typedef signed char         s8;
```
- `extern` declarations for all UART driver functions

---

## 🚀 Getting Started

### Hardware Required
- LPC2129 Development Board
- USB-to-UART (TTL) Module (e.g., CP2102 / CH340)
- Keil uVision installed on PC
- Flash tool (Flash Magic or Keil's built-in flasher)

### Setup Steps

**1. Clone the Repository**
```bash
git clone https://github.com/Gopi-Shankar-V/UART-Calculator.git
cd UART-Calculator
```

**2. Open in Keil uVision**
- Launch Keil uVision
- Open `Calculator.uvproj`
- Build the project (`F7`) — generates `Calculator.hex`

**3. Flash to LPC2129**
- Open **Flash Magic**
- Select COM port, baud rate `9600`, device `LPC2129`
- Load `Calculator.hex` and click **Start**

**4. Open Serial Terminal**
- Open PuTTY or Tera Term
- Set: **COM port**, **Baud: 9600**, **8N1**
- Type expressions like `25+75` and press **Enter**

---

## 💻 Usage Examples

```
Input  →  12+8          Output  →  20
Input  →  100-45+5      Output  →  60
Input  →  6*7           Output  →  42
Input  →  20/4          Output  →  5
Input  →  17%5          Output  →  2
Input  →  10/0          Output  →  Divide By Zero!
Input  →  99999999999+1 Output  →  Buffer Overflow!
```

---

## 🧠 Key Concepts Demonstrated

- **Register-Level Peripheral Programming** — direct UART0 register manipulation (PINSEL0, U0LCR, U0DLL, U0DLM, U0LSR, U0THR, U0RBR)
- **Bare-Metal Firmware Architecture** — no OS, no HAL, pure hardware control
- **Custom Driver Development** — reusable UART driver with clean API
- **Fault-Tolerant Design** — overflow and divide-by-zero safety guards
- **MISRA-C Coding Practices** — portable typedefs, modular file structure
- **ARM Toolchain Workflow** — Keil uVision build → .hex → Flash Magic deploy

---

## 👤 Author

**Gopi Shankar V**
GitHub: [@Gopi-Shankar-V](https://github.com/Gopi-Shankar-V)

---

## 📄 License

This project is open source and available for educational and reference purposes.
