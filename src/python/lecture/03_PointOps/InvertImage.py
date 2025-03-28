"""
Invert a grayscale image by direct pixel access and numpy.

@author: Marc Hensel
@contact: http://www.haw-hamburg.de/marc-hensel

@copyright: 2024, Marc Hensel
@version: 2024.10.11
@license: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
"""

import cv2
import time

import sys, os
projectDir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(projectDir + '/../..')

from ImageData import ImageData

# -----------------------------------------------------------------------------

# Configuration
inputImage = 'misc/Cologne.jpg'

# -----------------------------------------------------------------------------

# Load image from file
inputImagePath =  ImageData.getImagePath(inputImage)
image = cv2.imread(inputImagePath, cv2.IMREAD_GRAYSCALE)

# Clone image and invert using direct pixel access
rows, cols = image.shape[0:2]
inverted = image.copy()
startTime = time.time()
for y in range(rows):
    for x in range(cols):
        inverted[y][x] = 255 - inverted[y][x]
print('Runtimes for different approches (only one sample execution):')
print("Direct access   : {:.2f} s".format(time.time() - startTime))

# Clone image and invert using numpy access
inverted = image.copy()
startTime = time.time()
for y in range(rows):
    for x in range(cols):
        inverted.itemset((y, x), 255 - inverted.item(y, x))
print("Numpy access    : {:.2f} s".format(time.time() - startTime))

# Clone image and invert using numpy operation
inverted = image.copy()
startTime = time.time()
inverted = 255 - inverted
print("Numpy operation : {:.12f} s".format(time.time() - startTime))

# Display images
cv2.imshow('Image', image)
cv2.imshow('Inverted image', inverted)

# Wait for keypress and close all windows
cv2.waitKey(0)
cv2.destroyAllWindows()