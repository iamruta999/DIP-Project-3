//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 2B
//This code performs error diffusion digital halftoning
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc,char *argv[])
{
    //char *argv[] = {"mandrill.raw","FloydErrorMandrill.raw","JJNErrorMandrill.raw","StuckiErrorMandrill.raw"};
    
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
    //Normalise the image in range 0 to 1
    double normalisedImage[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            normalisedImage[row][column] = double(originalImage[row][column])/double(255);
        }
    }
    //FLOYD STEINBERG
    unsigned char floydErrorImage[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            floydErrorImage[row][column] = originalImage[row][column];
        }
    }
    double FSerrDiff1 = double(7)/double(16);
    double FSerrDiff2 = double(3)/double(16);
    double FSerrDiff3 = double(5)/double(16);
    double FSerrDiff4 = double(1)/double(16);
    double quantError, oldValue;
    double threshold = 128;
    //Thresholding at 50% and then using Floyd-Steinberg diffusion method
    for (int row = 0; row < height; row++)
    {
        if (row % 2 == 0)//For even row go left to right
        {
        for (int column = 0; column < width; column++)
        {
            if (floydErrorImage[row][column] < threshold)
            {
                oldValue = floydErrorImage[row][column];
                floydErrorImage[row][column] = 0;
                quantError = double(oldValue) - double(floydErrorImage[row][column]);//i.e. the old value itself
            }
            else
            {
                oldValue = floydErrorImage[row][column];
                floydErrorImage[row][column] = 255;
                quantError = double(oldValue) - double(floydErrorImage[row][column]);//i.e. old value - 255
            }
            floydErrorImage[row][column+1] = double(floydErrorImage[row][column+1]) + double(FSerrDiff1 * quantError);
            floydErrorImage[row+1][column-1] = double(floydErrorImage[row+1][column-1]) + double(FSerrDiff2 * quantError);
            floydErrorImage[row+1][column] = double(floydErrorImage[row+1][column]) + double(FSerrDiff3 * quantError);
            floydErrorImage[row+1][column+1] = double(floydErrorImage[row+1][column+1]) + double(FSerrDiff4 * quantError);
        }
        }
        else//For odd row go right to left
        {
            for (int column = width-1; column >= 0 ; column--)
            {
                if (floydErrorImage[row][column] < threshold)
                {
                    oldValue = floydErrorImage[row][column];
                    floydErrorImage[row][column] = 0;
                    quantError = double(oldValue) - double(floydErrorImage[row][column]);//i.e. the old value itself
                }
                else
                {
                    oldValue = floydErrorImage[row][column];
                    floydErrorImage[row][column] = 255;
                    quantError = double(oldValue) - double(floydErrorImage[row][column]);//i.e. old value - 255
                }
                floydErrorImage[row][column-1] = double(floydErrorImage[row][column-1]) + double(FSerrDiff1 * quantError);
                floydErrorImage[row+1][column+1] = double(floydErrorImage[row+1][column+1]) + double(FSerrDiff2 * quantError);
                floydErrorImage[row+1][column] = double(floydErrorImage[row+1][column]) + double(FSerrDiff3 * quantError);
                floydErrorImage[row+1][column-1] = double(floydErrorImage[row+1][column-1]) + double(FSerrDiff4 * quantError);
            }
        }
    }
    //Write the error diffused image for Floyd-Steinberg
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(floydErrorImage, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    //JARVIS, JUDICE, NINKE
    unsigned char JJNErrorImage[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            JJNErrorImage[row][column] = originalImage[row][column];
        }
    }
    double JJNDiff1 = double(7)/double(48);
    double JJNDiff2 = double(5)/double(48);
    double JJNDiff3 = double(3)/double(48);
    double JJNDiff4 = double(1)/double(48);
    //Thresholding at 50% and then using JJN diffusion method
    for (int row = 0; row < height; row++)
    {
        if (row % 2==0)//For even row go left to right
        {
        for (int column = 0; column < width; column++)
        {
            if (JJNErrorImage[row][column] < threshold)
            {
                oldValue = JJNErrorImage[row][column];
                JJNErrorImage[row][column] = 0;
                quantError = oldValue - JJNErrorImage[row][column];//i.e. the old value itself
            }
            else
            {
                oldValue = JJNErrorImage[row][column];
                JJNErrorImage[row][column] = 255;
                quantError = oldValue - JJNErrorImage[row][column];//i.e. old value - 255
            }
            
            //For JJNdiff1
            JJNErrorImage[row][column+1] = JJNErrorImage[row][column+1] + (JJNDiff1 * quantError);
            JJNErrorImage[row+1][column] = JJNErrorImage[row+1][column] + (JJNDiff1 * quantError);
            //For JJNdiff2
            JJNErrorImage[row][column+2] = JJNErrorImage[row][column+2] + (JJNDiff2 * quantError);
            JJNErrorImage[row+1][column+1] = JJNErrorImage[row+1][column+1] + (JJNDiff2 * quantError);
            JJNErrorImage[row+1][column-1] = JJNErrorImage[row+1][column-1] + (JJNDiff2 * quantError);
            JJNErrorImage[row+2][column] = JJNErrorImage[row+2][column] + (JJNDiff2 * quantError);
            //For JJNdiff3
            JJNErrorImage[row+2][column+1] = JJNErrorImage[row+2][column+1] + (JJNDiff3 * quantError);
            JJNErrorImage[row+2][column-1] = JJNErrorImage[row+2][column-1] + (JJNDiff3 * quantError);
            JJNErrorImage[row+1][column+2] = JJNErrorImage[row+1][column+2] + (JJNDiff3 * quantError);
            JJNErrorImage[row+1][column-2] = JJNErrorImage[row+1][column-2] + (JJNDiff3 * quantError);
            //For JJNdiff4
            JJNErrorImage[row+2][column+2] = JJNErrorImage[row+2][column+2] + (JJNDiff4 * quantError);
            JJNErrorImage[row+2][column-2] = JJNErrorImage[row+2][column-2] + (JJNDiff4 * quantError);   
            
        }
        }
        else//For odd row go right to left
        {
            for (int column = width-1; column >= 0; column--)
            {
                if (JJNErrorImage[row][column] < threshold)
                {
                    oldValue = JJNErrorImage[row][column];
                    JJNErrorImage[row][column] = 0;
                    quantError = oldValue - JJNErrorImage[row][column];//i.e. the old value itself
                }
                else
                {
                    oldValue = JJNErrorImage[row][column];
                    JJNErrorImage[row][column] = 255;
                    quantError = oldValue - JJNErrorImage[row][column];//i.e. old value - 255
                }
                
                //For JJNdiff1
                JJNErrorImage[row][column-1] = JJNErrorImage[row][column-1] + (JJNDiff1 * quantError);
                JJNErrorImage[row+1][column] = JJNErrorImage[row+1][column] + (JJNDiff1 * quantError);
                //For JJNdiff2
                JJNErrorImage[row][column-2] = JJNErrorImage[row][column-2] + (JJNDiff2 * quantError);
                JJNErrorImage[row+1][column-1] = JJNErrorImage[row+1][column-1] + (JJNDiff2 * quantError);
                JJNErrorImage[row+1][column+1] = JJNErrorImage[row+1][column+1] + (JJNDiff2 * quantError);
                JJNErrorImage[row+2][column] = JJNErrorImage[row+2][column] + (JJNDiff2 * quantError);
                //For JJNdiff3
                JJNErrorImage[row+2][column-1] = JJNErrorImage[row+2][column-1] + (JJNDiff3 * quantError);
                JJNErrorImage[row+2][column+1] = JJNErrorImage[row+2][column+1] + (JJNDiff3 * quantError);
                JJNErrorImage[row+1][column-2] = JJNErrorImage[row+1][column-2] + (JJNDiff3 * quantError);
                JJNErrorImage[row+1][column+2] = JJNErrorImage[row+1][column+2] + (JJNDiff3 * quantError);
                //For JJNdiff4
                JJNErrorImage[row+2][column-2] = JJNErrorImage[row+2][column-2] + (JJNDiff4 * quantError);
                JJNErrorImage[row+2][column+2] = JJNErrorImage[row+2][column+2] + (JJNDiff4 * quantError);
                
            }
        }
            
    }
    //Write the error diffused image for JJN
    if (!(file=fopen(argv[3],"wb")))
    {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(JJNErrorImage, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    //Stucki
    unsigned char stuckiErrorImage[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            stuckiErrorImage[row][column] = originalImage[row][column];
        }
    }
    double stuckiDiff1 = double(8)/double(42);
    double stuckiDiff2 = double(4)/double(42);
    double stuckiDiff3 = double(2)/double(42);
    double stuckiDiff4 = double(1)/double(42);
    //Thresholding at 50% and then using Stucki diffusion method
    for (int row = 0; row < height; row++)
    {
        if(row % 2 == 0)//For even row go left to right
        for (int column = 0; column < width; column++)
        {
            if (stuckiErrorImage[row][column] < threshold)
            {
                oldValue = stuckiErrorImage[row][column];
                stuckiErrorImage[row][column] = 0;
                quantError = oldValue - stuckiErrorImage[row][column];//i.e. the old value itself
            }
            else
            {
                oldValue = stuckiErrorImage[row][column];
                stuckiErrorImage[row][column] = 255;
                quantError = oldValue - stuckiErrorImage[row][column];//i.e. old value - 255
            }
            
            //For stuckidiff1
            stuckiErrorImage[row][column+1] = stuckiErrorImage[row][column+1] + (stuckiDiff1 * quantError);
            stuckiErrorImage[row+1][column] = stuckiErrorImage[row+1][column] + (stuckiDiff1 * quantError);
            //For stuckidiff2
            stuckiErrorImage[row][column+2] = stuckiErrorImage[row][column+2] + (stuckiDiff2 * quantError);
            stuckiErrorImage[row+1][column+1] = stuckiErrorImage[row+1][column+1] + (stuckiDiff2 * quantError);
            stuckiErrorImage[row+1][column-1] = stuckiErrorImage[row+1][column-1] + (stuckiDiff2 * quantError);
            stuckiErrorImage[row+2][column] = stuckiErrorImage[row+2][column] + (stuckiDiff2 * quantError);
            //For stuckidiff3
            stuckiErrorImage[row+2][column+1] = stuckiErrorImage[row+2][column+1] + (stuckiDiff3 * quantError);
            stuckiErrorImage[row+2][column-1] = stuckiErrorImage[row+2][column-1] + (stuckiDiff3 * quantError);
            stuckiErrorImage[row+1][column+2] = stuckiErrorImage[row+1][column+2] + (stuckiDiff3 * quantError);
            stuckiErrorImage[row+1][column-2] = stuckiErrorImage[row+1][column-2] + (stuckiDiff3 * quantError);
            //For stuckidiff4
            stuckiErrorImage[row+2][column+2] = stuckiErrorImage[row+2][column+2] + (stuckiDiff4 * quantError);
            stuckiErrorImage[row+2][column-2] = stuckiErrorImage[row+2][column-2] + (stuckiDiff4 * quantError);
            
        }
        else //For odd roe go right to left
        {
            for (int column = width-1; column >= 0; column--)
            {
                if (stuckiErrorImage[row][column] < threshold)
                {
                    oldValue = stuckiErrorImage[row][column];
                    stuckiErrorImage[row][column] = 0;
                    quantError = oldValue - stuckiErrorImage[row][column];//i.e. the old value itself
                }
                else
                {
                    oldValue = stuckiErrorImage[row][column];
                    stuckiErrorImage[row][column] = 255;
                    quantError = oldValue - stuckiErrorImage[row][column];//i.e. old value - 255
                }
                
                //For stuckidiff1
                stuckiErrorImage[row][column-1] = stuckiErrorImage[row][column-1] + (stuckiDiff1 * quantError);
                stuckiErrorImage[row+1][column] = stuckiErrorImage[row+1][column] + (stuckiDiff1 * quantError);
                //For stuckidiff2
                stuckiErrorImage[row][column-2] = stuckiErrorImage[row][column-2] + (stuckiDiff2 * quantError);
                stuckiErrorImage[row+1][column-1] = stuckiErrorImage[row+1][column-1] + (stuckiDiff2 * quantError);
                stuckiErrorImage[row+1][column+1] = stuckiErrorImage[row+1][column+1] + (stuckiDiff2 * quantError);
                stuckiErrorImage[row+2][column] = stuckiErrorImage[row+2][column] + (stuckiDiff2 * quantError);
                //For stuckidiff3
                stuckiErrorImage[row+2][column-1] = stuckiErrorImage[row+2][column-1] + (stuckiDiff3 * quantError);
                stuckiErrorImage[row+2][column+1] = stuckiErrorImage[row+2][column+1] + (stuckiDiff3 * quantError);
                stuckiErrorImage[row+1][column-2] = stuckiErrorImage[row+1][column-2] + (stuckiDiff3 * quantError);
                stuckiErrorImage[row+1][column+2] = stuckiErrorImage[row+1][column+2] + (stuckiDiff3 * quantError);
                //For stuckidiff4
                stuckiErrorImage[row+2][column-2] = stuckiErrorImage[row+2][column-2] + (stuckiDiff4 * quantError);
                stuckiErrorImage[row+2][column+2] = stuckiErrorImage[row+2][column+2] + (stuckiDiff4 * quantError);
                
            }
        }

        
    }
    //Write the error diffused image for JJN
    if (!(file=fopen(argv[4],"wb")))
    {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(stuckiErrorImage, sizeof(unsigned char), height*width, file);
	fclose(file);
   
    return 0;
}
