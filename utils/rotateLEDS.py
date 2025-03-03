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
#

import sys
from PIL import Image

#
# App consts
#

# Filenames
FILE_SRC = "./base_leds.png"
FILE_DEST = "./leds.png"

# "image" dimensions
IMG_W = 13
IMG_H = 23

C_BKCOLOUR = (0, 0, 0) # noir

#
# Entry point
#
if "__main__" == __name__:

    # Ouverture de l'image'
    try:
        base = Image.open(FILE_SRC)
    except FileNotFoundError:
        print(f"Impossible d'ouvrir le fichier {FILE_SRC}")
        sys.exit(1)

    src = base.convert('RGB')   # ???

    # Petite verification des dimensions de l'image
    width, height = src.size
    if width != IMG_W or 0 != (height % IMG_H):
        print("Les dimensions de l'image sont incorrectes")
        sys.exit(1)

    # Creation de l'image destination
    dest = Image.new('RGB', (IMG_W + IMG_H,  height), C_BKCOLOUR)
    imgCount = int(height / IMG_H)
    # print(f"{imgCount} images à transformer")

    # Rotation des "petites" images
    for row in range(imgCount):
        for y in range(IMG_H):
            for x in range(IMG_W):
                pos = (x, row * IMG_H + y)  # position du point
                pixel = src.getpixel(pos)

                # Copie du pixel
                dest.putpixel(pos, pixel)

                # Rotation dans la seconde colonne
                dest.putpixel((y + IMG_W, (row + 1) * IMG_W - x -1), pixel)

    # Sauvegarde du fichier
    dest.save(FILE_DEST, 'png')

# EOF
