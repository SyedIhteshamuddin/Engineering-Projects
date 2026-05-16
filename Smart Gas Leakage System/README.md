
Smart Gas Leakage System

This project is an Arduino + Tinkercad based gas monitoring and alert system built while learning embedded systems and automation concepts.

The system continuously monitors gas levels using a gas sensor and responds with different warning stages based on gas concentration.

---

# Features

- Real-time gas monitoring
- Multi-level gas detection
- SAFE / WARNING / DANGER indication
- LCD status display
- LED alert indicators
- Buzzer alarm during dangerous gas levels
- Serial monitor live sensor values

---

# Components Used

- Arduino Uno
- Gas Sensor
- LCD Display (16x2 I2C)
- LEDs
- Buzzer
- Resistors
- Breadboard
- Tinkercad Circuits
<img width="1738" height="724" alt="Smart Gas Leakage System" src="https://github.com/user-attachments/assets/16552253-a5c4-46d2-80c3-683338be4aa0" />
---

# How The System Works

The gas sensor continuously reads surrounding gas concentration levels.

Depending on the sensor value:

- SAFE level → Green LED turns ON
- WARNING level → Yellow LED turns ON
- DANGER level → Red LED + buzzer alarm activate

The LCD display shows:
- current system status
- live gas sensor value

This creates a simple simulation of an automated gas leakage warning system.

---

# Threshold Levels

| Gas Value | System Status |
|-----------|----------------|
| Below 307 | No Gas Detected |
| 307 - 499 | SAFE |
| 500 - 699 | WARNING |
| 700+ | DANGER |

---

# What I Learned

While building this project, I learned:

- basics of gas sensors
- analog sensor readings
- threshold-based system logic
- LCD interfacing
- buzzer integration
- multi-output embedded system behavior
- how monitoring systems react to environmental conditions

I also improved my understanding of how sensors, outputs, and decision-making logic work together in embedded systems.

---

# Future Improvements

Some improvements I may explore later:

- GSM alert system
- IoT monitoring
- mobile notifications
- automatic ventilation system
- real hardware implementation

---

# Project Preview

<img width="1906" height="896" alt="Screenshot 2026-05-16 182553" src="https://github.com/user-attachments/assets/c539ef90-8679-41b0-b576-bca0ad655f10" />


---

# Built Using

- Arduino
- Tinkercad Circuits
- Arduino Blocks + C++

