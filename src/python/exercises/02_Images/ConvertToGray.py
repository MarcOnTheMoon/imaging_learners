"""
Convert color to gray image.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import os

# Configuration
image_path = os.environ['ImagingData'] + '/images/misc/Docks.jpg'

# Load image from file
color_image = cv2.imread(image_path)

# Display image in named window and wait for keypress
cv2.imshow('Image', color_image)
cv2.waitKey(0)

# Convert image to grayscale and display in window
gray_image = cv2.cvtColor(color_image, cv2.COLOR_BGR2GRAY)
cv2.imshow('Image', gray_image)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()