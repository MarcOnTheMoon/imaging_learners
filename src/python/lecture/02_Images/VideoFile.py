"""
Display a video file.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import sys, os

# Configuration
video_path = os.environ['ImagingData'] + '/videos/SoccerShot.mp4'

# Open video file
video = cv2.VideoCapture(video_path)

if not video.isOpened():
    print('ERROR: Cannot open video: ' + video_path)
    sys.exit()

# Get speed (fps)
fps = video.get(cv2.CAP_PROP_FPS)
wait_time_ms = int(1000 / fps) if (fps > 0.0) else int(1000 / 30)

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
    if cv2.waitKey(wait_time_ms) > 0:
        break

# Free resources
video.release()
cv2.destroyAllWindows()