"""
Convert color to gray image.

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
inputImage = 'misc/Docks.jpg'

# -----------------------------------------------------------------------------

# Load image from file
inputImagePath =  ImageData.getImagePath(inputImage)
colorImage = cv2.imread(inputImagePath)

# Display image in named window and wait for keypress
cv2.imshow('Image', colorImage)
cv2.waitKey(0)

# Convert image to grayscale and display in window
grayImage = cv2.cvtColor(colorImage, cv2.COLOR_BGR2GRAY)
cv2.imshow('Image', grayImage)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()