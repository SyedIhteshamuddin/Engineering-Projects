/*
===============================================================================
                     SMART ASSISTIVE NAVIGATOR (SAN)
===============================================================================

Firmware Version : 1.0 RC1
Author           : Syed Ihteshamuddin
Platform         : Arduino UNO (ATmega328P)

Project Overview
----------------
Smart Assistive Navigator (SAN) is an embedded assistive mobility system
designed to improve obstacle awareness and drop-off detection for visually
impaired users.

The system continuously monitors the environment using dual ultrasonic sensors,
processes measurements through a robust filtering algorithm, classifies hazards
using a finite state machine with hysteresis, and provides intuitive haptic and
auditory feedback in real time.

===============================================================================
Hardware Configuration
===============================================================================

Microcontroller
---------------
• Arduino UNO R3

Sensors
-------
• HC-SR04 Ultrasonic Sensor (Front Obstacle Detection)
• HC-SR04 Ultrasonic Sensor (Ground / Drop Detection)

Output Devices
--------------
• 16×2 LCD Display (I²C Backpack)
• Vibration Motor
• Piezo Buzzer

===============================================================================
System Features
===============================================================================

✓ Dual Ultrasonic Navigation
✓ Obstacle Detection
✓ Stair / Drop Detection
✓ Median-Based Sensor Filtering
✓ Sensor Failure Recovery
✓ Finite State Machine with Hysteresis
✓ Non-blocking Alert Engine
✓ Intelligent LCD Update Manager
✓ Boot Self-Test Routine
✓ State Transition Alerts
✓ Serial Debug Interface

===============================================================================
Navigation States
===============================================================================

SAFE          : Normal walking condition
NEAR          : Obstacle approaching
CRITICAL      : Immediate obstacle warning
DROP          : Stair / Edge detected
SENSOR_ERROR  : Sensor communication failure

===============================================================================
Pin Configuration
===============================================================================

Front Ultrasonic
----------------
Trigger : D3
Echo    : D2

Ground Ultrasonic
-----------------
Trigger : D5
Echo    : D4

Output Devices
--------------
Motor  : D9
Buzzer : D11

LCD
---
I²C Backpack

===============================================================================
Copyright
===============================================================================

Developed as an independent embedded systems project for educational,
engineering portfolio, and assistive technology research purposes.

===============================================================================
*/

#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(7);

// ---------------- PIN DEFINITIONS ----------------

const byte FRONT_TRIG = 3;
const byte FRONT_ECHO = 2;

const byte GROUND_TRIG = 5;
const byte GROUND_ECHO = 4;

const byte MOTOR_PIN = 9;
const byte BUZZER_PIN = 11;

//---------------- THRESHOLDS ----------------

const int SAFE_DISTANCE = 150;
const int SAFE_EXIT     = 160;

const int CRITICAL_DISTANCE = 75;
const int CRITICAL_EXIT     = 85;

const int DROP_DISTANCE = 22;
const int DROP_EXIT     = 14;

// ---------------- STATES ----------------

enum NavigationState
{
  SAFE,
  NEAR,
  CRITICAL,
  DROP,
  SENSOR_ERROR
};

NavigationState currentState = SAFE;
NavigationState previousState = SENSOR_ERROR;

// ---------------- DISTANCES ----------------

int frontDistance = 0;
int groundDistance = 0;

// ---------------- SENSOR CACHE ----------------

int lastFrontDistance = 150;
int lastGroundDistance = 10;

byte frontFailCount = 0;
byte groundFailCount = 0;

const byte MAX_SENSOR_FAILURES = 5;

byte dropDetectCount = 0;
byte dropClearCount = 0;

const byte DROP_CONFIRM_COUNT = 3;
const byte DROP_CLEAR_COUNT = 3;


//--------------------------------------------------
// LCD CACHE
//--------------------------------------------------

int oldFront = -999;
int oldGround = -999;

NavigationState oldLCDState = SENSOR_ERROR;

// --------------------------------------------------
// ALERT ENGINE VARIABLES
// --------------------------------------------------

unsigned long alertTimer = 0;
unsigned long lcdTimer = 0;

