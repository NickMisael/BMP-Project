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

    // If number of args == 2 
    if(argc != 2)
        usage(argv[0]);

    // Var Declaration
    FILE *fp;
    int totalWidth = 0, bytesPerRow = 0;
    int y, x;
    int actualByte = 0, count = 0, readBits = 0, quantBitsSeguidos = 0;
    unsigned char bitAtual = 1, temp = 0;

    // Read file mode
    if((fp = fopen(argv[1], "rb")) == NULL) {
        printf("Failed to open image!\n");
        exit(-1);
    }

    // Fill the BMP Header
    fillHeader(&bf,fp);

    /*printf("Signature   : 0x%x 0x%x\n", bf.signature[0], bf.signature[1]);
    printf("File Size   : %d Bytes\n", bf.imageSize);
    printf("Reserved    : %hd\n", bf.reserved1);
    printf("Reserved    : %hd\n", bf.reserved2);
    printf("Pixel Offset: 0x%x\n", bf.pixelDataOffset);*/

    // If bitsPerPixel not monochromatic
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

    // Set the pointer to offset pixels
    fseek(fp, bf.pixelDataOffset, SEEK_SET);

    // Loop to read and show the image on Terminal
    for(y = 0; y < bf.BIHeader.imageHeight; y++) {
        // Memory allocation for bmp image
        repBMP[y] = (unsigned char *) malloc(totalWidth * sizeof(unsigned char) + 1);
        
        // Fill the bmp image with 0x0
        memset(repBMP[y],'\0', (sizeof(unsigned char) * totalWidth + 1));
        
        // Set readBits when new line is read
        readBits = 0;

        // Loop to read bytes of Row
        for(x = 0; x < bytesPerRow; x++) {
            
            // Read actual byte
            fread(&actualByte,1,1,fp);

            // Read bits of actual byte
            for(count = 7; count >= 0; count--) {
                
                // If Read Bits less than total width of image
                if(readBits < totalWidth) {
                    
                    // If bit checked equal to 1, store + on our matriz
                    if(checkBit(&actualByte, count) == 1) repBMP[y][readBits] = '+';

                    // If bit checked equal to 0, store - on our matriz 
                    else repBMP[y][readBits] = '-';
                }

                readBits++;
            }
        }
    }

    // Show Image on Terminal
    for(y = bf.BIHeader.imageHeight-1; y >= 0; y--) {
        printf("%s\n", repBMP[y]);
    }


    /* RLE Compressor
    // Loop para ler e preencher na memoria com os Bits
    for(y = 0; y < bf.BIHeader.imageHeight; y++){
        readBits = quantBitsSeguidos = 0;

        for(x = 0; x < bytesPerRow; x++){
            fread(&actualByte,1,1,fp);
            for(count = 7; count >= 0; count--){
                if(readBits < totalWidth) {
                    if(count == 7 ) {
                        bitAtual = checkBit(&actualByte, count);
                        quantBitsSeguidos++;
                    } else {
                        if((temp = checkBit(&actualByte, count)) == bitAtual) quantBitsSeguidos++;
                        else {
                            printf("%d:%d, ", quantBitsSeguidos, bitAtual);
                            quantBitsSeguidos = 1;
                            bitAtual = temp;
                        }
                        if(quantBitsSeguidos >= 0xff){
                            return 0;
                        }
                    }
                }
                readBits++;
            }
        }
        printf("%d:%d, ", quantBitsSeguidos, bitAtual);
    }*/
    
    return 0;
}
