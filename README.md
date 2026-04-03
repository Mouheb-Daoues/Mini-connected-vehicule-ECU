# Mini Connected Vehicle ECU – Real-Time Automotive System

## 📌 Overview
This project involves designing and implementing a real-time embedded system simulating an automotive ECU based on the **STM32F407 (Cortex-M4)**. It integrates multiple sensors and actuators managed by **FreeRTOS** to ensure deterministic behavior.

## 🏗️ Software Architecture (AUTOSAR-Inspired)
The project follows a layered architecture to ensure modularity and scalability:
- **Application Layer:** Decision logic (e.g., emergency braking based on distance).
- **Service Layer:** RTOS scheduling and communication management.
- **ECU Abstraction Layer:** Sensor data normalization and hardware-independent API.
- **MCAL (Microcontroller Abstraction Layer):** Low-level drivers for ADC, UART, GPIO, and PWM.

## 🧠 RTOS Design (FreeRTOS Tasks)
The system uses a task-based architecture:
- **MotorTask:** ADC acquisition (potentiometer) + PWM update for DC motor control.
- **UltrasonicTask:** Distance calculation and safety logic (buzzer/motor stop).
- **GpsTask:** NMEA parsing and data extraction from NEO-6M module.
- **TemperatureTask:** Thermal monitoring via LM35.
- **DashboardTask:** Real-time system supervision via UART.

## 🛠️ Hardware Components
- STM32F407-Discovery.
- L298N H-Bridge + DC Motor.
- HC-SR04 Ultrasonic Sensor & NEO-6M GPS Module.
- LM35 Temperature Sensor & Active Buzzer.
