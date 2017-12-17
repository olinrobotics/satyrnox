
#include <Servo.h> //Servo library

//Creating servo objects

Servo BaseRot;
Servo RArmRot;
Servo LArmRot;
Servo RArmLift;
Servo LArmLift;
Servo HeadRot;
Servo HeadLift;

//Pin button is connected to

const int BaseRotPin = 2;
const int RArmRotPin = 3;
const int LArmRotPin = 4;
const int RArmLiftPin = 5;
const int LArmLiftPin = 6;
const int HeadRotPin = 7;
const int HeadLiftPin = 8;

const int HandButton = 10;


//Preset Servo angle settings
//const int initialPos;
const int readyPos = 30;
const int highFivePos = 70;
const int finalPos = 30;


//Variables relating to state of button/servo
bool highFiveOccuring = true;
bool highFiveComplete = false;
int buttonState = 0;
int pos = readyPos;



void setup() {
  Serial.begin(9600);

  BaseRot.attach(BaseRotPin);
  LArmLift.attach(LArmLiftPin); // attaches the servo
  //RArmLift.attach(RArmLiftPin);
  LArmRot.attach(LArmRotPin);
  //RArmRot.attach(RArmRotPin);
  HeadRot.attach(HeadRotPin);
  HeadLift.attach(HeadLiftPin);
  


  pinMode(HandButton, INPUT); // initialize the pin for button as input

  //LArmLift.write(pos) tells servo to go to position 'pos' (degrees)

  LArmLift.write(readyPos); //LArmLift.write(intitialPos);
  BaseRot.write(140);
  HeadRot.write(100);
  HeadLift.write(100);
  LArmRot.write(50);
  delay(1000); // waits x time for the servo to reach the position

}

void loop() { //loop for testing
  //LArmLift.write(100);
  //highFive(readyPos, highFivePos, finalPos);
  //delay(5000);
  //highFiveOccuring = true;


if (buttonState == HIGH) {
    Serial.println("on");
  } else {
    Serial.println("off");
  }
/*

buttonState = digitalRead(HandButton);

  buttonState = 0;

  while ( LArmLift.read() < highFivePos && highFiveComplete == false && highFiveOccuring == true)
  {

    Serial.print(LArmLift.read());
    // goes from readyPos to highFivePos in steps of 1 degree

    buttonState = digitalRead(HandButton);
    Serial.print(" ");
    Serial.print(buttonState);
    Serial.println("");
    
    LArmLift.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    if (buttonState == HIGH)
    {
      highFiveComplete = true;
      pos = LArmLift.read();
      break;
    }

    pos += 1;


  }*/
  
}


void highFive(const int readyPos, const int highFivePos, const int finalPos)
  {

  highFiveOccuring == true; //to remove at some point

  Serial.print("high five");


  while ( LArmLift.read() < highFivePos && highFiveComplete == false && highFiveOccuring == true)
  {
    // goes from readyPos to highFivePos in steps of 1 degree

    buttonState = digitalRead(HandButton);
    LArmLift.write(pos);              // tell servo to go to position in variable 'pos'
    LArmRot.write(pos);
    
    delay(15);                       // waits 15ms for the servo to reach the position
    if (buttonState == HIGH)
    {
      Serial.print("pressed");
      highFiveComplete = true;
      pos = LArmLift.read();
      delay(300);
      break;
    }

    pos += 1;

  }


  while (LArmLift.read() > finalPos && highFiveOccuring == true)
  {
    LArmLift.write(pos);
    delay(20);
    pos -= 1;

  }

  highFiveOccuring = false;

  }



