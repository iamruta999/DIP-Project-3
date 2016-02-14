//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 2C
//This code performs scalar color halftoning on a color image
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc,char *argv[])
{
    //char *argv[] = {"Sailboat.raw","ScalarColorHalftonedSailboat.raw"};
    
    /* Define file pointer and variable */
    FILE *file;
    int width = 512;
    int height = 512;
    int bytesPerPixel = 3;
    
    /* Allocate image data array */
    unsigned char originalImage[height][width][bytesPerPixel];
    
    /* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(originalImage, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    //Scalar color Halftoning 
    unsigned char floydErrorImage[height][width][bytesPerPixel];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            floydErrorImage[row][column][0] = 255-originalImage[row][column][0];//Convert red to cyan
            floydErrorImage[row][column][1] = 255-originalImage[row][column][1];//Convert green to magenta
            floydErrorImage[row][column][2] = 255-originalImage[row][column][2];//Convert blue to yellow
        }
    }
    double FSerrDiff1 = double(7)/double(16);
    double FSerrDiff2 = double(3)/double(16);
    double FSerrDiff3 = double(5)/double(16);
    double FSerrDiff4 = double(1)/double(16);
    int quantError, oldValue;
    double threshold = 128;
    //Thresholding at 50% and then using Floyd-Steinberg diffusion method
    //FOR RED i.e. CYAN
    for (int row = 0; row < height; row++)
    {
        if(row % 2 == 0)//For even rows go from left to right
        {
        for (int column = 0; column < width; column++)
        {
            if (floydErrorImage[row][column][0] < threshold)
            {
                oldValue = floydErrorImage[row][column][0];
                floydErrorImage[row][column][0] = 0;
                quantError = oldValue - floydErrorImage[row][column][0];//i.e. the old value itself
            }
            else
            {
                oldValue = floydErrorImage[row][column][0];
                floydErrorImage[row][column][0] = 255;
                quantError = oldValue - floydErrorImage[row][column][0];//i.e. old value - 255
            }
            floydErrorImage[row][column+1][0] = floydErrorImage[row][column+1][0] + (FSerrDiff1 * quantError);
            floydErrorImage[row+1][column-1][0] = floydErrorImage[row+1][column-1][0] + (FSerrDiff2 * quantError);
            floydErrorImage[row+1][column][0] = floydErrorImage[row+1][column][0] + (FSerrDiff3 * quantError);
            floydErrorImage[row+1][column+1][0] = floydErrorImage[row+1][column+1][0] + (FSerrDiff4 * quantError);
        }
        }
        else//For odd rows go from right to left
        {
            for (int column = width-1; column >= 0; column--)
            {
                if (floydErrorImage[row][column][0] < threshold)
                {
                    oldValue = floydErrorImage[row][column][0];
                    floydErrorImage[row][column][0] = 0;
                    quantError = oldValue - floydErrorImage[row][column][0];//i.e. the old value itself
                }
                else
                {
                    oldValue = floydErrorImage[row][column][0];
                    floydErrorImage[row][column][0] = 255;
                    quantError = oldValue - floydErrorImage[row][column][0];//i.e. old value - 255
                }
                floydErrorImage[row][column-1][0] = floydErrorImage[row][column-1][0] + (FSerrDiff1 * quantError);
                floydErrorImage[row+1][column+1][0] = floydErrorImage[row+1][column+1][0] + (FSerrDiff2 * quantError);
                floydErrorImage[row+1][column][0] = floydErrorImage[row+1][column][0] + (FSerrDiff3 * quantError);
                floydErrorImage[row+1][column-1][0] = floydErrorImage[row+1][column-1][0] + (FSerrDiff4 * quantError);
            }
        }
        
    }
    //FOR GREEN i.e. MAGENTA
    for (int row = 0; row < height; row++)
    {
        if(row % 2 == 0)//For even row go from left to right
        {
        for (int column = 0; column < width; column++)
        {
            if (floydErrorImage[row][column][1] < threshold)
            {
                oldValue = floydErrorImage[row][column][1];
                floydErrorImage[row][column][1] = 0;
                quantError = oldValue - floydErrorImage[row][column][1];//i.e. the old value itself
            }
            else
            {
                oldValue = floydErrorImage[row][column][1];
                floydErrorImage[row][column][1] = 255;
                quantError = oldValue - floydErrorImage[row][column][1];//i.e. old value - 255
            }
            floydErrorImage[row][column+1][1] = floydErrorImage[row][column+1][1] + (FSerrDiff1 * quantError);
            floydErrorImage[row+1][column-1][1] = floydErrorImage[row+1][column-1][1] + (FSerrDiff2 * quantError);
            floydErrorImage[row+1][column][1] = floydErrorImage[row+1][column][1] + (FSerrDiff3 * quantError);
            floydErrorImage[row+1][column+1][1] = floydErrorImage[row+1][column+1][1] + (FSerrDiff4 * quantError);
        }
        }
        else//For odd row go from right to left
        {
            for (int column = width-1; column >= 0; column--)
            {
                if (floydErrorImage[row][column][1] < threshold)
                {
                    oldValue = floydErrorImage[row][column][1];
                    floydErrorImage[row][column][1] = 0;
                    quantError = oldValue - floydErrorImage[row][column][1];//i.e. the old value itself
                }
                else
                {
                    oldValue = floydErrorImage[row][column][1];
                    floydErrorImage[row][column][1] = 255;
                    quantError = oldValue - floydErrorImage[row][column][1];//i.e. old value - 255
                }
                floydErrorImage[row][column-1][1] = floydErrorImage[row][column-1][1] + (FSerrDiff1 * quantError);
                floydErrorImage[row+1][column+1][1] = floydErrorImage[row+1][column+1][1] + (FSerrDiff2 * quantError);
                floydErrorImage[row+1][column][1] = floydErrorImage[row+1][column][1] + (FSerrDiff3 * quantError);
                floydErrorImage[row+1][column-1][1] = floydErrorImage[row+1][column-1][1] + (FSerrDiff4 * quantError);
            }
        }
        
    }
    //FOR BLUE i.e. YELLOW
    for (int row = 0; row < height; row++)
    {
        if(row % 2 == 0)//For even row go from right to left
        {
        for (int column = 0; column < width; column++)
        {
            if (floydErrorImage[row][column][2] < threshold)
            {
                oldValue = floydErrorImage[row][column][2];
                floydErrorImage[row][column][2] = 0;
                quantError = oldValue - floydErrorImage[row][column][2];//i.e. the old value itself
            }
            else
            {
                oldValue = floydErrorImage[row][column][2];
                floydErrorImage[row][column][2] = 255;
                quantError = oldValue - floydErrorImage[row][column][2];//i.e. old value - 255
            }
            floydErrorImage[row][column+1][2] = floydErrorImage[row][column+1][2] + (FSerrDiff1 * quantError);
            floydErrorImage[row+1][column-1][2] = floydErrorImage[row+1][column-1][2] + (FSerrDiff2 * quantError);
            floydErrorImage[row+1][column][2] = floydErrorImage[row+1][column][2] + (FSerrDiff3 * quantError);
            floydErrorImage[row+1][column+1][2] = floydErrorImage[row+1][column+1][2] + (FSerrDiff4 * quantError);
        }
        }
        else//For odd rows go from right to left
        {
            for (int column = width-1; column >= 0; column--)
            {
                if (floydErrorImage[row][column][2] < threshold)
                {
                    oldValue = floydErrorImage[row][column][2];
                    floydErrorImage[row][column][2] = 0;
                    quantError = oldValue - floydErrorImage[row][column][2];//i.e. the old value itself
                }
                else
                {
                    oldValue = floydErrorImage[row][column][2];
                    floydErrorImage[row][column][2] = 255;
                    quantError = oldValue - floydErrorImage[row][column][2];//i.e. old value - 255
                }
                floydErrorImage[row][column-1][2] = floydErrorImage[row][column-1][2] + (FSerrDiff1 * quantError);
                floydErrorImage[row+1][column+1][2] = floydErrorImage[row+1][column+1][2] + (FSerrDiff2 * quantError);
                floydErrorImage[row+1][column][2] = floydErrorImage[row+1][column][2] + (FSerrDiff3 * quantError);
                floydErrorImage[row+1][column-1][2] = floydErrorImage[row+1][column-1][2] + (FSerrDiff4 * quantError);
            }

        }
    }
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            floydErrorImage[row][column][0] = 255-floydErrorImage[row][column][0];
            floydErrorImage[row][column][1] = 255-floydErrorImage[row][column][1];
            floydErrorImage[row][column][2] = 255-floydErrorImage[row][column][2];
        }
    }

    //Write the scalar color half-toned image 
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(floydErrorImage, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);

    return 0;
}
