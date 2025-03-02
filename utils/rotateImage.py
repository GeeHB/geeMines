# coding=UTF-8
#
#   File     :   rotateImage.py
#
#   Author     :   GeeHB
#
#   Description :   	Simple script for 'rotating' images in a png file
#			Images are stored in a single row in the source file :
#
#				[Img1]
#				[Img2]
#				...
#				[Imgn]
#				
#			The script create a new png file containg two rows : the first with original
#			images and the second with the rotated images : 
#
#				[Img1][rotImg1]
#				[Img2][rotImg2]
#				...
#				[Imgn][rotImgn]
#                   
#   Comment    :   Python 3.xx
#
#
#   Parameters : ./rotateImage.py src={source file} dest={dest file} width={width of single image}
#			height={height of a single image}

import sys
from PIL import Image

#
# App consts
#

# Filenames
FILE_SRC = "../assets-cg/font_default.png"

FILE_DEST_HORZ = "../assets-cg/font_horz.png"  # smaller than orginal ?
FILE_DEST_VERT = "../assets-cg/font_vert.png"

# "image" dimensions
IMG_W = 7
IMG_H = 9

# Source file organisation
HORZ_COUNT = 16
VERT_COUNT = 6

C_WHITE = (255, 255, 255)

#
# Entry point
#
if "__main__" == __name__:

    # Source image
    try:
        src = Image.open(FILE_SRC)
    except FileNotFoundError:
        print(f"Unable to find {FILE_SRC}")
        sys.exit(1)

    # Check source file dimensions
    width, height = src.size
    if width != (CHAR_W * HORZ_COUNT) or height != (CHAR_H * VERT_COUNT):
        print("Image dimension are not valid")
        sys.exit(1)

    # Create dest images
    dest = Image.new('RGB', (HORZ_COUNT * CHAR_H, VERT_COUNT * CHAR_W), C_WHITE)
    
    # Rotate char pixels
    for row in range(VERT_COUNT):
        for y in range(CHAR_H):
            for col in range(HORZ_COUNT):
                for x in range(CHAR_W):
                    
                    pos = (col * CHAR_W + x, row * CHAR_H + y)  # tuple containing pixel position
                    
                    # put the same pixel in the dest vert. file
                    dest.putpixel((col * CHAR_H + y, (row + 1) * CHAR_W - x -1), src.getpixel(pos))


    # Save dest file
    dest.save(FILE_DEST_VERT, 'png')
    
# EOF
