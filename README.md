# STM32F429 PTPd
---
本项目基于STM32F429阿波罗开发板，由stm32_f4_ptpd项目改进而来，开源项目地址为：https://github.com/mpthompson/stm32_f4_ptpd

## 1. 特点
+ 本项目改进了原项目的PTPd的滤波器，降低同步误差的同时，又加快了收敛速度。
+ 本项目使用了STM32 Cube直接生成的FreeRTOS操作系统。与LwIP协议栈，底层库为HAL库，手动添加了HAL库中没有的ETH模块PTP相关函数。

## 2. 使用方法
将Master与Slave分别烧录至两块不同的开发板，然后用网线直接连接即可。需要注意的是，本项目中物理层芯片复位信号由PA5发出，通过飞线直接连接到物理层芯片复位引脚，若物理层无法复位的可采用此方法。

#### 具体细节可参考文章
Implementation And Analysis of IEEE 1588 PTP Daemon Based on Embedded System
