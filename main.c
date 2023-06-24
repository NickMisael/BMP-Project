#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

BFHeader bf;

void usage(char *prog_name) {
    printf("Usage: %s <filename>\n", prog_name);
    exit(0);
}

unsigned char **repBMP; // Representacao do BMP na memoria

int main(int argc, char **argv) {
    if(argc != 2)
        usage(argv[0]);

    FILE *fp;
    int totalWidth = 0, bytesPerRow = 0;
    int y, x;
    int actualByte = 0, count = 0, readBits = 0, quantBitsSeguidos = 0;
    unsigned char bitAtual = 1, temp = 0;

    if((fp = fopen(argv[1], "rb")) == NULL) {
        printf("Failed to open image!\n");
        exit(-1);
    }

    fillHeader(&bf,fp);

    /*printf("Signature   : 0x%x 0x%x\n", bf.signature[0], bf.signature[1]);
    printf("File Size   : %d Bytes\n", bf.imageSize);
    printf("Reserved    : %hd\n", bf.reserved1);
    printf("Reserved    : %hd\n", bf.reserved2);
    printf("Pixel Offset: 0x%x\n", bf.pixelDataOffset);*/

    if(bf.BIHeader.bitsPerPixel != 1){
        printf("Error: Only work with monochromatic images!");
        fclose(fp);
        return 0;
    }

    // BMP Console Reader
    totalWidth = bf.BIHeader.bitsPerPixel * bf.BIHeader.imageWidth;
    bytesPerRow = ceilling(totalWidth, 32) * 4;
    //bytesPerRow = floor(totalWidth+31,32) * 4;
    repBMP = (unsigned char **) malloc(bf.BIHeader.imageHeight * sizeof(unsigned char *));

    // Offset dos pixels da imagem
    fseek(fp, bf.pixelDataOffset, SEEK_SET);

    // Loop para ler e preencher na memoria com os Bits
    for(y = 0; y < bf.BIHeader.imageHeight; y++) {
        repBMP[y] = (unsigned char *) malloc(totalWidth * sizeof(unsigned char) + 1);
        memset(repBMP[y],'\0', (sizeof(unsigned char) * totalWidth + 1));
        readBits = 0;

        for(x = 0; x < bytesPerRow; x++) {
            fread(&actualByte,1,1,fp);
            for(count = 7; count >= 0; count--) {
                if(readBits < totalWidth) {
                    if(checkBit(&actualByte, count) == 1) repBMP[y][readBits] = '1';
                    else repBMP[y][readBits] = '0';
                }
                readBits++;
            }
        }
    }

    // Show Image on Terminal
    for(y = bf.BIHeader.imageHeight-1; y >= 0; y--) {
            printf("%s\n", repBMP[y]);
    }

    return 0;
}
