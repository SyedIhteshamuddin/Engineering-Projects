Smart Assistive Navigator (SAN)

<img width="1011" height="791" alt="SAN 1" src="https://github.com/user-attachments/assets/44275c3b-d59e-4338-9a96-b11817c41e64" />

*A wearable assistive navigation system designed to help visually impaired users detect obstacles and unsafe walking surfaces using real-time ultrasonic sensing.*

---

![Arduino](https://img.shields.io/badge/Arduino-UNO-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Embedded](https://img.shields.io/badge/Embedded-Systems-success)
![Accessibility](https://img.shields.io/badge/Accessibility-Assistive%20Technology-orange)

---

## About the Project

Smart Assistive Navigator (SAN) is an embedded systems project built on the Arduino Uno that assists users while walking by detecting obstacles in front of them and identifying sudden drops such as stairs or potholes.

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

✔ Dual ultrasonic sensing

✔ Real-time obstacle detection

✔ Drop detection

✔ Adaptive audio alerts

✔ Adaptive vibration feedback

✔ LCD monitoring

✔ Sensor fault detection

✔ Median filtering

✔ Boot self-test

✔ Modular firmware

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
## Working Principle

Start

↓

Read Front Sensor

↓

Read Ground Sensor

↓

Median Filter

↓

Determine Navigation State

↓

Generate Alerts

↓

Update LCD

↓

Repeat

---

# Navigation Logic

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
  
 ## Working Principle
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
      ┌──────────┼──────────┐
      ▼          ▼          ▼
   LCD Update  Vibrator   Buzzer
                 │
                 ▼
             Repeat Loop


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

Smart-Assistive-Navigator/

├── Firmware/
│     └── SAN.ino
│
├── Circuit/
│     └── Tinkercad Design
│
├── Images/
│     ├── Hero.png
│     ├── Circuit.png
│     └── Workflow.png
│
├── Demo/
│     └── Demo.mp4
│
├── BOM/
│     └── Bill_of_Materials.pdf
│
└── README.md

---
## Software Architecture

loop()

│

├── Sensor Module
│      ├── Front Sensor
│      └── Ground Sensor
│
├── Signal Processing
│      └── Median Filter
│
├── Decision Engine
│      └── Finite State Machine
│
├── Display Manager
│
├── Alert Controller
│      ├── Vibration
│      └── Buzzer
│
└── Serial Debug
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


## Safety Features

* Sensor failure handling
* Invalid reading rejection
* Cached sensor recovery
* Hysteresis for stable state transitions
* Startup hardware self-test
* Real-time status display

---
## Key Specifications

Obstacle Detection Range: 2–300 cm

Ground Drop Detection Threshold: 22 cm

Controller: Arduino Uno

Processing: Real-Time FSM

Noise Reduction: Median Filter

Feedback Modes: LCD + Vibration + Buzzer

Firmware Language: Embedded C++


---

## Future Improvements

Although the current prototype demonstrates the core navigation system, future versions could include:

• Battery-powered portable enclosure

• Rechargeable battery management

• Waterproof housing

• Voice feedback

• Bluetooth diagnostics

• Lightweight wearable PCB

• IMU integration for orientation detection

---

## What I Learned

Building this project taught me much more than simply connecting sensors to an Arduino.

Throughout development I learned about:

* Embedded system design
* Sensor calibration
* Finite State Machines (FSM)
* Real-time decision making
* Firmware organization
* Hardware debugging
* Designing reliable user feedback systems

Perhaps the biggest lesson was that making a system *reliable* is often more difficult than making it simply *work*.

---


## Author

**Syed Ihteshamuddin**

Class 12 Student

Interested in Embedded Systems, Electrical Engineering, and Assistive Technologies.

---

Thank you for visiting this project.

If you have suggestions or ideas for improving the Smart Assistive Navigator, I'd be happy to hear them.
