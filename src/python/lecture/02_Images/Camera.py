"""
Display camera stream.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2024, Marc Hensel
@version: 2024.10.09
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2

# -----------------------------------------------------------------------------

# Configuration
waitTimeMs = int(1000.0 / 30.0)     # Set speed to 30 fps

# -----------------------------------------------------------------------------

# Open the first connected camera found
camera = cv2.VideoCapture(0)

if not camera.isOpened():
    print('ERROR: Cannot open camera')
    exit()

# Loop through frames
while True:
    # Get current frame from camera
    ret, frame = camera.read()
        
    # Display frame in named window
    cv2.imshow('Camera [Press any key to quit]', frame)

    # Wait (exit loop on key press)
    if cv2.waitKey(waitTimeMs) > 0:
        break

# Free resources
camera.release()
cv2.destroyAllWindows()