"""
Create single named window to display two images in.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import os

# Configuration
image_path_1 = os.environ['ImagingData'] + '/images/misc/Docks.jpg'
image_path_2 = os.environ['ImagingData'] + '/images/misc/Cologne.jpg'

# -----------------------------------------------------------------------------

# Load image from file
image1 = cv2.imread(image_path_1)
image2 = cv2.imread(image_path_2)

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