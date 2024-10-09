"""
Display a video file.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2024, Marc Hensel
@version: 2024.10.09
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2

import sys, os
projectDir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(projectDir + '/../..')
from ImageData import ImageData

# -----------------------------------------------------------------------------

# Configuration
inputImage = 'SoccerShot.mp4'

# -----------------------------------------------------------------------------

# Open video file
inputVideoPath =  ImageData.getVideoPath(inputImage)
video = cv2.VideoCapture(inputVideoPath)

if not video.isOpened():
    print('ERROR: Cannot open video: ' + inputVideoPath)
    exit()

# Get speed (fps)
fps = video.get(cv2.CAP_PROP_FPS)
waitTimeMs = int(1000 / fps) if (fps > 0.0) else int(1000 / 30)

# Loop through frames
while True:
    # Get next frame from file
    ret, frame = video.read()
    
    # Exit loop when no frame left
    if not ret:
        break
        
    # Display frame in named window
    cv2.imshow('Video [Press any key to quit]', frame)

    # Wait (exit loop on key press)
    if cv2.waitKey(waitTimeMs) > 0:
        break

# Free resources
video.release()
cv2.destroyAllWindows()