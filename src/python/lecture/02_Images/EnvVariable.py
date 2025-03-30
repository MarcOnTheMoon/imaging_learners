"""
Access image data directory using environment variable 'ImagingData'.

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
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

# Display image in named window
cv2.imshow('Image [Press any key to quit]', image)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()