//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 1A
//This code gives a swirling effect to an image
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <cmath>
#define PI 3.1415

using namespace std;

int main(int argc,char *argv[])
{
    //char *argv[] = {"Kate.raw","swirlKate.raw"};
    
    /* Define file pointer and variable */
    FILE *file;
    int bytesPerPixel = 3;
    int width = 512;
    int height = 512;
    
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
    
    /* Getting the Swirl Effect */
    unsigned char swirlImage[height][width][bytesPerPixel];
    
    double midPointX = ceil(width/2);
    double midPointY = ceil(height/2);
    
    double cartesianX, cartesianY, theta, rho, rotationAngle;
    int scaling = 160;
    double newX,newY;
    
        for (int row = 0; row < height; row++)
        {
            for (int column = 0; column < width; column++)
            {
                //Convert from image to cartesian coordinates
                cartesianY = midPointY - row;
                cartesianX = column - midPointX;
                
                //Converting from cartesian to polar
                //Calculate angle 'theta'
                theta = atan2(cartesianY,cartesianX);
                rho = sqrt( (cartesianX*cartesianX) + (cartesianY*cartesianY) );
                //Calculate the angle by which we want to rotate the image
                //rotationAngle = theta + (0.008 * rho);
                rotationAngle = theta - (rho/scaling) + (90*PI/180);
                
                //Converting from polar to cartesian
                newX = double(rho * cos(rotationAngle));
                newY = double(rho * sin(rotationAngle));
            
                //convert back to image from cartesian
                newX = double(newX + midPointX);
                newY = double(newY + midPointY);
                
                //Bilinear Interpolation
                int intLocationX1 = int(newX);
                int intLocationY2 = int(newY);
                
                int intLocationX2 = intLocationX1 + 1;//Getting other three points
                int intLocationY1 = intLocationY2 + 1;// x1y2 x2y2
                                                      // x1y1 x2y1
                float xdiff = newX - intLocationX1;
                float ydiff = newY - intLocationY2;
                
                //Fit the image properly so that there are no negative out-of-image bound coordinates
                if(intLocationX1 < 0 || intLocationX2 < 0 || intLocationY1 < 0 || intLocationY2 < 0 || intLocationX1 >= height || intLocationX2 >= height || intLocationY1 >= width || intLocationY2 >= width)
                {
                    swirlImage[row][column][0] = 0;
                    swirlImage[row][column][1] = 0;
                    swirlImage[row][column][2] = 0;
                }
                
                else
                {
                swirlImage[row][column][0] = (1-xdiff) * (1-ydiff) * originalImage[intLocationX1][intLocationY2][0] +
                                             (xdiff)   * (1-ydiff) * originalImage[intLocationX2][intLocationY2][0] +
                                             (1-xdiff) * (ydiff)   * originalImage[intLocationX1][intLocationY1][0] +
                                             (xdiff)   * (ydiff)   * originalImage[intLocationX2][intLocationY1][0];
                
                swirlImage[row][column][1] = (1-xdiff) * (1-ydiff) * originalImage[intLocationX1][intLocationY2][1] +
                                             (xdiff)   * (1-ydiff) * originalImage[intLocationX2][intLocationY2][1] +
                                             (1-xdiff) * (ydiff)   * originalImage[intLocationX1][intLocationY1][1] +
                                             (xdiff)   * (ydiff)   * originalImage[intLocationX2][intLocationY1][1];
                
                swirlImage[row][column][2] = (1-xdiff) * (1-ydiff) * originalImage[intLocationX1][intLocationY2][2] +
                                             (xdiff)   * (1-ydiff) * originalImage[intLocationX2][intLocationY2][2] +
                                             (1-xdiff) * (ydiff)   * originalImage[intLocationX1][intLocationY1][2] +
                                             (xdiff)   * (ydiff)   * originalImage[intLocationX2][intLocationY1][2];
                }
                
                
            }
            

        }
    
   
    
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(swirlImage, sizeof(unsigned char),height*width*bytesPerPixel , file);
	fclose(file);
    
    
    return 0;
}


