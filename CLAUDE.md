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

## Conditional Compilation Flags

Defined in [libraries/zf_common/zf_common_headfile.h](libraries/zf_common/zf_common_headfile.h#L6-L8) — these are the primary build-time configuration switches:

| Flag | Purpose |
|------|---------|
| `run` (1/0) | **Master enable**: 1 = motors run, 0 = motors disabled (safe debug mode) |
| `mode_fuya` (1/0) | **Motor type**: 0 = brushless DC (无刷) with ramp-up, 1 = brushed DC (有刷) |
| `Battery_Valtage_mode` (1/0) | Battery voltage monitoring toggle |
| `Wifi_mode` (1/0) | WiFi module enable |

**Critical**: `mode_fuya` changes pin mappings, encoder channels, and PWM assignments in [Motor.h](code/Motor.h). When `mode_fuya=0` (brushless), an auxiliary brushless PWM (`fuya_wushua_pwm`, `ATOM0_CH0_P21_2`) is used for propulsion; brushed motors use `fuya_left_pwm` (`ATOM2_CH1_P33_5`). Motor direction pins and encoder assignments also swap between the two modes.

There is also a local `#define guandao 0` in [Control.c](code/Control.c#L320) that when set to 1 disables the turn state machine (inertial navigation bypass mode).

## Project Structure

```
TC264/
├── user/                      # Entry points and system config
│   ├── cpu0_main.c            # Core 0 entry (main control loop)
│   ├── cpu1_main.c            # Core 1 entry (camera image processing)
│   ├── isr.c / isr.h          # Interrupt service routines
│   └── isr_config.h           # Interrupt priority & service configuration
├── code/                      # Application code (user algorithm code)
│   ├── Control.c/h            # Speed & direction PID cascaded control + ISR handler
│   ├── Motor.c/h              # Motor PWM, encoder, GPIO pin definitions
│   ├── PID.c/h                # Position & Incremental PID implementations
│   ├── image.c/h              # Camera image processing pipeline
│   ├── IMU.c/h                # IMU sensor data processing (gyro/accel/yaw)
│   ├── Filter.c/h             # Signal filtering (LPF, IIR, Kalman)
│   ├── Init.c/h               # Device/Flash initialization + FlashInformation struct
│   ├── ljy_menu.c/h           # 3-level on-screen parameter tuning menu (IPS display)
│   ├── ljy_key.c/h            # 4-button + 5 DIP switch input handling
│   ├── ips_show.c/h           # IPS200 display rendering (debug visualization)
│   ├── Flash.c/h              # Flash read/write for parameter persistence
│   ├── VOFA.c/h               # VOFA+ serial data plotting protocol
│   ├── My_Math.c/h            # Math helpers (distance, slope, coordinate transforms)
│   └── pid.py                 # Python PID simulation/analysis script
└── libraries/                  # Abstraction layers (Seekfree library, GPLv3)
    ├── infineon_libraries/     # Infineon official iLLD SDK (iLLD, Infra, Service)
    ├── zf_driver/              # MCU peripheral drivers (ADC, PWM, UART, SPI, I2C, DMA, PIT, etc.)
    ├── zf_device/              # External device drivers (MT9V03X camera, ICM20602/IMU660RA IMU, IPS200 display,
    │                             encoders, wireless UART, ToF, GNSS, etc.)
    └── zf_common/              # Common utilities (debug printf, FIFO, fonts, interrupt helpers, typedefs)
        └── zf_components/      # Seekfree Assistant protocol & printf redirect
```

## Architecture

### Dual-Core Setup

- **CPU0** ([user/cpu0_main.c](user/cpu0_main.c)): System init → gyro calibration → infinite loop: menu handling, IPS display updates, and PID control (triggered by `pit_control_flag` set in ISR). Handles motor PWM output, encoder reading, and the speed/direction cascaded PID.
- **CPU1** ([user/cpu1_main.c](user/cpu1_main.c)): Dedicated to camera image processing. Waits for `mt9v03x_finish_flag` (set by camera DMA interrupt), then calls `ljw_camera_deal()` — the full image processing pipeline. Sets `data_ready_flag_for_core0` for cross-core synchronization.

Both cores use `#pragma section all "cpu0_dsram"` / `"cpu1_dsram"` to place code in core-specific RAM sections.

### Interrupt Architecture (PIT Heartbeat)

The system is timer-driven via PIT (Periodic Interrupt Timer) on CCU6 channels:

| PIT Channel | Period | Handler | Purpose |
|-------------|--------|---------|---------|
| CCU60_CH0 | 10ms | `cc60_pit_ch0_isr` | Key scanning (`key_scan()`), increments `pit_count_ms` |
| CCU60_CH1 | **1ms** | `cc60_pit_ch1_isr` | **Main control ISR**: gyro processing, IMU update, then `ljy_isr_headle()` (PID + motor output) when `pit_control_flag==1` |
| CCU61_CH0/CH1 | — | (empty) | Reserved |

Other interrupts: external GPIO (camera VSYNC on ERU_CH3, ToF on ERU_CH1), DMA (camera data transfer on channel 5), UART (4× UART for debug, camera config, wireless, GNSS).

**All interrupt priorities must be unique** (range 1-255, higher = higher priority). DMA-assigned interrupts are limited to priorities 0-47.

### Control System (Cascaded PID)

The control loop runs every 1ms inside `ljy_isr_headle()` in [Control.c](code/Control.c#L323):

1. **Speed Ramp** (`SpeedRamp_Update`): S-curve acceleration profile from 0 to `speed_target_max` over `ramp.ramp_time` seconds
2. **Outer Position Loop** (`PID_Direction_Control` position PD): Camera centerline deviation `Final_Sum` → position PD → `expect_gyro` (target angular velocity). Limited to ±2000.
3. **Inner Angular Velocity Loop** (`PID_Direction_Control` gyro PD): `expect_gyro` vs `avl_gyro_z` (gyro Z) → gyro PD → `speed_dis_out` (differential speed compensation)
4. **Speed Loop** (`PID_Speed_Control` incremental PI): Encoder-derived average speed vs `speed_target` → PI → base PWM
5. **Motor Output**: `Left PWM = speed_pid.out - speed_dis_out`, `Right PWM = speed_pid.out + speed_dis_out`

**Three separate PID parameter sets** are maintained for straight, turn-left, and turn-right — each with its own position PD and gyro PD gains. The `task_point` enum (`TASK_STRAIGHT`/`TASK_TURN_LEFT`/`TASK_TURN_RIGHT`) selects which parameter set is active.

### Turn Detection State Machine

In [Control.c](code/Control.c#L326-L408), controlled by `turning_state`, `end_turning_state`, `zuo_ing`, `you_ing`:

1. When `turning_state == 1` (set by image processing), records `yaw_turn_first` (current yaw), sets `zuo_ing = 1`, increments `turn_count`, zeroes all PID states
2. While `zuo_ing == 1`, monitors `angle_diff(yaw, yaw_turn_first)` — when ≥ 62°, sets `end_turning_state = 1` and zeroes PID states again
3. `turn_count` tracks total turns completed; run stops when `turn_count >= 30`

### Image Processing Pipeline

In [image.c](code/image.c), called from CPU1 as `ljw_camera_deal()`:

1. **Compress** (`Get_Zip_Image_Factor2`): Downsample MT9V03X raw (188×120) → 94×60 by 2×2 averaging
2. **Binarize** (`Get_img_binary`): Otsu thresholding → binary image
3. **Boundary Trace** (`trace_both_boundaries`): Trace left/right track edges from bottom row
4. **Gap/Disconnect Handling** (`Disconnected_Judge`/`Disconnected_Handle`): Detect and fill gaps in boundaries
5. **Centerline Extraction** (`find_center_point`/`trace_center_point`): Compute midline between boundaries
6. **Turn Detection** (`Road_Judge`): Detect upcoming turns from boundary geometry → sets `turning_state`
7. **Final Sum** (`Final_Road`): Compute `Final_Sum` (centerline deviation from image center) — this is the input to the position PID

Key parameters: `Img_Gap_left`/`Img_Gap_right` (look-ahead gap width in pixels) are tuned per `turn_count` stage in `ljy_isr_headle()` to handle different track features.

### Parameter Persistence (Flash)

[Init.h](code/Init.h#L5-L73): `FlashInformation` struct stores all tunable PID/speed parameters. Currently many fields are commented out — PID parameters are hardcoded in `Pid_Init()` rather than loaded from Flash. The menu system can modify and persist these values via `Flash_Value_Write()`/`Flash_Value_Read()`.

### Debug & Visualization

- **VOFA+** ([VOFA.c/h](code/VOFA.h)): Serial protocol for real-time data plotting (float → 4-byte transmission)
- **IPS200 Display** ([ips_show.c/h](code/ips_show.h)): Shows encoder counts, yaw, gyro limits, direction/PWM outputs, turn state when not in menu mode
- **Debug printf**: UART0 redirect via Seekfree library, controlled by `DEBUG_UART_USE_INTERRUPT`
- **SPI Image Output** ([image.c](code/image.c#L12-L15)): `img_SPI[][]` buffer and boundary point arrays for external visualization

## Pin Mapping

- Motor PWMs: `ATOM2_CH2_P11_3` (left), `ATOM2_CH0_P13_3` (right), `ATOM2_CH1_P33_5` (aux left), `ATOM2_CH3_P33_7` (aux right)
- Motor DIRs: `P00_8` (left), `P00_9` (right)
- Brushless PWM: `ATOM0_CH0_P21_2` (`fuya_wushua_pwm`)
- Encoders: `TIM5_ENCODER_CH1_P21_7`/`CH2_P21_6` (left), `TIM3_ENCODER_CH1_P02_6`/`CH2_P02_7` (right)
- Keys: `P13_2`, `P13_0`, `P13_1`, `P15_5`
- DIP switches (BM): `P15_6`, `P15_4`, `P15_2`, `P15_0`, `P15_1`
- Avoid pins: `P14.2-P14.6`, `P10.5-P10.6` (boot pins), `P20.2` (cannot output high), `P21.6` (TC264DA only)

## Important Conventions

- All interrupt priorities **must be unique** (cannot share priority values)
- New code files should be placed directly in the `code/` folder (no subdirectories)
- All application code includes `zf_common_headfile.h` as the universal header — it pulls in everything (SDK, drivers, devices, application headers). Do not include individual library headers directly.
- `cpu_wait_event_ready()` must be called on each core before entering its main loop — it synchronizes core startup.
- The `pit_control_flag` gates the main control ISR — set to 1 after gyro calibration completes, set to 0 on timeout or protection trigger.
- `protect_flag` (set by image processing) triggers immediate motor stop.
- Image dimensions: raw camera 188×120, processed `my_camera_h=60` × `my_camera_w=94` (compressed 2×).
