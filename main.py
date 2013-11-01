import cv2
import numpy as np
from snippets import waitEnter

img = cv2.imread('resources/polygonsDrawed.png')
imgGray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

edges = cv2.Canny(img, 1, 2)
cv2.imshow('name', edges)
waitEnter()
cv2.destroyWindow('name')

lines = cv2.HoughLinesP(edges, 1, np.pi, 150, minLineLength=20, maxLineGap=50)

edges = cv2.cvtColor(edges, cv2.COLOR_GRAY2RGB)
for x1, y1, x2, y2 in lines[0]:
    cv2.line(edges, (x1, y1), (x2, y2), (0, 255, 0), 2)

cv2.imshow('houghlines', edges)
waitEnter()
cv2.destroyAllWindows()