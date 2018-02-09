/***********************************************************************************************
 *  2017 Olin Synthespian Robot Brain for Arduino
 *  This brain structure template contains a SENSE-THINK-ACT data flow to 
 *  allow a Synthespian Robot to perform a sequence of meta-behaviors to
 *  accomplish a generic robot mission.
 *  Robot Name: ? Synthespian Rev A Arduino Mega main board DF-Robot Gravity i/o shield
 *  What does code do: ? Take text based commands from serial console to move robot
 *  Hardware warnings: ? Turn e-stop motor power on AFTER starting this code
 *  Created by D.Barrett Nov 2017
 *  ********************************************************************************************
 */
//TODO Add an interrupt way to stop serial read loop to allow reprogramming!
//TODO Add an interrupt driven E-Stop
// TODO compare motor move speed to loop cycle time and fix for open loop operation
//or add in a closed loop check somehow. 
// TODO add relay e-stop for console estop operation

//==============================================================================================
// Load supporting Arduino Libraries
//=============================================================================================
#include <Servo.h>               //Synthespian has 7 RC ServoMotors
#include "pitches.h"             //Synthespian has 1 speaker, this is the sound library to drive it

//==============================================================================================
// Create and initialize global variables, objects and constants (containers for all data)
//===============================================================================================
const int aliveLED = 13;         //create a name for "robot alive" blinky light pin
const int speakerPin = 9;        //create a name for speaker pin
const int eStopPin = 12;         //create a name for pin connected to ESTOP switch
boolean aliveLEDState = true;    //create a name for alive blinky light state to be used with timer
boolean ESTOP = true;            //create a name for Emergency stop of all motors
boolean realTimeRunStop = true;  //create a name for real time control loop flag
boolean motorError = true;       //create a name for motor error flag
String command = "stop    ";     //create a String object name for operator command string
String loopError = "no error";   //create a String for the real time control loop error system
unsigned long oldLoopTime = 0;   //create a name for past loop time in milliseconds
unsigned long newLoopTime = 0;   //create a name for new loop time in milliseconds
unsigned long cycleTime = 0;     //create a name for elapsed loop cycle time
const long controlLoopInterval 
= 500 ;                         //create a name for control loop cycle time in milliseconds
                                       //HS-322HD servo 201deg travel 553-2450 microsecond command 
int baseRot[10]= {90};                 //create a base servo position global variable
int rArmRot[10]= {90};                 //create a right arm rotate servo position global variable
int rArmLift[10]= {90};                //create a right arm lift servo position global variable
int lArmRot[10]= {90};                 //create a left arm rotate servo position global variable
int lArmLift[10]= {90};                //create a laft arm lift servo position global variable
int headRot[10]= {90};                 //create a head rotate servo position global variable
int headLift[10]= {90};                //create a head lift servo position global variable
int servoSpeed = 400;                  // 400 degrees per second 40 degrees in 0.1 seconds
Servo basePanServo;              //create servo object rotates (pans) Synth about fixed base
Servo rightArmPanServo;          //create servo object rotates right Synth arm fore/aft wrt body
Servo leftArmPanServo;           //create servo object rotates left Synth arm fore/aft wrt body
Servo rightArmTiltServo;         //create servo object rotates right Synth arm up/down wrt body
Servo leftArmTiltServo;          //create servo object rotates left Synth arm up/down wrt body
Servo headPanServo;              //create servo object rotates Synth head left/right wrt body
Servo headTiltServo;             //create servo object rotates Synth head up/down wrt body

//================================================================================================
// Startup code to configure robot and pretest all robot functionality (to run once) 
// and code to setup robot mission for launch.
//================================================================================================
void setup() {
  // 1)Put your robot setup code here, to run once:
  pinMode(aliveLED, OUTPUT);        // initialize aliveLED pin as an output
  pinMode(eStopPin, INPUT_PULLUP);  // use internal pull-up on ESTOP switch input pin
  basePanServo.attach(2);           // attaches the servo on pin 2 to the base pan servo 
  rightArmPanServo.attach(3);       // attaches the servo on pin 3 to the right arm pan servo
  leftArmPanServo.attach(4);        // attaches the servo on pin 4 to the right arm pan servo
  rightArmTiltServo.attach(5);      // attaches the servo on pin 5 to the right arm tilt servo
  leftArmTiltServo.attach(6);       // attaches the servo on pin 6 to the left arm tilt servo
  headPanServo.attach(7);           // attaches the servo on pin 7 to the head pan servo
  headTiltServo.attach(8);          // attaches the servo on pin 8 to the right arm pan servo
  wakeUp();                         // play wake up melody
  Serial.begin(9600);               // start serial communications
  Serial.println(" Set console to 9600 baud, No line ending");
  
  //TODO chnage to servo.attach(pin, min, max) use to set servo limits min 544 max 2400 microseconds
  //TODO 544 is 0 degrees 2400 is 180 degrees

  // put your mission setup code here, to run once:
  // TODO add mssion download and actuator sensor startup check code here
}

