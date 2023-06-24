#pragma once
#include <stdio.h>

typedef struct {
    unsigned int   headerSize;           // Offset 0E - It need to equal 40 (0x28)
    unsigned int   imageWidth;           // Offset 12
    unsigned int   imageHeight;          // Offset 16
    unsigned short planes;               // Offset 1A - It need to equal 1 (0x01)
    short          bitsPerPixel;         // Offset 1C - It need to equal 1 (0x01) - monochromatic
    unsigned int   compression;          // Offset 1E
    unsigned int   imageRawSize;         // Offset 22
    unsigned int   horizontalResolution; // Offset 26
    unsigned int   verticalResolution;   // Offset 2A
    unsigned int   colorPallete;         // Offset 2E
    unsigned int   importantColors;      // Offset 32
} BitMapInfoHeader;

typedef struct {
    unsigned char signature[2];    // Offset 00
    unsigned int  imageSize;       // Offset 02
    short         reserved1;       // Offset 06
    short         reserved2;       // Offset 08
    unsigned int  pixelDataOffset; // Offset 0A
    BitMapInfoHeader BIHeader;
} BFHeader;

void fillHeader(BFHeader *, FILE *);
int ceilling(int, int);
int checkBit(unsigned char *, int);
