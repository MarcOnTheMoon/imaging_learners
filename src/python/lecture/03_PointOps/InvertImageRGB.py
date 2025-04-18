"""
Invert a color image by direct pixel access and numpy.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2025, Marc Hensel
@version: 2025.03.30
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import time
import os

# Configuration
image_path = os.environ['ImagingData'] + '/images/misc/Parrot.jpg'

# Load image from file
image = cv2.imread(image_path)

# Clone image and invert using direct pixel access
rows, cols = image.shape[0:2]
inverted = image.copy()
start_time = time.time()
for y in range(rows):
    for x in range(cols):
        inverted[y][x][0] = 255 - inverted[y][x][0]
        inverted[y][x][1] = 255 - inverted[y][x][1]
        inverted[y][x][2] = 255 - inverted[y][x][2]
print('Runtimes for different approches (only one sample execution):')
print("Direct access   : {:.2f} s".format(time.time() - start_time))

# Clone image and invert using numpy access
inverted = image.copy()
start_time = time.time()
for y in range(rows):
    for x in range(cols):
        inverted.itemset((y, x, 0), 255 - inverted.item(y, x, 0))
        inverted.itemset((y, x, 1), 255 - inverted.item(y, x, 1))
        inverted.itemset((y, x, 2), 255 - inverted.item(y, x, 2))
print("Numpy access    : {:.2f} s".format(time.time() - start_time))

# Clone image and invert using numpy operation
inverted = image.copy()
start_time = time.time()
inverted = 255 - inverted
print("Numpy operation : {:.12f} s".format(time.time() - start_time))

# Display images
cv2.imshow('Image', image)
cv2.imshow('Inverted image', inverted)

# Wait for keypress and close all windows
cv2.waitKey(0)
cv2.destroyAllWindows()