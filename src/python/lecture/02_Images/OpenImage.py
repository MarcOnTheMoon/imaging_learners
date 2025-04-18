"""
Open an image from file and display it.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2

# Load image from file
image = cv2.imread('D:/Docks.jpg', cv2.IMREAD_ANYCOLOR)

# Display image in named window
cv2.imshow('Image [Press any key to quit]', image)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()