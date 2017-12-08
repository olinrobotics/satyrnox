
#include <Servo.h> //Servo library

Servo servo1, servo2; //Creating servo object

//Pins servo is connected to

const int servo1Pin = 3;
const int servo2Pin = 4;

//Preset Servo angle settings
//const int initialPos;

const int readyPos = 170;
const int finalPos = 110;


//Variables relating to state of button/servo

int pos;



void setup() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin); // attaches the servo on pin 3 and 4 to servo object

  //servo1.write(pos) tells servo to go to position 'pos' (degrees)

  servo1.write(readyPos);
  servo2.write(readyPos);
  
  //delay(15); // waits x time for the servo to reach the position

}

void loop() {

  for (int i = 1; i <= 3; i++)
  {
    wiggleArms();
  }
}



void wiggleArms()
{

  for (pos = readyPos; pos <= finalPos; pos -= 5) { // goes from readyPos to finalPos
    servo1.write(pos);              
    servo2.write(pos);                // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = finalPos; pos >= readyPos; pos += 5) { // goes from finalPos to readyPos
    // in steps of 1 degree
    servo1.write(pos);              
    servo2.write(pos);                // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  


}