//==================================================================================================
// Flight code to run continuously until robot mission complete  
//==================================================================================================
void loop() {
  // 2)Put Operator-Input-to-Robot and State-Reports-from-Robot code in non-real-time "outer" void loop:
  // GET Operator Control Unit (OCU) Input ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu------------
  
  int iloop = 0;                                              // create a local loop index
  command = getOperatorInput();                               // get operator input from serial monitor
  loopError = buildMissionDefFile(command);                   // build an MDF based on operator input
  printMDF();
  Serial.println(loopError);
  realTimeRunStop = true;                                     // Set loop flag to run = true                                   
  
  // real-time******real-time******real-time******real-time******real-time******real-time******real-time******
  // 3)Put your main flight code into "inner" soft-real-time while loop structure below, to run repeatedly, 
  //   at a known fixed "real-time" periodic interval. This "soft real-time" loop timimg structure, runs 
  //   fast flight code loop once every controlLoopInterval.
  
  //   Start by checking if operator has input a command during real-time loop execution 
  while(realTimeRunStop == true) {                            // if Stop not commanded, run control loop
    if (Serial.available() > 0) {                             // check to see if operator typed at OCU
      realTimeRunStop = false;                                // if OCU input typed, stop control loop
      command = Serial.readString();                          // read command string to clear buffer
      }                                                       
    else {realTimeRunStop = true;}                            // if no operator input, run real-time loop
    
    newLoopTime = millis();                                    // get current Arduino time (50 days till wrap)
    // Serial.println(newLoopTime - oldLoopTime);
    if (newLoopTime - oldLoopTime >= controlLoopInterval) {    // if true run flight code 
      oldLoopTime = newLoopTime;                               // reset time stamp 
      blinkAliveLED();                                         // toggle blinky alive light
      noTone(speakerPin);

     
    // SENSE  sense---sense---sense---sense---sense---sense---sense---sense---sense---sense---sense---------
       //TODO add pixiecam and capacitive touch sensor code here
       
    // THINK  think---think---think---think---think---think---think---think---think---think---think---------
    // pick robot behavior based on operator input command typed at console
       if ( command == "stop"){
         Serial.println("Stop Robot");
         tone(speakerPin,NOTE_B2,120);
         realTimeRunStop = false;                               //exit real time control loop
       } 
       else if (command == "move"){                             //Move robot to Operator commanded position
         // TODO play moving sound here?
         Serial.println("Move to robot to Operator comanded positions");
         realTimeRunStop = false;                               //exit loop after running once
       }
        else if (command == "idle"){                            //Make robot alive with small motions
         Serial.println("Idle Robot");
         // TODO play idle sound here?
         tone(speakerPin,NOTE_B2,120);
         realTimeRunStop = true;                                //run loop continually                
       }
        else if (command == "wave")                             //Make robot wave once
       {
         Serial.println("Robot Wave");
         //TODO play wave sound
         tone(speakerPin,NOTE_C2,120);
         tone(speakerPin,NOTE_CS3,30);
         realTimeRunStop = true;                               //run loop continually 
       }
        else if (command == "point")
       {
         Serial.println("Robot Point");
         //TODO play point sound
         tone(speakerPin,NOTE_F4,130);
         realTimeRunStop = false;                               //exit loop after running once 
       }
        else if (command == "dance")
       {
         Serial.println("Robot dance");
         //TODO play dance sound
         realTimeRunStop = true;                               //run loop continually
       }
       else
       {
         Serial.println("Invalid Input, Robot Stopped, Please try again!");
         realTimeRunStop = false;
       } 
     
    // ACT    act---act---act---act---act---act---act---act---act---act---act---act---act---act------------
       ESTOP = digitalRead(eStopPin);                                 // check ESTOP switch
       if (ESTOP == false) {                                          // only run motors if E-Stop is off
       motorError = moveMotors(baseRot[iloop],rArmRot[iloop],
       rArmLift[iloop],lArmRot[iloop],
       lArmLift[iloop],headRot[iloop],headLift[iloop]);
       //TODO have moveMotors return a string error message instead of binary
       //TODO seem to be dropping movements on contiusous motion, find and fix
       //TODO stop command should estop motors too
       }
      iloop = iloop + 1 ;                                               //increment loop index  
      if (iloop > 9) { iloop = 0;}                                    //reset loop after 10 motion frames
      //Serial.println(iloop);
                
    } // end of  "if (newLoopTime - oldLoopTime >= controlLoopInterval)" real-time structure 
    
    cycleTime = millis()-newLoopTime;                                 // calculate loop execution time 
    // Serial.println(cycleTime);   
    if( cycleTime > controlLoopInterval){
      Serial.println("******************************************");
      Serial.println("error - real time has failed, stop robot!");    // loop took too long to run
      Serial.print(" 1000 ms real-time loop took = ");
      Serial.println(cycleTime);                                      // print loop time
      Serial.println("******************************************");
      break;                                                          // break out of real-time inner loop  
    } 
      
  } // end of "inner"  "while(realTimeRunStop == true)" real-time controlLoopInterval -------------------------
  // real-time******real-time******real-time******real-time******real-time******real-time******real-time******
   
  // SEND Robot State to Operator Control Unit (OCU)ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu-------------------
    Serial.println("  ");
    Serial.println("__________________________________________________________________________________");
    Serial.println("Robot control loop stopping to wait for new behavior command "); // send robot status to operator
    if (ESTOP == true) Serial.println("Robot motors E-Stopped by external switch");  // send E-Stop mesage to OCU
    // Serial.println("    ");    
    
} // end of "outer" void loop()
//===================================================================================================================
// END OF Flight Code
//===================================================================================================================


