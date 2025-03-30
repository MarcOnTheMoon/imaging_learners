"""
Pause camera stream and save frame to file.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2

# Configuration
wait_time_ms = int(1000.0 / 30.0)     # Set speed to 30 fps
image_write_path = 'Frame.jpg'

# Open the first connected camera found
camera = cv2.VideoCapture(0)

if not camera.isOpened():
    print('ERROR: Cannot open camera')
    exit()

# Display help on console and wait for keypress
print('\nCamera controls:\nP    : Pause\nS    : Save frame\n<ESC>: Quit\n')
    
# Loop through frames
is_paused = False
while True:
    # Get and display current frame from camera
    if not is_paused:
        ret, frame = camera.read()
        cv2.imshow('Camera [Press ESC to quit]', frame)

    # Wait (react to key presses)
    key_pressed = cv2.waitKey(wait_time_ms)
    if key_pressed in [ord('p'), ord('P')]:      # Pause
        is_paused = not is_paused
    elif key_pressed in [ord('s'), ord('S')]:    # Save frame to file
        cv2.imwrite(image_write_path, frame)
    elif key_pressed == 27:                      # Exit when ESC (= 27) pressed
        break

# Free resources
camera.release()
cv2.destroyAllWindows()