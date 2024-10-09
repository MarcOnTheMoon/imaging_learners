"""
Open an image from file and display it.

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
image = cv2.imread(inputImagePath, cv2.IMREAD_ANYCOLOR)

# Display image in named window
cv2.imshow('Image [Press any key to quit]', image)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()