# coding=UTF-8
#
#   File     :   genSmileys.py
#
#   Author     :   GeeHB
#
#   Description :   Simple script for 'rotating' images in a png file
#			        Images are stored in a single col in the source file :
#
#				[Img1]
#				[Img2]
#				...
#				[Imgn]
#
#			The script create a new png file containg two cols : the first with original
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

import sys
from PIL import Image

#
# App consts
#

# Filenames
FILE_SRC = "./base_smileys.png"
FILE_DEST = "./smileys.png"

# "image" dimensions
IMG_W = 24
IMG_H = 24

# new dims
IMG_NEW_W = 23

C_BORDERCOLOUR = (128,128,128)
C_BKCOLOUR = (192, 192, 192) # COCOCO

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

    imgCount = int(height / IMG_H)
    print(f"Le fichier '{FILE_SRC} contient {imgCount} vignettes")

    # Creation de l'image destination
    dest = Image.new('RGB', (2 * IMG_NEW_W,  (imgCount - 1) * IMG_NEW_W), C_BKCOLOUR)

    # Rotation des vignettes
    for row in range(1, imgCount):
        for y in range(1,IMG_NEW_W-1):
            for x in range(1,IMG_NEW_W-1):
                pos = (x + 1, row * IMG_H + y + 1)  # position du point
                pixel = src.getpixel(pos)
                if pixel == C_BORDERCOLOUR:
                    pixel = C_BKCOLOUR

                # Copie du pixel
                posDest = (x, (row - 1) * IMG_NEW_W + y)
                dest.putpixel(posDest, pixel)

                # Rotation dans la seconde colonne
                dest.putpixel((y + IMG_NEW_W, row * IMG_NEW_W - x -1), pixel)

    # Sauvegarde du fichier
    dest.save(FILE_DEST, 'png')

    print(f"Génération du fichier '{FILE_DEST}' terminée")

# EOF
