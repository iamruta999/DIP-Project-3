//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 2A
//This code performs dithering digital halftoning on a gray scale image
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc,char *argv[])
{
    //char *argv[] = {"mandrill.raw","ditheredMandrillI4.raw","ditheredMandrillI8.raw","ditheredMandrillGrayI4.raw","ditheredMandrillGrayI8.raw"};
    
    /* Define file pointer and variable */
    FILE *file;
    int width = 512;
    int height = 512;
    
    /* Allocate image data array */
    unsigned char originalImage[height][width];
    
    /* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(originalImage, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    //Normalize the image between range 0 to 1
    double normalisedImage[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            normalisedImage[row][column] = double(originalImage[row][column])/double(255);
        }
    }

    //Create index matrix, threshold matrix and dithered image for I4
    unsigned char ditheredImageI4[height][width];
    int N = 4;
    int I4[4][4] = { {5,9,6,10}, {13,1,14,2}, {7,11,4,8}, {15,3,12,0} };
    double thresholdOfI4[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            thresholdOfI4[i][j] = double(I4[i][j] + 0.5)/double(N * N);
            
        }
    }
    
    // PART 1 WITH ONLY ONE THRESHOLD
    //Apply thresholds to the normalised image
    for (int row = 0; row < width; row++)
    {
        for (int column = 0; column < height; column++)
        {
            if (normalisedImage[row][column] > thresholdOfI4[row % N][column % N])
                ditheredImageI4[row][column] = 255;
            else
                ditheredImageI4[row][column] = 0;
        }
    }
    //Write the dithered image for I4
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ditheredImageI4, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    //Create index matrix, threshold matrix and dithered image for I8
    unsigned char ditheredImageI8[height][width];
    N = 8;
    int I8[8][8] = { {21,37,25,41,22,38,26,42}, {53,5,57,9,54,6,58,10},
        {29,45,17,33,30,46,18,34}, {61,13,49,1,62,14,50,2},
        {23,39,27,43,20,36,24,40}, {55,7,59,11,52,4,56,8},
        {31,47,19,35,28,44,16,32}, {63,15,51,3,60,12,48,0} };
    double thresholdOfI8[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            thresholdOfI8[i][j] = double(I8[i][j] + 0.5)/double(N * N);
        }
    }
    //Apply thresholds to the normalised image
    for (int row = 0; row < width; row++)
    {
        for (int column = 0; column < height; column++)
        {
            if (normalisedImage[row][column] > thresholdOfI8[row % N][column % N])
                ditheredImageI8[row][column] = 255;
            else
                ditheredImageI8[row][column] = 0;
        }
    }
    //Write the dithered image for I8
    if (!(file=fopen(argv[3],"wb")))
    {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(ditheredImageI8, sizeof(unsigned char), height*width, file);
	fclose(file);
     
    // PART 2 WITH THREE THRESHOLDS
    //Apply thresholds to the normalised image
    unsigned char ditheredImageGrayI4[height][width];
    double threshold1, threshold2, threshold3;
    N = 4;

    //Apply 3 thresholds to the normalised image
    for (int row = 0; row < width; row++)
    {
        for (int column = 0; column < height; column++)
        {
            threshold2 = thresholdOfI4[row % N][column % N];
            threshold1 = 0.5 * threshold2;
            threshold3 = 1.5 * threshold2;
            
            if (normalisedImage[row][column] < threshold1)
                ditheredImageGrayI4[row][column] = 0;
            else if ((normalisedImage[row][column] >= threshold1) && (normalisedImage[row][column] < threshold2))
                ditheredImageGrayI4[row][column] = 85;
            else if ((normalisedImage[row][column] >= threshold2) && (normalisedImage[row][column] < threshold3))
                ditheredImageGrayI4[row][column] = 170;
            else 
                ditheredImageGrayI4[row][column] = 255;
        }
    }
    
    //Write the Gray dithered image for I4
    if (!(file=fopen(argv[4],"wb")))
    {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(ditheredImageGrayI4, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    //Apply thresholds to the normalised image
    unsigned char ditheredImageGrayI8[height][width];
    N = 8;
    
    //Apply 3 thresholds to the normalised image
    for (int row = 0; row < width; row++)
    {
        for (int column = 0; column < height; column++)
        {
            threshold2 = thresholdOfI8[row % N][column % N];
            threshold1 = 0.5 * threshold2;
            threshold3 = 1.5 * threshold2;
            
            if (normalisedImage[row][column] < threshold1)
                ditheredImageGrayI8[row][column] = 0;
            else if ((normalisedImage[row][column] >= threshold1) && (normalisedImage[row][column] < threshold2))
                ditheredImageGrayI8[row][column] = 85;
            else if ((normalisedImage[row][column] >= threshold2) && (normalisedImage[row][column] < threshold3))
                ditheredImageGrayI8[row][column] = 170;
            else
                ditheredImageGrayI8[row][column] = 255;
        }
    }
    
    //Write the Gray dithered image for I8
    if (!(file=fopen(argv[5],"wb")))
    {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(ditheredImageGrayI8, sizeof(unsigned char), height*width, file);
	fclose(file);
    

    
    
    
    return 0;
}