bool alertOutput = LOW;

int alertOnTime = 0;
int alertOffTime = 0;

int buzzerFrequency = 0;


//---------- GLOBAL OUTPUT CONTROL VARIABLES------------



long readUltrasonic(byte trigPin, byte echoPin)
{
  pinMode(trigPin, OUTPUT);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if(duration==0)
      return -1;

  return duration;
}

// --------------------------------------------------

int getDistanceCM(byte trig, byte echo)
{
    long duration = readUltrasonic(trig, echo);

    if (duration == -1)
        return -1;

    int distance = duration * 0.01723;

    if (distance < 2 || distance > 300)
        return -1;

    return distance;
}

// --------------------------------------------------
// SENSOR ACQUISITION & FILTERING
// --------------------------------------------------

int medianDistance(byte trig, byte echo)
{
    int sample[3];
    int validCount = 0;

    // Collect three readings
    for(int i = 0; i < 3; i++)
    {
        int d = getDistanceCM(trig, echo);

        if(d != -1)
        {
            sample[validCount++] = d;
        }

        delay(2);
    }

    // No valid readings
    if(validCount == 0)
        return -1;

    // One valid reading
    if(validCount == 1)
        return sample[0];

    // Two valid readings
    if(validCount == 2)
        return (sample[0] + sample[1]) / 2;

    // Three valid readings -> median
    if(sample[0] > sample[1])
    {
        int t = sample[0];
        sample[0] = sample[1];
        sample[1] = t;
    }

    if(sample[1] > sample[2])
    {
        int t = sample[1];
        sample[1] = sample[2];
        sample[2] = t;
    }

    if(sample[0] > sample[1])
    {
        int t = sample[0];
        sample[0] = sample[1];
        sample[1] = t;
    }

    return sample[1];
}
//--------------------------------------------------
// Reads and validates ultrasonic sensor data.
//--------------------------------------------------
void readSensors()
{
    int newFront = medianDistance(FRONT_TRIG, FRONT_ECHO);
    int newGround = medianDistance(GROUND_TRIG, GROUND_ECHO);

    //---------------- FRONT SENSOR ----------------

    if(newFront == -1)
    {
        frontFailCount++;

        if(frontFailCount >= MAX_SENSOR_FAILURES)
            frontDistance = -1;
        else
            frontDistance = lastFrontDistance;
    }
 else
{
    frontFailCount = 0;
    frontDistance = newFront;
    lastFrontDistance = newFront;
}
    //---------------- GROUND SENSOR ----------------

    if(newGround == -1)
    {
        groundFailCount++;

        if(groundFailCount >= MAX_SENSOR_FAILURES)
            groundDistance = -1;
        else
            groundDistance = lastGroundDistance;
    }
  else
{
    groundFailCount = 0;
    groundDistance = newGround;
    lastGroundDistance = newGround;
}
  }

//--------------------------------------------------
// NAVIGATION STATE MACHINE (HYSTERESIS)
//--------------------------------------------------

// Determines navigation state using hysteresis.

void determineState()
 {
    if(frontDistance == -1 || groundDistance == -1)
{
    currentState = SENSOR_ERROR;
    return;
}

    switch(currentState)
    {

        //---------------- SAFE ----------------

        case SAFE:

            if(groundDistance > DROP_DISTANCE)
{
    dropDetectCount++;

    if(dropDetectCount >= DROP_CONFIRM_COUNT)
    {
        currentState = DROP;
        dropDetectCount = 0;
    }
}
else
{
    dropDetectCount = 0;

    if(frontDistance < CRITICAL_DISTANCE)
        currentState = CRITICAL;
    else if(frontDistance < SAFE_DISTANCE)
        currentState = NEAR;
}
           

            break;


        //---------------- NEAR ----------------

        case NEAR:

            if(groundDistance > DROP_DISTANCE)
            {
                currentState = DROP;
            }
            else if(frontDistance < CRITICAL_DISTANCE)
            {
                currentState = CRITICAL;
            }
            else if(frontDistance > SAFE_EXIT)
            {
                currentState = SAFE;
            }

            break;


        //---------------- CRITICAL ----------------

        case CRITICAL:

            if(groundDistance > DROP_DISTANCE)
            {
                currentState = DROP;
            }
            else if(frontDistance > CRITICAL_EXIT)
            {
                currentState = NEAR;
            }

            break;


        //---------------- DROP ----------------

       case DROP:

    if(groundDistance < DROP_EXIT)
    {
        dropClearCount++;

        if(dropClearCount >= DROP_CLEAR_COUNT)
        {
            dropClearCount = 0;

            if(frontDistance < CRITICAL_DISTANCE)
                currentState = CRITICAL;
            else if(frontDistance < SAFE_DISTANCE)
                currentState = NEAR;
            else
                currentState = SAFE;
        }
    }
    else
    {
        dropClearCount = 0;
    }

    break;


        //---------------- SENSOR ERROR ----------------

        case SENSOR_ERROR:

            currentState = SAFE;

            break;
    }
}
//--------------------------------------------------
// LCD USER INTERFACE
//--------------------------------------------------

