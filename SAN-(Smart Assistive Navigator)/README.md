# Smart Assistive Navigator (SAN)

<img width="1011" height="791" alt="SAN 1" src="https://github.com/user-attachments/assets/44275c3b-d59e-4338-9a96-b11817c41e64" />

---

![Arduino](https://img.shields.io/badge/Arduino-UNO-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Embedded](https://img.shields.io/badge/Embedded-Systems-success)
![Accessibility](https://img.shields.io/badge/Accessibility-Assistive%20Technology-orange)

---
## Project Overview
* SAN is A wearable assistive navigation system designed to help visually impaired users detect obstacles and unsafe walking surfaces using real-time ultrasonic sensing.*

Instead of relying on a single warning mechanism, the system communicates danger through **vibration**, **audio alerts**, and a **16×2 LCD**, allowing users to receive immediate feedback depending on the surrounding environment.

The project was developed to explore how embedded systems and simple sensors can be combined to solve real-world accessibility problems.

---
## Demo Video

https://github.com/user-attachments/assets/2c6f290e-6d18-4922-b412-0dd310eef9df


# Problem Statement

Millions of visually impaired individuals rely on traditional white canes for navigation.

While effective for nearby obstacles, they often provide limited awareness of:

- obstacles at head level
- objects farther ahead
- sudden drops
- stairs
- potholes

This project explores a low-cost embedded solution capable of providing earlier warnings through intelligent sensing.

---

# Our Solution

```
          Walking Path
                │
                ▼

     Front Ultrasonic Sensor
                │
                ▼
         Arduino UNO
                │
                │
     Ground Ultrasonic Sensor
                │
                ▼
      Intelligent Decision System
                │
      ┌─────────┼─────────┐
      ▼         ▼         ▼
   LCD      Vibrator    Buzzer
```
## Features

- Dual ultrasonic sensing

- Real-time obstacle detection

- Drop detection

- Adaptive audio alerts

- Adaptive vibration feedback

- LCD monitoring

- Sensor fault detection

- Median filtering

- Boot self-test

- Modular firmware

---
## Hardware Table

| Component                 | Qty | Purpose                   |
| ------------------------- | --: | ------------------------- |
| Arduino Uno               |   1 | Main controller           |
| HC-SR04 Ultrasonic Sensor |   2 | Obstacle & drop detection |
| 16×2 LCD                  |   1 | System status             |
| Vibration Motor           |   1 | Haptic feedback           |
| Piezo Buzzer              |   1 | Audio feedback            |
| NPN Transistor            |   1 | Motor driver              |
| Breadboard                |   1 | Prototype                 |
| Jumper Wires              |   — | Connections               |

---

<img width="1738" height="724" alt="SAN - (Smart Assistive Navigator)" src="https://github.com/user-attachments/assets/315533d0-18b0-499b-a635-c413551c5400" />

---
## Firmware Workflow

```
Read Sensors
      │
      ▼
Filter Sensor Readings
      │
      ▼
Determine Navigation State
      │
      ├── SAFE
      ├── NEAR
      ├── CRITICAL
      ├── DROP
      └── SENSOR ERROR
      │
      ▼
Update LCD
      │
      ▼
Generate Audio & Haptic Alerts
```
---  
## System Workflow

```text
        ┌──────────────┐
        │    START     │
        └──────┬───────┘
               │
               ▼
 Read Front & Ground Sensors
               │
               ▼
    Median Noise Filtering
               │
               ▼
 Navigation State Machine
               │
      ┌────────┼────────┐
      ▼        ▼        ▼
 LCD Update  Vibrator  Buzzer
               │
               ▼
          Repeat Loop
```


---

## Alert System

| State        | Vibration      | Buzzer                 |
| ------------ | -------------- | ---------------------- |
| SAFE         | Off            | Off                    |
| NEAR         | Slow pulses    | Slow beeps             |
| CRITICAL     | Fast pulses    | Fast beeps             |
| DROP         | Long vibration | High-frequency warning |
| SENSOR ERROR | Error pulse    | Error tone             |

---
## Repository Structure

```text
Smart-Assistive-Navigator/
│
├── Firmware/
│   └── SAN.ino
│
├── Circuit/
│   └── Tinkercad Design
│
├── Images/
│   ├── Hero.png
│   ├── Circuit.png
│   └── Workflow.png
│
├── Demo/
│   └── Demo.mp4
│
├── BOM/
│   └── Bill_of_Materials.pdf
│
└── README.md
```
---
## Software Architecture

```text
loop()
│
├── Sensor Module
│   ├── Front Sensor
│   └── Ground Sensor
│
├── Signal Processing
│   └── Median Filter
│
├── Decision Engine
│   └── Finite State Machine
│
├── Display Manager
│
├── Alert Controller
│   ├── Vibration Motor
│   └── Piezo Buzzer
│
└── Serial Debug
```
---

## Software Design

The firmware is divided into independent modules to keep the code organized and easier to maintain.

* Sensor acquisition
* Distance filtering
* Navigation state machine
* LCD display manager
* Alert controller
* Boot diagnostics
* Serial debugging

This modular structure made testing and debugging significantly easier during development.


## Engineering Challenges

During development several practical challenges had to be solved:

- Filtering noisy ultrasonic sensor readings
- Preventing false stair detection
- Stabilizing state transitions using hysteresis
- Handling temporary sensor failures
- Designing distinct vibration and buzzer patterns
- Organizing firmware into modular components for easier debugging


## Safety Features

* Sensor failure handling
* Invalid reading rejection
* Cached sensor recovery
* Hysteresis for stable state transitions
* Startup hardware self-test
* Real-time status display

---
## Key Specifications

| Specification | Value |
|--------------|-------|
| Controller | Arduino Uno |
| Programming Language | Embedded C++ |
| Sensors | 2 × HC-SR04 |
| Detection Range | 2–300 cm |
| Drop Detection Threshold | 22 cm |
| Feedback | LCD + Vibration + Buzzer |
| Firmware Architecture | Finite State Machine |
| Signal Processing | Median Filter |


## Future Improvements

Although the current prototype demonstrates the core navigation system, future versions could include:

- Rechargeable battery management

- Waterproof housing

- Voice feedback

- Bluetooth diagnostics

- Lightweight wearable PCB


## What I Learned

Building this project taught me much more than simply connecting sensors to an Arduino.

One thing I didn't expect was how difficult it was to make the sensors reliable. Early versions constantly jumped between states because ultrasonic readings fluctuated. I eventually solved this by combining median filtering, hysteresis, and cached readings instead of relying on raw sensor values. That experience taught me that reliability often comes from small engineering decisions rather than adding more hardware.

Perhaps the biggest lesson was that making a system *reliable* is often more difficult than making it simply *work*.

## Current Status

| Status | Progress |
|---------|----------|
| Prototype | ✅ Complete |
| Firmware | ✅ Complete |
| Simulation | ✅ Tested |
| Documentation | ✅ Complete |
| Demo Video | ✅ Available |
| GitHub Repository | ✅ Public |
---

## Author

**Syed Ihteshamuddin**

Class 12 Student

Interested in Embedded Systems, Electrical Engineering, and Assistive Technologies.

## Feedback

If you have suggestions, ideas, or improvements for this project, feel free to open an issue or reach out. Constructive feedback is always welcome.
---
## License

This project is shared for educational purposes.
