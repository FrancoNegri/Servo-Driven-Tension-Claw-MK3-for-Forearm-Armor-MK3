#include <ESP32Servo.h>
#include "SSD1306.h"

#define X_AXIS_PIN 25
#define SERVO_PIN 13

// Oled Display
SSD1306  display(0x3c, 5, 4);

// Servo setup
Servo myServo;
int servoAngle = 0;
int joystickValue = 0;

// Movement thresholds
const int deadZone = 300;
const int fastZone = 1000;
const int maxAngle = 180;
const int minAngle = 0;
const int slowStep = 1;
const int fastStep = 5;
const int delayMs = 10;

void setup() {
  Serial.begin(115200);
  // Init servo
  myServo.attach(SERVO_PIN);
  myServo.write(servoAngle);

   // Start the OLED Display
  display.init();
  display.setFont(ArialMT_Plain_24);
  //display.flipScreenVertically();                 // this is to flip the screen 180 degrees

  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "Invent_Ar");
  
  display.display();

  delay(500);
}

void loop() {
  joystickValue = analogRead(X_AXIS_PIN);
  int center = 2048;
  int distance = joystickValue - center;
  int step = 0;
  bool angleChanged = false;
  // Dead zone with dual-speed control
  if (abs(distance) > deadZone) {
    if (abs(distance) > fastZone) {
      step = (distance > 0) ? fastStep : -fastStep;
    } else {
      step = (distance > 0) ? slowStep : -slowStep;
    }

    int newAngle = constrain(servoAngle + step, minAngle, maxAngle);
    if (newAngle != servoAngle) {
      servoAngle = newAngle;
      myServo.write(servoAngle);
      angleChanged = true;
    }
  }
  drawOLED();
}

/******************************************************************************/
/*          Draw OLED                                                         */
/******************************************************************************/
void drawOLED(){

      display.clear();
      display.setColor(WHITE);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(ArialMT_Plain_10);
      display.drawString( 64, 0, "Power Armor");
      display.drawLine(   0, 12, 127, 12);
      display.drawLine(   0, 63, 127, 63);
      display.drawLine(   0, 12,   0, 63);
      display.drawLine( 127, 12, 127, 63);
      display.drawLine(  63, 12,  63, 63);
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER);

      display.drawString( 31, 14, "Servo Angle");
      display.drawString( 95, 14, "Joystick_X_Value");      
      display.setFont(ArialMT_Plain_10);
      display.drawString( 31, 30, String(servoAngle));
      display.setFont(ArialMT_Plain_10);
      display.drawString( 95, 30, String(joystickValue));   

      display.display();      
} //drawOLED()