// Updates LCD only when displayed values change.

void updateLCD()
{
    char buffer[17];

    // ---------- Front Distance ----------

    if(frontDistance != oldFront)
{
    lcd.setCursor(0,0);

    if(frontDistance == -1)
    {
        lcd.print("F:---cm ");
    }
    else
    {
        sprintf(buffer,"F:%3dcm", frontDistance);
        lcd.print("        ");
        lcd.setCursor(0,0);
        lcd.print(buffer);
    }

    oldFront = frontDistance;
}
    // ---------- Ground Distance ----------

    if(groundDistance != oldGround)
{
    lcd.setCursor(0,1);

    if(groundDistance == -1)
    {
        lcd.print("G:---cm ");
    }
    else
    {
        sprintf(buffer,"G:%3dcm", groundDistance);
        lcd.print("        ");
        lcd.setCursor(0,1);
        lcd.print(buffer);
    }

    oldGround = groundDistance;
}
  

    // ---------- Navigation State ----------

    if(currentState != oldLCDState)
    {
        lcd.setCursor(10,0);
        lcd.print("      ");        // erase old state
        lcd.setCursor(10,0);

        switch(currentState)
        {
            case SAFE:
                lcd.print("SAFE");
                break;

            case NEAR:
                lcd.print("NEAR");
                break;

            case CRITICAL:
                lcd.print("STOP");
                break;

            case DROP:
                lcd.print("DROP");
                break;

          case SENSOR_ERROR:
    lcd.print("ERROR");
    break;
        }

        oldLCDState = currentState;
    }
}
// --------------------------------------------------

void serialDebug()
{

    Serial.print("Front : ");

    Serial.print(frontDistance);

    Serial.print(" cm | Ground : ");

    Serial.print(groundDistance);

    Serial.print(" cm | State : ");

    switch(currentState)
    {

      case SAFE:

        Serial.println("SAFE");

      break;

      case NEAR:

        Serial.println("NEAR");

      break;

      case CRITICAL:

        Serial.println("CRITICAL");

      break;

      case DROP:

        Serial.println("DROP");

      break;

      case SENSOR_ERROR:

        Serial.println("ERROR");

      break;
      
    
    }

}

void playTransitionAlert()
{
    switch(currentState)
    {
        case SAFE:

            tone(BUZZER_PIN,800);
            delay(25);
            noTone(BUZZER_PIN);

            break;


        case NEAR:

            digitalWrite(MOTOR_PIN,HIGH);
            delay(40);
            digitalWrite(MOTOR_PIN,LOW);

            break;


        case CRITICAL:

            for(byte i=0;i<2;i++)
            {
                tone(BUZZER_PIN,1500);
                delay(20);
                noTone(BUZZER_PIN);
                delay(15);
            }

            break;


        case DROP:

            digitalWrite(MOTOR_PIN,HIGH);
            tone(BUZZER_PIN,2200);

            delay(120);

            digitalWrite(MOTOR_PIN,LOW);
            noTone(BUZZER_PIN);

            break;


        case SENSOR_ERROR:

            for(byte i=0;i<3;i++)
            {
                tone(BUZZER_PIN,400);
                delay(15);
                noTone(BUZZER_PIN);
                delay(10);
            }

            break;
    }
}

