"""
Calculate and display histogram of a grayscale image.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.31
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import os
import matplotlib.pyplot as plt

# Configuration
image_path = os.environ['ImagingData'] + '/images/misc/Cologne.jpg'

# Load image from file
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

# Calculate the histogram
hist = cv2.calcHist(images=[image], channels=[0], mask=None, histSize=[256], ranges=[0,256])

# Plot histogram
plt.title('Histogram')
plt.xlabel('Gray value')
plt.ylabel('Pixel count')
plt.bar(range(256), hist[:,0], width=1.0)
plt.xlim([0, 255])
plt.show()

# Display image
cv2.imshow('Image', image)

# Wait for keypress and close all windows
cv2.waitKey(0)
cv2.destroyAllWindows()