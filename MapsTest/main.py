from numpy.distutils.command.config import config
import cv2
import numpy as np
from print_contours import print_contours
from showTools import showTinyContours
from snippets import waitEnter



img = cv2.imread('resources/onePolygon2x.png')
imgGray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
# cv2.imshow('name', imgGray)
# waitEnter()
imgGray = cv2.fastNlMeansDenoising(imgGray, h=5, templateWindowSize=10)
# cv2.imshow('name', imgGray)
# waitEnter()

borders = cv2.Canny(imgGray, 50, 900)
cv2.imshow('name', borders)
waitEnter()

contours, hierarchy = cv2.findContours(borders, cv2.cv.CV_RETR_LIST, cv2.cv.CV_CHAIN_APPROX_TC89_KCOS)
showTinyContours(contours, img.shape)

contours = [ cv2.approxPolyDP(c, 5, False) for c in  contours]
showTinyContours(contours, img.shape)

print_contours(contours, img.shape[:2])