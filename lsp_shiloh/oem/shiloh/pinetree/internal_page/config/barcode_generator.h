#ifndef BARCODE_GENERATOR_H
#define BARCODE_GENERATOR_H

unsigned char* barcodeGen(const char * str, int barWidthInPixel, int height,int * width);
unsigned char * graphGen(int width, int height, int lineWidth, int gridHeight, int axisNumber, int current, int total,  int fullThreshold);
//int fillMemory(unsigned char *img, unsigned char *dest, unsigned long imgSizeX, unsigned long  imgSizeY, unsigned long  targetOffsetX, unsigned long targetOffsetY, unsigned long targetStride);

#endif
