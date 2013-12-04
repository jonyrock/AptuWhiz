import cv2
import numpy as np
from snippets import waitEnter
import random


img = cv2.imread('resources/onePolygon2x.png')
imgGray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
# cv2.imshow('name', imgGray)
# waitEnter()
imgGray = cv2.fastNlMeansDenoising(imgGray, h=5, templateWindowSize=10)
# cv2.imshow('name', imgGray)
# waitEnter()

edges = cv2.Canny(imgGray, 200, 100)
cv2.imshow('name', edges)
waitEnter()

contours, hierarchy = cv2.findContours(edges, cv2.cv.CV_RETR_LIST, cv2.cv.CV_CHAIN_APPROX_TC89_KCOS)

contoursImage = np.zeros(img.shape, np.uint8)



for i in range(0, len(contours)):
    color = (random.randint(20,255),random.randint(20,255), random.randint(20,255))
    cv2.drawContours(contoursImage, contours, i, color)
cv2.imshow('name', contoursImage)
waitEnter()

cv2.destroyAllWindows()
