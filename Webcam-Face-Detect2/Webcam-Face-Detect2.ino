#include <Servo.h> //Servo library

int counter=1;

Servo BaseRot; Servo RArmRot; Servo LArmRot; Servo RArmLift; Servo LArmLift; Servo HeadRot; Servo HeadLift;
const int BaseRotPin = 2;
const int RArmRotPin = 3;
const int LArmRotPin = 4;
const int RArmLiftPin = 5;
const int LArmLiftPin = 6;
const int HeadRotPin = 7;
const int HeadLiftPin = 8;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  BaseRot.attach(BaseRotPin);
  LArmLift.attach(LArmLiftPin); // attaches the servo
  RArmLift.attach(RArmLiftPin);
  LArmRot.attach(LArmRotPin);
  RArmRot.attach(RArmRotPin);
  HeadRot.attach(HeadRotPin);
  HeadLift.attach(HeadLiftPin);
}




void loop() {
  // put your main code here, to run repeatedly:

if (counter==1)
   {
    Serial.println("Arduino Started!");
    delay(1000);
    counter++;
   }
delay(1000);
while (Serial.available())
{
  //delay(300);
  while (counter==2)
     {
      String robot_mood = Serial.readStringUntil('\n');
      if (robot_mood.length() ==9) //This means that the robot is happy. Any code for whatever it will do when it is happy should
      //be down below in the place of the code that turns the LED on and off
          happy(); 
      else //this means that the robot is nuetral. Down below should be code that just tells the robot not to do anything. 
      //Or to do whatever it might do if no one is around.
          RArmLift.write(60);
          LArmLift.write(140);
          delay(1000);
      
      
     
     }
}
}


void happy(){
  //can we make it squeal with a sound file? later
  for (pos = 90; pos <= 150; pos = pos+2) { // goes from readyPos to finalPos
    
    if(LArmLift.read()>0 ){
    RArmLift.write(pos);  
    Serial.println(RArmLift.read());            
    LArmLift.write(180-pos);                // tell servo to go to position in variable 'pos'
    Serial.println(LArmLift.read());

    BaseRot.write(pos/2 +70);
    HeadLift.write(pos/2 +70);

    delay(15);                       // waits 15ms for the servo to reach the position
    }
    //delay(20);
  }

  
}
