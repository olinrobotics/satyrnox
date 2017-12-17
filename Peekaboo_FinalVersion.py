import cv2
import sys
import logging as log
import datetime as dt
from time import sleep
import serial
import time

"""This initiates the arduino"""
arduino = serial.Serial("/dev/ttyACM0",9600, timeout =5);
time.sleep(1)
print("Starting up Arduino")
test_arduino = 1
arduino.flush()
if(test_arduino==1):
        data_recieved=arduino.readline()
        time.sleep(1)
        print data_recieved
        test_arduino=test_arduino+1
time.sleep(1)


"""This is all face cascade stuff"""

cascPath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)
log.basicConfig(filename='webcam.log',level=log.INFO)

"""Here it sets up a video stream and intializes
how many faces there are to 0"""

video_capture = cv2.VideoCapture(1)
anterior = 0
happiness_index = 0 #This means the robot is sad
"""This is the while loop for scanning for faces and
the robot reacting to people or lack thereof"""

while True:

    #happiness_index = 0 #This means the robot is sad

    if not video_capture.isOpened():
        print('Unable to load camera.')
        sleep(5)
        pass

    # Capture frame-by-frame
    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(80, 80)
        )

    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    n0=anterior

    #print("First one:",n0)

    if anterior != len(faces):
        anterior = len(faces)

        nf=anterior
        #tf = dt.datetime.now()
        #print("Second one:",nf)
        log.info("faces: "+str(len(faces))+" at "+str(dt.datetime.now()))
        #print(dt.datetime.now())



        if nf > n0:
            #happiness_index = happiness_index+2
            robot_mood=str("Happiness")
            #arduino.write(happiness_index)
            arduino.write(robot_mood)
            #new_data1 = arduino.readline()
            #print new_data1
            print("Happy! :D")

        if nf < n0:
            #happiness_index = happiness_index-1
            robot_mood=str("Nuetral")
            #arduino.write(happiness_index)
            arduino.write(robot_mood)
            #new_data2 = arduino.readline()
            #print new_data2
            #print("Sadness :(")

    #new_data = arduino.readline()
    #print(new_data)
    # Display the resulting frame
    cv2.imshow('Video', frame)

    #if happiness_index >= 3:
	#robot_mood=str("Happiness")
            #arduino.write(happiness_index)
	#arduino.write(robot_mood)
	#print("Happy! :D")
	#if happiness_index >= 4:
	    #happiness_index = 0
    #else:
	#robot_mood=str("Neutral")
            #arduino.write(happiness_index)
	#arduino.write(robot_mood)
	#print("Sadness :(")


    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Display the resulting frame
    cv2.imshow('Video', frame)

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
