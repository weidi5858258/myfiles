import cv2
import numpy as np

img = cv2.imread("/root/Pictures/500px/images/215139415.jpg")
cv2.imshow("src", img)
print(img.shape)
print(img.size)
print(img.dtype)
print(img)
cv2.waitKey()
