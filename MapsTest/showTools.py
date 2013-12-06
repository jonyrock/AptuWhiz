import cv2
import numpy as np
import random
from snippets import waitEnter


def showTinyContours(contours, shape):
    contoursImage = np.zeros(shape, np.uint8)
    for i in range(0, len(contours)):
        color = (random.randint(20,255),random.randint(20,255), random.randint(20,255))
        cv2.drawContours(contoursImage, contours, i, color)
    
    cv2.imshow('name', contoursImage)
    waitEnter()