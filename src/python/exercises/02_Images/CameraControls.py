"""
Pause camera stream and save frame to file.

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
imageWriteFile = 'Frame.jpg'

# -----------------------------------------------------------------------------

# Open the first connected camera found
camera = cv2.VideoCapture(0)

if not camera.isOpened():
    print('ERROR: Cannot open camera')
    exit()

# Display help on console and wait for keypress
print('\nCamera controls:\nP    : Pause\nS    : Save frame\n<ESC>: Quit\n')
    
# Loop through frames
isPaused = False
while True:
    # Get and display current frame from camera
    if not isPaused:
        ret, frame = camera.read()
        cv2.imshow('Camera [Press ESC to quit]', frame)

    # Wait (react to key presses)
    keyPressed = cv2.waitKey(waitTimeMs)
    if keyPressed in [ord('p'), ord('P')]:      # Pause
        isPaused = not isPaused
    elif keyPressed in [ord('s'), ord('S')]:    # Save frame to file
        cv2.imwrite(imageWriteFile, frame)
    elif keyPressed == 27:                      # Exit when ESC (= 27) pressed
        break

# Free resources
camera.release()
cv2.destroyAllWindows()