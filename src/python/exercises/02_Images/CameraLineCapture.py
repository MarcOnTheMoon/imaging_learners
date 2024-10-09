"""
Capture image line by line from camera stream.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2024, Marc Hensel
@version: 2024.10.09
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2

# -----------------------------------------------------------------------------

# Configuration
imageWriteFile = 'OutImage.jpg'

# -----------------------------------------------------------------------------

# Open the first connected camera found
camera = cv2.VideoCapture(0)

if not camera.isOpened():
    print('ERROR: Cannot open camera')
    exit()

# Get frame properties
height = int(camera.get(cv2.CAP_PROP_FRAME_HEIGHT))

# Init target image with grayscaled camera frame
ret, frame = camera.read()
image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)

# Loop through image rows
for row in range(height):
    # Get current frame from camera
    ret, frame = camera.read()

    # Copy gray frame to "preview" part of target image
    grayFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    grayFrame = cv2.cvtColor(grayFrame, cv2.COLOR_GRAY2BGR)
    image[row:, :, :] = grayFrame[row:, :, :]

    # Copy current row to target image and display image
    image[row, :, :] = frame[row, :, :]
    cv2.imshow('Camera [Press ESC to quit]', image)

    # Wait (exit loop on key press)
    if cv2.waitKey(1) > 0:
        break

# Save image to file
cv2.imwrite(imageWriteFile, image)

# Free resources
camera.release()
cv2.destroyAllWindows()