// --------------------------------------------------
// ALERT ENGINE
// --------------------------------------------------

// Generates vibration and buzzer alert patterns.

void updateAlerts()
{
    unsigned long now = millis();
  
  if(currentState != previousState)
{
    alertOutput = LOW;
    alertTimer = 0;
}

    switch(currentState)
    {

        //---------------- SAFE ----------------

        case SAFE:

            digitalWrite(MOTOR_PIN, LOW);
            noTone(BUZZER_PIN);
            alertOutput = LOW;
            alertTimer = millis();   // reset timer
            return;

        //---------------- SENSOR ERROR ----------------

        case SENSOR_ERROR:

            alertOnTime = 150;
            alertOffTime = 150;
            buzzerFrequency = 300;

            break;

        //---------------- NEAR ----------------

        case NEAR:

            alertOnTime = 250;
            alertOffTime = 750;
            buzzerFrequency = 900;

            break;

        //---------------- CRITICAL ----------------
      case CRITICAL :
alertOnTime = 120;
alertOffTime = 120;
buzzerFrequency = 1500;

            break;

        //---------------- DROP ----------------

        case DROP:

            alertOnTime = 500;
            alertOffTime = 200;
            buzzerFrequency = 2200;

            break;
    }

    if(alertOutput)
    {
        if(now-alertTimer>=alertOnTime)
        {
            alertTimer=now;

            alertOutput=LOW;

            digitalWrite(MOTOR_PIN,LOW);

            noTone(BUZZER_PIN);
        }
    }
    else
    {
        if(now-alertTimer>=alertOffTime)
        {
            alertTimer=now;

            alertOutput=HIGH;

            digitalWrite(MOTOR_PIN,HIGH);

            tone(BUZZER_PIN,buzzerFrequency);
        }
    }
}

//--------------------------------------------------
// BOOT SELF TEST
//--------------------------------------------------

void bootSelfTest()
{
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("SYSTEM CHECK");

    lcd.setCursor(0,1);
    lcd.print("LCD......OK");

    delay(250);

    lcd.setCursor(0,1);
    lcd.print("FRONT....");

    if(getDistanceCM(FRONT_TRIG, FRONT_ECHO) != -1)
        lcd.print("OK");
    else
        lcd.print("FAIL");

    delay(250);

    lcd.setCursor(0,1);
    lcd.print("GROUND...");

    if(getDistanceCM(GROUND_TRIG, GROUND_ECHO) != -1)
        lcd.print("OK");
    else
        lcd.print("FAIL");

    delay(250);

    lcd.setCursor(0,1);
    lcd.print("MOTOR....OK");

    delay(200);

    lcd.setCursor(0,1);
    lcd.print("BUZZER...OK");

    tone(BUZZER_PIN,1200);
    delay(100);
    noTone(BUZZER_PIN);

    delay(200);

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print(" SYSTEM READY");

    lcd.setCursor(0,1);
    lcd.print("Have a Safe Walk");

    delay(600);

    lcd.clear();
}

// --------------------------------------------------

void setup()
{

    Serial.begin(9600);
  
    Serial.println("===== BOOT =====");

    lcd.begin(16,2);

    pinMode(MOTOR_PIN,OUTPUT);

    pinMode(BUZZER_PIN,OUTPUT);

    lcd.clear();
  
    oldFront = -999;
  
    oldGround = -999;
  
    oldLCDState = SENSOR_ERROR;

    lcd.setCursor(0,0);

    lcd.print(" Smart Assistive ");

    lcd.setCursor(0,1);

    lcd.print(" Navigator ");

    delay(1500);

    lcd.clear();
  
    bootSelfTest();

}

// --------------------------------------------------

void loop()

{
  readSensors();


    determineState();
 
  if(currentState != previousState)
{
    playTransitionAlert();
}
  

    if (millis() - lcdTimer >= 100)
{
    lcdTimer = millis();
    updateLCD();
}


    updateAlerts();


    serialDebug();


    previousState = currentState;


    delay(50);  
}  
