 import cv2
import sys
import logging as log
import datetime as dt
from time import sleep

"""The measurement from where the stage is to where the people will be is about
    """

cascPath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)
log.basicConfig(filename='webcam.log',level=log.INFO)

video_capture = cv2.VideoCapture(0)
anterior = 0

while True:
    if not video_capture.isOpened():
        print('Unable to load camera.')
        sleep(5)
        pass

    # Capture frame-by-frame
    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=7,
        minSize=(75, 75)
        )

    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    if anterior != len(faces):
        anterior = len(faces)
        print(anterior)
        log.info("faces: "+str(len(faces))+" at "+str(dt.datetime.now()))
        print(dt.datetime.now())

        t0 = dt.datetime.now()

        print(t0)


        if anterior >0:
            print("Peekaboo!")

    #tf = dt.datetime.now()
    #lag = tf-t0
    #print(lag)


    # Display the resulting frame
    cv2.imshow('Video', frame)


    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Display the resulting frame
    cv2.imshow('Video', frame)

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
