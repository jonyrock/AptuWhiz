import cv2 


def waitEnter():
    while True: 
        key = cv2.waitKey()
        if key == 1048586:
            break
            
            
