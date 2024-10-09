"""
Create single named window to display two images in.

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
inputImage1 = 'misc/Docks.jpg'
inputImage2 = 'misc/Cologne.jpg'

# -----------------------------------------------------------------------------

# Load image from file
inputImagePath1 =  ImageData.getImagePath(inputImage1)
inputImagePath2 =  ImageData.getImagePath(inputImage2)
image1 = cv2.imread(inputImagePath1)
image2 = cv2.imread(inputImagePath2)

# Display named window and wait for keypress
cv2.namedWindow('Image')
cv2.waitKey(0)

# Display image in named window and wait for keypress
cv2.imshow('Image', image1)
cv2.waitKey(0)

# Display another image in named window and wait for keypress
cv2.imshow('Image', image2)
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()