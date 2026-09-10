# 第21届全国大学生智能汽车竞赛 — 疯狂电路组

> **成都大学 你还爱我对不队**
>
> 荣获：四川省第一名 · 西部赛一等奖（第二名） · 全国总决赛国家一等奖

---

## 项目简介

本项目为第21届全国大学生智能汽车竞赛 **疯狂电路组** 的嵌入式控制软件。系统基于 **Infineon TC264D** 双核微控制器，搭载 **MT9V03X** 灰度摄像头与 **ICM20602/IMU660RA** 六轴惯性测量单元，实现赛道图像识别与高速循迹控制。

### 主要技术特点

- **双核异构处理**：CPU0 负责控制环路（PID 级联、IMU 姿态解算、电机输出），CPU1 专用于摄像头图像处理流水线
- **级联 PID 控制**：位置 PD（图像偏差）→ 角速度 PD（陀螺仪）→ 速度 PI（编码器），三套独立参数分别用于直道/左转/右转
- **图像处理流水线**：Otsu 自适应二值化 → 双边边界跟踪 → 断点修复 → 中心线提取 → 弯道检测
- **IMU 姿态融合**：Madgwick 互补滤波算法，融合陀螺仪与加速度计数据
- **S 形速度规划**：基于时间的 S-curve 加速度曲线，实现平滑启停
- **Flash 参数持久化**：PID 参数、目标速度等可在运行时通过菜单调参并保存到 Flash

## 成绩

| 赛级 | 奖项 | 名次 |
|------|------|------|
| 四川省赛 | 一等奖 | 第 1 名 |
| 西部赛区 | 一等奖 | 第 2 名 |
| 全国总决赛 | 国家一等奖 | — |

## 硬件平台

| 组件 | 型号 |
|------|------|
| 主控 MCU | **Infineon TC264D** (TriCore™ 双核, 200 MHz) |
| 摄像头 | **MT9V03X** 灰度摄像头 (188×120, 通过 DMA 传输) |
| 陀螺仪/加速度计 | **ICM20602 / IMU660RA** (SPI/I2C) |
| 电机驱动 | 分立 MOS 全桥驱动 |
| 编码器 | 双路正交编码器 (TIM3/TIM5 编码器模式) |
| 显示器 | **IPS200** 全彩显示屏 (调试信息可视化) |
| 电源 | 7.2V 动力锂电池 + 稳压电路 |

## 软件架构

```
TC264/
├── user/                      # 系统入口与中断配置
│   ├── cpu0_main.c            # CPU0 入口（主控制循环）
│   ├── cpu1_main.c            # CPU1 入口（图像处理）
│   ├── isr.c / isr.h          # 中断服务
│   └── isr_config.h           # 中断优先级配置
├── code/                      # 应用算法代码
│   ├── Control.c/h            # 级联 PID 控制 + 弯道状态机
│   ├── Motor.c/h              # 电机 PWM、编码器、GPIO
│   ├── PID.c/h                # 位置式 / 增量式 PID 实现
│   ├── image.c/h              # 图像处理流水线
│   ├── IMU.c/h                # 姿态解算（Madgwick）
│   ├── Filter.c/h             # 低通 / IIR / 卡尔曼滤波器
│   ├── Init.c/h               # 设备初始化 + Flash 参数
│   ├── ljy_menu.c/h           # 三级菜单调参系统
│   ├── ljy_key.c/h            # 按键 / 拨码开关输入
│   ├── ips_show.c/h           # IPS 显示渲染
│   ├── Flash.c/h              # Flash 读写
│   ├── VOFA.c/h               # VOFA+ 串口数据协议
│   └── My_Math.c/h            # 数学工具函数
└── libraries/                  # 逐飞科技开源库（GPLv3）
    ├── zf_driver/              # MCU 外设驱动
    ├── zf_device/              # 外接设备驱动
    └── zf_common/              # 公共工具链
```

### 双核分工

| 核心 | 职责 | 触发方式 |
|------|------|---------|
| **CPU0** | 主控制环路：菜单处理 → 级联 PID 计算 → 电机 PWM 输出 | PIT 1ms 定时中断 (`pit_control_flag`) |
| **CPU1** | 图像处理：压缩 → 二值化 → 边界跟踪 → 弯道检测 | 摄像头 DMA 完成中断 (`mt9v03x_finish_flag`) |

### 级联 PID 控制流程

```
摄像头中心偏差 (Final_Sum)
    ↓ [位置 PD]  →  expect_gyro (目标角速度)
    ↓ [角速度 PD] →  speed_dis_out (差速补偿)
                        ↓
编码器速度  →  [速度 PI]  →  base_pwm
                        ↓
        左轮 = base_pwm - speed_dis_out
        右轮 = base_pwm + speed_dis_out
```

## 开发环境

| 工具 | 版本 |
|------|------|
| IDE | **AURIX Development Studio** v1.10.2 (Eclipse-based) |
| 编译器 | **TASKING C/C++ Compiler** for TriCore (tc26xb) |
| SDK | **Infineon iLLD**（逐飞科技封装库） |

### 编译

```bash
make -C Debug          # 编译项目
make -C Debug clean    # 清理构建产物
```

## 关键参数

| 参数 | 说明 |
|------|------|
| `run` (宏定义) | 1 = 电机运行模式, 0 = 安全调试模式 |
| `mode_fuya` | 0 = 无刷电机, 1 = 有刷电机 |
| `pit_control_flag` | PIT 控制使能标志（陀螺仪校准完成后置 1） |
| PID 参数 | 直道 / 左转 / 右转 三套独立参数 |

## 作者

- **欢乐豆** — [2703767620@qq.com](mailto:2703767620@qq.com)

## 许可证

本项目代码部分遵循 GPLv3 许可证。

逐飞科技开源库（`libraries/` 目录下）基于 GPLv3 许可证，版权归逐飞科技所有。
Infineon iLLD SDK（`libraries/infineon_libraries/` 目录下）版权归 Infineon Technologies 所有。

---

*谨以此项目纪念那些调车到深夜的日子。*