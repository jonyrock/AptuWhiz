import cv2
import numpy as np
from snippets import waitEnter

# img = cv2.imread('resources/polygonsDrawedBold.png')
# img = cv2.imread('resources/map_s.png')
img = cv2.imread('resources/polygonsDrawedThin.png')
imgGray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

edges = cv2.Canny(img, 150, 250)
cv2.imshow('name', edges)
waitEnter()
cv2.destroyWindow('name')

edgesToDraw = cv2.cvtColor(edges, cv2.COLOR_GRAY2RGB)

lines = cv2.HoughLinesP(edges.copy(), 1, np.pi / 180, 3, minLineLength=30, maxLineGap=10)
for x1, y1, x2, y2 in lines[0]:
    cv2.line(edgesToDraw, (x1, y1), (x2, y2), (0, 255, 0), 2)

cv2.imshow('houghlines', edgesToDraw)
waitEnter()

cv2.destroyAllWindows()