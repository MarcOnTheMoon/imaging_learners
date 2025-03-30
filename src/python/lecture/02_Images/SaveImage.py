"""
Open an image as grayscale image and save it to a file.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2

# Load image from file
image = cv2.imread('D:/Docks.jpg', cv2.IMREAD_GRAYSCALE)

# Display image in named window
cv2.imshow('Image [Press any key to quit]', image)

# Save image to file
cv2.imwrite('D:/Docks_gray.jpg', image)

# Wait for keypress
cv2.waitKey(0)

# Close all named windows
cv2.destroyAllWindows()