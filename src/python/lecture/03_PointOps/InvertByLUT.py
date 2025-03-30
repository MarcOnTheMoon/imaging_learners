"""
Invert a grayscale image using a LUT (direct pixel access and OpenCV method).

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import time
import numpy as np
import os

# Configuration
image_path = os.environ['ImagingData'] + '/images/misc/Cologne.jpg'

# Load image from file
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

# Prepare LUT
lut = np.empty(256, dtype=np.uint8)
for i in range(256):
    lut[i] = 255 - i

# Clone image and invert using direct pixel access and LUT
rows, cols = image.shape[0:2]
inverted = image.copy()
start_time = time.time()
for y in range(rows):
    for x in range(cols):
        inverted[y][x] = lut[inverted[y][x]]
print('Runtimes for different approches (only one sample execution):')
print("Direct access : {:.2f} s".format(time.time() - start_time))

# Clone image and invert using OpenCV LUT method
inverted = image.copy()
start_time = time.time()
cv2.LUT(image, lut, inverted)
print("OpenCV LUT    : {:.12f} s".format(time.time() - start_time))

# Display images
cv2.imshow('Image', image)
cv2.imshow('Inverted image', inverted)

# Wait for keypress and close all windows
cv2.waitKey(0)
cv2.destroyAllWindows()