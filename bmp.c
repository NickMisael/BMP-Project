#pragma once
#include "bmp.h"
#include <stdio.h>

void fillHeader(BFHeader *bf, FILE *fp) {
    // BitMapFileHeader
    fread(&bf->signature, 2, 1,fp);
    fread(&bf->imageSize, 4, 1,fp);
    fread(&bf->reserved1, 2, 1,fp);
    fread(&bf->reserved2, 2, 1,fp);
    fread(&bf->pixelDataOffset, 4, 1,fp);

    //BitMapInfoHeader
    fread(&bf->BIHeader.headerSize, 4, 1,fp);
    fread(&bf->BIHeader.imageWidth, 4, 1,fp);
    fread(&bf->BIHeader.imageHeight, 4, 1,fp);
    fread(&bf->BIHeader.planes, 2, 1,fp);
    fread(&bf->BIHeader.bitsPerPixel, 2, 1,fp);
    fread(&bf->BIHeader.compression, 4, 1,fp);
    fread(&bf->BIHeader.imageRawSize, 4, 1,fp);
    fread(&bf->BIHeader.horizontalResolution, 4, 1,fp);
    fread(&bf->BIHeader.verticalResolution, 4, 1,fp);
    fread(&bf->BIHeader.colorPallete, 4, 1,fp);
    fread(&bf->BIHeader.importantColors, 4, 1, fp);
}

int ceilling(int x, int y) {
    // Condicao ternaria
    return x%y == 0 ? x/y : (x/y)+1;
}

int checkBit(unsigned char *ch, int position) {
    unsigned char result;

    // Assembly inline on CodeBlocks
    asm(
        "movl (%2), %%ebx;"
        "movl (%%eax), %%eax;"
        "btl  %%ebx, %%eax;" // bt - pega um byte qualquer e testa o bit
        "jnc  zero;" // JC - jump if carry
        "jmp  um;"
        "um:"
            "xorl %%eax, %%eax;"
            "incl %%eax;"
            "jmp fim;"
        "zero:"
            "xorl %%eax, %%eax;"
        "fim:"
            "movb %%al, %0;"

        : "=r" (result)
        : "r" (ch), "r" (&position)
        : "%eax", "%ebx"
    );

    return result;
}
