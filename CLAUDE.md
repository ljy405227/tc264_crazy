# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Smart car (智能车) competition firmware for **Infineon TC264D** (TriCore dual-core microcontroller). Based on the **Seekfree (逐飞科技) TC264 Open-source Library** which wraps Infineon's iLLD SDK.

## Build System

- **IDE**: AURIX Development Studio (ADS) v1.10.2 (Eclipse-based)
- **Compiler**: TASKING C/C++ Compiler for TriCore (tc26xb, BGA292 package)
- **Build**: Makefile-based, invoked via Eclipse or CLI `make -C Debug`
- **Debug output**: ELF + HEX + MAP files in `Debug/`
- **Linker script**: `Lcf_Tasking_Tricore_Tc.lsl`
- **Clean**: `删除临时文件.bat` (removes Debug/ and .ads/ directories)

### Key Compile Commands

```
make -C Debug              # Build the project
make -C Debug clean        # Clean build artifacts
```

## Project Structure

```
TC264/
├── user/                      # Entry points and system config
│   ├── cpu0_main.c            # Core 0 entry (main logic loop)
│   ├── cpu1_main.c            # Core 1 entry
│   ├── isr.c / isr.h          # Interrupt service routines
│   └── isr_config.h           # Interrupt priority & service configuration
├── code/                      # Application code (user's algorithm code)
│   ├── Control.c/h            # Speed & direction PID control logic
│   ├── Motor.c/h              # Motor PWM, encoder, GPIO pin definitions
│   ├── PID.c/h                # Position & Increment PID implementations
│   ├── image.c/h              # Camera image processing (binarization, boundary tracing)
│   ├── IMU.c/h                # IMU sensor data processing
│   ├── Filter.c/h             # Signal filtering utilities
│   ├── Init.c/h               # Device/Flash initialization + Flash parameter struct
│   ├── ljy_menu.c/h           # On-screen parameter tuning menu (IPS display)
│   ├── ljy_key.c/h            # Key/button handling
│   ├── ips_show.c/h           # IPS200 display rendering
│   ├── Flash.c/h              # Flash read/write for parameter persistence
│   ├── VOFA.c/h               # VOFA+ serial data plotting protocol
│   ├── My_Math.c/h            # Math helpers
│   └── pid.py                 # Python PID simulation/analysis script
└── libraries/                  # Abstraction layers (Seekfree library)
    ├── infineon_libraries/     # Infineon official iLLD SDK (iLLD, Infra, Service)
    ├── zf_driver/              # MCU peripheral drivers (ADC, PWM, UART, SPI, I2C, DMA, etc.)
    ├── zf_device/              # External device drivers (cameras, IMUs, displays, encoders, etc.)
    └── zf_common/              # Common utilities (debug printf, FIFO, fonts, interrupt helpers, typedefs)
        └── zf_components/      # Seekfree Assistant protocol & printf redirect
```

## Architecture

### Dual-Core Setup
- **CPU0** (`user/cpu0_main.c`): Main control loop — clock/debug init, device init, gyro calibration, then runs the infinite control loop with PID, image processing, display updates, and serial printf.
- **CPU1** (`user/cpu1_main.c`): Secondary core (minimal, currently basic setup).

### Control Flow (CPU0 main loop)
1. `clock_init()` → `debug_init()` → `Device_Init()` — hardware & peripheral setup
2. Gyro calibration (waits for `Gyro_Flag`)
3. Main loop: PID control → image processing → IPS display → serial debug output
4. A menu system (`ljy_menu.c`) allows on-the-fly tuning of PID and speed parameters stored in Flash

### Key Modules
- **Image Processing** (`code/image.c/h`): Camera image binarization, boundary tracing, centerline extraction, gap handling
- **PID Control** (`code/Control.c/h`): Speed loop + direction loop with separate left/right tuning parameters
- **Parameter Storage** (`code/Init.h`): `FlashInformation` struct holds all tunable PID/speed parameters stored to Flash

### Pin Mapping
- Motor PWMs: `ATOM2_CH2_P11_3` (left), `ATOM2_CH0_P13_3` (right), `ATOM2_CH1_P33_5` (aux left), `ATOM2_CH3_P33_7` (aux right)
- Motor DIRs: `P00_8` (left), `P00_9` (right)
- Encoders: `TIM5_ENCODER_CH1_P21_7`/`CH2_P21_6` (left), `TIM3_ENCODER_CH1_P02_6`/`CH2_P02_7` (right)
- Avoid pins: `P14.2-P14.6`, `P10.5-P10.6` (boot pins), `P20.2` (cannot output high), `P21.6` (TC264DA only)

## Important Conventions

- All interrupt priorities **must be unique** (cannot share priority values)
- New code files should be placed directly in the `code/` folder (no subdirectories)
- Conditional compilation flags: `run` (enable motor movement), `mode_fuya` (use brushless motors/ramp-up)
- Debug printf uses Seekfree's `printf` redirect through UART
- IPS200 display shows real-time encoder, yaw, PID output data when not in menu mode
