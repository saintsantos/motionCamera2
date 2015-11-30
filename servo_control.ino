#include <Servo.h>
Servo servo;
Servo servo1;

int pos = 90; //Stores the position (angle) of the servo. range is [0, 180]
int pos1 = 90;
int ledPin = 13;

void setup() {
  Serial.begin(9600);
  servo.attach(9); //Attaches the servo on pin 9 to the servo object
  servo1.attach(8);
  servo.write(pos); //Resets the position to halfway
  servo1.write(pos1);
  pinMode(ledPin, OUTPUT); 
}

void loop() {
  if (Serial.available()) { //Returns true if there is serial input
     char ch;
     while (ch != 'x') {
       ch = Serial.read();
     }
     // we just received x
     int x = Serial.parseInt();
     while (ch != 'y') {
       ch = Serial.read();
     }
     int y = Serial.parseInt();
   
   // char ch = Serial.read();
    //int x = Serial.parseInt();
    //int y = Serial.parseInt();
    int x_diff = 320 - x;
    int y_diff = 240 - y;
    float x_multiplier = -0.01;
    float y_multiplier = 0.01;
    
    if (x_diff > 20 || x_diff < -20)
    pos1 += x_multiplier*x_diff;
    
    if (y_diff > 20 || y_diff < -20);
    pos += y_multiplier*y_diff;
    
    servo1.write(pos1);
    servo.write(pos);
   
    //Serial.print(x);
    /*
    if (x > 320) {  
       x_diff = x - 320;
    } else {
      x_diff = 320 - x;
    }
    
    if ( y > 240) {
      y_diff = y - 240;
    } else {      
      y_diff = 240 - y;
    }*/
    //pos = x_diff;
    //pos1 = y_diff;
    /*if (ch == 'r') {
      // Make sure not to exceed the mechanical limitation.
      if (pos < 180) {
        pos += 1;
      }
    } else if (ch == 'e') {
      //Make sure not to exceed the mechanical limitation.
      if (pos > 0) {
        pos -= 1;
      }
    } else if (ch == 'q') {
      if (pos1 > 0) {
        pos1 -= 1;
      }
    } else if (ch == 'w') {
      if (pos1 < 180) {
        pos1+= 1;
      }
    } else {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
    }*/

    // Now ask the servo to move to that position.
    
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    // Mechanical limitation to the frequency of commands given.
    //delay(50);
  }
  // put your main code here, to run repeatedly:

}