//===================================================================================================================
// Functions for each section of above code 

//-------------------------------------------------------------------------------------------
// 1)Functions for setup code 
//-------------------------------------------------------------------------------------------
void wakeUp(){
// this function plays a wake up song to let user know robot is alive and running
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

// iterate over the notes of the melody:
for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speakerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
} 
 

//-------------------------------------------------------------------------------------------
// 2)and 3) Functions for flight code
//-------------------------------------------------------------------------------------------

// Realtime loop functions loop---loop---loop---loop---loop---loop---loop---loop---loop---loop----
void blinkAliveLED(){
  // This function toggles state of aliveLED blinky light LED
  // if the LED is off turn it on and vice-versa:
    if (aliveLEDState == LOW) {
      aliveLEDState = HIGH;
    } else {
      aliveLEDState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(aliveLED, aliveLEDState); 
}

// OCU functions ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu---ocu------------
String getOperatorInput(){
  // This function prints operator behavior command options on the serial console and prompts 
  // operator to type input for desired robot behavior
  Serial.println("    ");
  Serial.println("=======================================================================================");
  Serial.println("| Robot Behavior-Commands: move(moves robot), stop(e-stops motors), idle(robot idles) |");
  Serial.println("| dance (robot dances), point(robot points), wave(robot waves)                        |");
  Serial.println("=======================================================================================");
  Serial.print("Please type desired robot behavior in command line above ^^^^^ ");   
  Serial.println("and then press SEND button.");               // ask for operator command input
  while (Serial.available()==0) {};                           // do nothing until operator input typed
  command = Serial.readString();                              // read command string
  //TODO need to convert string based human commnad into enum for finite state machine
  //could alternatively write a new stringSwitch("string") that used strcmp and ifs to do this
  Serial.print("New robot behavior command is:  ");           // Give command feedback to operator
  Serial.println(command);
  Serial.println("type 'stop' to stop control loop and wait for new command");
  Serial.println("____________________________________________________________________________________");
  return command;  
}

String buildMissionDefFile(String command){
  //This function takes the either builds a desired robot action (Mission Definition File) file from command
  //or asks for additional information before building the MDF 
  //returns an error message stating all is ok or indicating operator input failed   
   String errorMessage = "MDF built ok"; 
   int randomMotion = 90;                               //random motion local variable 
   
   if (command == "stop"){
        baseRot[0] = 90;               
        rArmRot[0] = 90;                
        rArmLift[0] = 90;               
        lArmRot[0] = 90;                
        lArmLift[0] = 90;               
        headRot[0] = 90;               
        headLift[0] = 90;    
       } 
       else if (command == "move"){                     //move individual joints to commanded angle
        Serial.print("Enter base rotation angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        baseRot[0]=Serial.parseInt();                   // enter angle
        Serial.println(baseRot[0]);
        Serial.print("Enter right arm rotation angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        rArmRot[0]=Serial.parseInt();                   // enter angle
        Serial.println(rArmRot[0]);
        Serial.print("Enter right arm lift angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        rArmLift[0]=Serial.parseInt();                   // enter angle
        Serial.println(rArmLift[0]);
        Serial.print("Enter left arm rotation angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        lArmRot[0]=Serial.parseInt();                   // enter angle
        Serial.println(lArmRot[0]);
        Serial.print("Enter left arm lift angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        lArmLift[0]=Serial.parseInt();                   // enter angle
        Serial.println(lArmLift[0]);
        Serial.print("Enter head rotation angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        headRot[0]=Serial.parseInt();                   // enter angle
        Serial.println(headRot[0]);
        Serial.print("Enter head lift angle 0-180: ");       
        while(Serial.available()==0){}                  // wait for operator input
        headLift[0]=Serial.parseInt();                   // enter angle
        Serial.println(headLift[0]);
        }
       else if (command == "idle"){                     //make small random robot motions
        for(int J=0; J <= 9 ; J = J+1){
         baseRot[J] = random(90,100);              
         rArmRot[J] = random(90,95);                
         rArmLift[J] = random(90,95);               
         lArmRot[J] = random(90,97);                
         lArmLift[J] = random(90,97);               
         headRot[J] = random(90,96);               
         headLift[J] = random(90,96); 
        }              
       errorMessage = "Idle MDF built";
       }
        else if (command == "wave"){                    //Make robot wave
        for(int J=0; J <= 9 ; J = J+1){                // servo can only move 400 deg per 1.0sec
         baseRot[J] = 90;              
         rArmRot[J] = 90;                              
         lArmRot[J] = 90;                
         lArmLift[J] = 90;               
         headRot[J] = 90;               
         headLift[J] = 90; 
        }
        rArmLift[0] = 90;                               //hardcode wave in
        rArmLift[1] = 110;
        rArmLift[2] = 90;
        rArmLift[3] = 110;
        rArmLift[4] = 90;
        rArmLift[5] = 110;
        rArmLift[6] = 90;
        rArmLift[7] = 110;
        rArmLift[8] = 90;
        rArmLift[9] = 110; 
        lArmLift[0] = 90;                               //hardcode wave in
        lArmLift[1] = 110;
        lArmLift[2] = 90;
        lArmLift[3] = 110;
        lArmLift[4] = 90;
        lArmLift[5] = 110;
        lArmLift[6] = 90;
        lArmLift[7] = 110;
        lArmLift[8] = 90;
        lArmLift[9] = 110;                      
        errorMessage = "Wave MDF built";  
       }
        else if (command == "point"){                   //Make Robot point forward
         for(int J=0; J <= 9 ; J = J+1){                // servo can only move 400 deg per 1.0sec
         baseRot[J] = 90;              
         rArmRot[J] = 90;
         rArmLift[J] = 90;                              
         lArmRot[J] = 90;                
         lArmLift[J] = 90;               
         headRot[J] = 90;               
         headLift[J] = 90; 
        } 
         baseRot[0] = 150;               
         rArmRot[0] = 75;
         rArmLift[0] = 100;                               
         lArmRot[0] = 90; 
         lArmLift[0] = 30;               
         headRot[0] = 60;               
         headLift[0] = 90;
         errorMessage = "Point MDF built"; 
       }
        else if (command == "dance") {                  //Make Robot dance
        for(int J=0; J <= 9 ; J = J+1){                 //TODO linear ramp smoothly
         baseRot[J] = random(80,140);                   //TODO instead of random jerky             
         rArmRot[J] = random(80,100);                
         rArmLift[J] = random(40,120);               
         lArmRot[J] = random(80,100);                
         lArmLift[J] = random(40,120);               
         headRot[J] = random(70,120);               
         headLift[J] = random(75,95);
        }
        errorMessage = "dance MDF built"; 
       }
       else
       {
         errorMessage= "Invalid Input, Robot Stopped, No MDF created. Please try again!";
         
       } 
       return errorMessage;
}

void printMDF(){
  // This function prints the next to run MDF in matrix format
  Serial.println("baseRot,rArmRot,rArmLift,lArmRot,LArmLift,headRot,headLift");
  for(int J=0; J <= 9 ; J = J+1){
         Serial.print (J),Serial.print (" "),Serial.print (baseRot[J]), Serial.print(",");              
         Serial.print (rArmRot[J]), Serial.print(",");                
         Serial.print (rArmLift[J]), Serial.print(",");               
         Serial.print (lArmRot[J]), Serial.print(",");                
         Serial.print (lArmLift[J]),Serial.print(",");               
         Serial.print (headRot[J]), Serial.print(",");               
         Serial.println(headLift[J]);

        }             
}

// SENSE functions sense---sense---sense---sense---sense---sense---sense---sense---sense---

// THINK functions think---think---think---think---think---think---think---think---think---

// ACT functions act---act---act---act---act---act---act---act---act---act---act---act---act---
boolean moveMotors(int baseRot, int rArmRot, int rArmLift, int lArmRot, int lArmLift, int headRot, int headLift){
  //This function moves all 6 of the Synthespians servo motors and returns "True" when finished
   basePanServo.write(baseRot);
   rightArmPanServo.write(rArmRot);
   leftArmPanServo.write(lArmRot); 
   rightArmTiltServo.write(rArmLift);
   leftArmTiltServo.write(lArmLift);      
   headPanServo.write(headRot);          
   headTiltServo.write(headLift);      
   return true;
}

// END of Functions
//============================================================================================
// END OF CODE
