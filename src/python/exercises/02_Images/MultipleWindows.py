"""
Open image as color and gray image and display in separate windows.

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
gray_image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

# Display images in named windows
cv2.imshow('Color image', color_image)
cv2.imshow('Grayscale image', gray_image)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()