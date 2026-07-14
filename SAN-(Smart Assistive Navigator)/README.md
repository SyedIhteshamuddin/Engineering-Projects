# Smart Assistive Navigator (SAN)

*A wearable assistive navigation system designed to help visually impaired users detect obstacles and unsafe walking surfaces using real-time ultrasonic sensing.*

<img width="1011" height="791" alt="SAN 1" src="https://github.com/user-attachments/assets/44275c3b-d59e-4338-9a96-b11817c41e64" />

---

## About the Project

Smart Assistive Navigator (SAN) is an embedded systems project built on the Arduino Uno that assists users while walking by detecting obstacles in front of them and identifying sudden drops such as stairs or potholes.

Instead of relying on a single warning mechanism, the system communicates danger through **vibration**, **audio alerts**, and a **16×2 LCD**, allowing users to receive immediate feedback depending on the surrounding environment.

The project was developed to explore how embedded systems and simple sensors can be combined to solve real-world accessibility problems.

---
## Demo Video

https://github.com/user-attachments/assets/2c6f290e-6d18-4922-b412-0dd310eef9df


## Features

* Dual ultrasonic sensor navigation
* Front obstacle detection
* Ground drop/stair detection
* Real-time finite state machine
* Adaptive vibration feedback
* Adaptive buzzer alerts
* LCD status monitoring
* Sensor failure detection
* Median-based sensor filtering for stable readings
* Boot self-test during startup
* Modular firmware architecture

---

## Hardware Used

| Component                 | Quantity    |
| ------------------------- | ----------- |
| Arduino Uno               | 1           |
| HC-SR04 Ultrasonic Sensor | 2           |
| 16×2 LCD Display          | 1           |
| Vibration Motor           | 1           |
| Piezo Buzzer              | 1           |
| Breadboard                | 1           |
| Jumper Wires              | As required |

---

## How It Works

The system continuously measures two distances:

* **Front Sensor** → detects obstacles directly ahead.
* **Ground Sensor** → measures the distance to the walking surface and detects sudden drops.

Sensor readings are processed through a median filter before being evaluated by the navigation state machine.

Depending on the measured distances, the system enters one of the following states:

* SAFE
* NEAR
* CRITICAL
* DROP
* SENSOR ERROR

Each state generates a unique combination of vibration and buzzer patterns so the user can distinguish different situations without looking at the display.

---

## Navigation Logic

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

## Safety Features

* Sensor failure handling
* Invalid reading rejection
* Cached sensor recovery
* Hysteresis for stable state transitions
* Startup hardware self-test
* Real-time status display

---

## Future Improvements

Although the current prototype demonstrates the core navigation system, future versions could include:

* Rechargeable battery management
* GPS-based outdoor navigation
* Voice guidance
* Bluetooth connectivity
* Mobile application integration
* AI-assisted path planning

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

## Repository Structure

```
Smart-Assistive-Navigator/

Firmware/
Circuit/
Images/
Demo/
Documents/
BOM/
README.md
```

---

## Author

**Syed Ihteshamuddin**

Class 12 Student

Interested in Embedded Systems, Electrical Engineering, and Assistive Technologies.

---

Thank you for visiting this project.

If you have suggestions or ideas for improving the Smart Assistive Navigator, I'd be happy to hear them.
