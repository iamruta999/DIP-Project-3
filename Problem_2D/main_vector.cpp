//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 2D
//This code performs vector color halftoning on a color image
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc,char *argv[])
{
    //char *argv[] = {"Sailboat.raw","VectorColorHalftonedSailboat.raw"};
                             
    /* Define file pointer and variable */
    FILE *file;
    int width = 512;
    int height = 512;
    int bytesPerPixel = 3;
    
    /* Allocate image data array */
    unsigned char originalImage[height][width][bytesPerPixel];
    double errorMatrix[height][width][bytesPerPixel];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            errorMatrix[row][column][0] = 0; //As initial error of all the pixels is zero
            errorMatrix[row][column][1] = 0;
            errorMatrix[row][column][2] = 0;
        }
    }
    
    
    /* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(originalImage, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    //Vector color Halftoning
    unsigned char CMYImage[height][width][bytesPerPixel];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            CMYImage[row][column][0] = 255-originalImage[row][column][0];//Convert red to cyan
            CMYImage[row][column][1] = 255-originalImage[row][column][1];//Convert green to magenta
            CMYImage[row][column][2] = 255-originalImage[row][column][2];//Convert blue to yellow
        }
    }
    
    int Cyan,Magenta,Yellow;
    int quadruple1[3], quadruple2[3], quadruple3[3], quadruple4[3];
    int error[3], pixelWithError[3], newError[3];
    int currentColor[3];
    int threshold = 128;
    int oldValue, vertex[3];
    double FSerrDiff1 = double(7)/double(16);
    double FSerrDiff2 = double(3)/double(16);
    double FSerrDiff3 = double(5)/double(16);
    double FSerrDiff4 = double(1)/double(16);
    double distance1, distance2, distance3, distance4;
    //VECTOR COLOR HALFTONING
    for (int row = 1; row < height-1; row++)
    {
        if (row % 2 == 0)
        {
            for (int column = 1; column < width-1;column++)
            {
                //CALCULTE THE MVBQ
                Cyan = CMYImage[row][column][0];
                Magenta = CMYImage[row][column][1];
                Yellow = CMYImage[row][column][2];
                
                currentColor[0] = Cyan; currentColor[1] = Magenta; currentColor[2] = Yellow;
                
                if ( (Cyan+Magenta) > 255 )
                {
                    if ( (Magenta+Yellow) > 255)
                    {
                        if ( (Cyan+Magenta+Yellow) > 510 )
                        {
                            //return RGBK;
                            quadruple1[0] = 0; quadruple1[1] = 255; quadruple1[2] = 255;//R
                            quadruple2[0] = 255; quadruple2[1] = 0; quadruple2[2] = 255;//G
                            quadruple3[0] = 255, quadruple3[1] = 255, quadruple3[2] = 0;//B
                            quadruple4[0] = quadruple4[1] = quadruple4[2] = 255;//K
                        }
                        else
                        {
                            //return GBMR;
                            quadruple1[0] = 255, quadruple1[1] = 0, quadruple1[2] = 255;//G
                            quadruple2[0] = 255, quadruple2[1] = 255, quadruple2[2] = 0;//B
                            quadruple3[0] = 0, quadruple3[1] = 255, quadruple3[2] = 0 ;//M
                            quadruple4[0] = 0, quadruple4[1] = 255, quadruple4[2] = 255;//R
                        }
                    }
                    else
                    {
                        //return CMGB;
                        quadruple1[0] = 255, quadruple1[1] = quadruple1[2] = 0;//C
                        quadruple2[0] = 0, quadruple2[1] = 255, quadruple2[2] = 0;//M
                        quadruple3[0] = 255, quadruple3[1] = 0, quadruple3[2] = 255;//G
                        quadruple4[0] = 255, quadruple4[1] = 255, quadruple4[2] = 0;//B
                    }
                }
                else
                {
                    if(!((Magenta+Yellow) > 255 ) )
                    {
                        if ( !((Cyan+Magenta+Yellow) > 255 ) )
                        {
                            //return WCMY;
                            quadruple1[0] = quadruple1[1] = quadruple1[2] = 0;//W
                            quadruple2[0] = 255, quadruple2[1] = quadruple2[2] = 0;//C
                            quadruple3[0] = 0, quadruple3[1] =  255, quadruple3[2] = 0;//M
                            quadruple4[0] = quadruple4[1] = 0, quadruple4[2] = 255;//Y
                        }
                        else
                        {
                            //return CMYG;
                            quadruple1[0] = 255, quadruple1[1] = quadruple1[2] = 0;//C
                            quadruple2[0] = 0, quadruple2[1] = 255, quadruple2[2] = 0;//M
                            quadruple3[0] = quadruple3[1] = 0, quadruple3[2] = 255;//Y
                            quadruple4[0] = 255, quadruple4[1] = 0; quadruple4[2] = 255;//G
                        }
                    }
                    else
                    {
                        //return RGMY;
                        quadruple1[0] = 0, quadruple1[1] = 255, quadruple1[2] = 255;//R
                        quadruple2[0] = 255, quadruple2[1] = 0, quadruple2[2] = 255;//G
                        quadruple3[0] = 0, quadruple3[1] = 255, quadruple3[2] = 0;//M
                        quadruple4[0] = quadruple4[1] = 0, quadruple4[2] = 255;//Y
                    }
                }
                //Getting the distance to calculate the closest vertex
                for (int i = 0; i < 3; i++)
                {
                    pixelWithError[i] = errorMatrix[row][column][i] + currentColor[i];
                }
                distance1 = sqrt ( ((pixelWithError[0]-quadruple1[0])*(pixelWithError[0]-quadruple1[0])) + ((pixelWithError[1]-quadruple1[1])*(pixelWithError[1]-quadruple1[1])) + ((pixelWithError[2]-quadruple1[2])*(pixelWithError[2]-quadruple1[2])) );
                distance2 = sqrt ( ((pixelWithError[0]-quadruple2[0])*(pixelWithError[0]-quadruple2[0])) + ((pixelWithError[1]-quadruple2[1])*(pixelWithError[1]-quadruple2[1])) + ((pixelWithError[2]-quadruple2[2])*(pixelWithError[2]-quadruple2[2])) );
                distance3 = sqrt ( ((pixelWithError[0]-quadruple3[0])*(pixelWithError[0]-quadruple3[0])) + ((pixelWithError[1]-quadruple3[1])*(pixelWithError[1]-quadruple3[1])) + ((pixelWithError[2]-quadruple3[2])*(pixelWithError[2]-quadruple3[2])) );
                distance4 = sqrt ( ((pixelWithError[0]-quadruple4[0])*(pixelWithError[0]-quadruple4[0])) + ((pixelWithError[1]-quadruple4[1])*(pixelWithError[1]-quadruple4[1])) + ((pixelWithError[2]-quadruple4[2])*(pixelWithError[2]-quadruple4[2])) );
                
                if (distance1 < distance2 && distance1 < distance3 && distance1 < distance4)
                {
                    vertex[0] = quadruple1[0]; vertex[1] = quadruple1[1]; vertex[2] = quadruple1[2];
                }
                else if (distance2 < distance1 && distance2 < distance3 && distance2 < distance4)
                {
                    vertex[0] = quadruple2[0]; vertex[1] = quadruple2[1]; vertex[2] = quadruple2[2];
                }
                else if (distance3 < distance1 && distance3 < distance2 && distance3 < distance4)
                {
                    vertex[0] = quadruple3[0]; vertex[1] = quadruple3[1]; vertex[2] = quadruple3[2];
                }
                else
                {
                    vertex[0] = quadruple4[0]; vertex[1] = quadruple4[1]; vertex[2] = quadruple4[2];
                }
                
                //Assign that selected vertex to the current pixel
                originalImage[row][column][0] = vertex[0];
                originalImage[row][column][1] = vertex[1];
                originalImage[row][column][2] = vertex[2];
                
                //Diffuse error using Floyd-Steinbergs matrix
                newError[0] = pixelWithError[0] - vertex[0];
                newError[1] = pixelWithError[1] - vertex[1];
                newError[2] = pixelWithError[2] - vertex[2];
                
                errorMatrix[row][column+1][0] = errorMatrix[row][column+1][0] + (FSerrDiff1 * newError[0]);
                errorMatrix[row+1][column-1][0] =  errorMatrix[row+1][column-1][0] + (FSerrDiff2 * newError[0]);
                errorMatrix[row+1][column][0] =  errorMatrix[row+1][column][0] + (FSerrDiff3 * newError[0]);
                errorMatrix[row+1][column+1][0] = errorMatrix[row+1][column+1][0] + (FSerrDiff4 * newError[0]);
                
                errorMatrix[row][column+1][1] = errorMatrix[row][column+1][1] + (FSerrDiff1 * newError[1]);
                errorMatrix[row+1][column-1][1] = errorMatrix[row+1][column-1][1] + (FSerrDiff2 * newError[1]);
                errorMatrix[row+1][column][1] = errorMatrix[row+1][column][1] + (FSerrDiff3 * newError[1]);
                errorMatrix[row+1][column+1][1] = errorMatrix[row+1][column+1][1] + (FSerrDiff4 * newError[1]);
                
                errorMatrix[row][column+1][2] = errorMatrix[row][column+1][2] + (FSerrDiff1 * newError[2]);
                errorMatrix[row+1][column-1][2] = errorMatrix[row+1][column-1][2] + (FSerrDiff2 * newError[2]);
                errorMatrix[row+1][column][2] = errorMatrix[row+1][column][2] + (FSerrDiff3 * newError[2]);
                errorMatrix[row+1][column+1][2] = errorMatrix[row+1][column+1][2] + (FSerrDiff4 * newError[2]);
                
            }
        }
        else
        {
            for (int column = width-2; column >= 1;column--)
            {
                //CALCULTE THE MVBQ
                Cyan = CMYImage[row][column][0];
                Magenta = CMYImage[row][column][1];
                Yellow = CMYImage[row][column][2];
                
                currentColor[0] = Cyan; currentColor[1] = Magenta; currentColor[2] = Yellow;
                
                if ( (Cyan+Magenta) > 255 )
                {
                    if ( (Magenta+Yellow) > 255)
                    {
                        if ( (Cyan+Magenta+Yellow) > 510 )
                        {
                            //return RGBK;
                            quadruple1[0] = 0; quadruple1[1] = 255; quadruple1[2] = 255;//R
                            quadruple2[0] = 255; quadruple2[1] = 0; quadruple2[2] = 255;//G
                            quadruple3[0] = 255, quadruple3[1] = 255, quadruple3[2] = 0;//B
                            quadruple4[0] = quadruple4[1] = quadruple4[2] = 255;//K
                        }
                        else
                        {
                            //return GBMR;
                            quadruple1[0] = 255, quadruple1[1] = 0, quadruple1[2] = 255;//G
                            quadruple2[0] = 255, quadruple2[1] = 255, quadruple2[2] = 0;//B
                            quadruple3[0] = 0, quadruple3[1] = 255, quadruple3[2] = 0 ;//M
                            quadruple4[0] = 0, quadruple4[1] = 255, quadruple4[2] = 255;//R
                        }
                    }
                    else
                    {
                        //return CMGB;
                        quadruple1[0] = 255, quadruple1[1] = quadruple1[2] = 0;//C
                        quadruple2[0] = 0, quadruple2[1] = 255, quadruple2[2] = 0;//M
                        quadruple3[0] = 255, quadruple3[1] = 0, quadruple3[2] = 255;//G
                        quadruple4[0] = 255, quadruple4[1] = 255, quadruple4[2] = 0;//B
                    }
                }
                else
                {
                    if(!((Magenta+Yellow) > 255 ) )
                    {
                        if ( !((Cyan+Magenta+Yellow) > 255 ) )
                        {
                            //return WCMY;
                            quadruple1[0] = quadruple1[1] = quadruple1[2] = 0;//W
                            quadruple2[0] = 255, quadruple2[1] = quadruple2[2] = 0;//C
                            quadruple3[0] = 0, quadruple3[1] =  255, quadruple3[2] = 0;//M
                            quadruple4[0] = quadruple4[1] = 0, quadruple4[2] = 255;//Y
                        }
                        else
                        {
                            //return CMYG;
                            quadruple1[0] = 255, quadruple1[1] = quadruple1[2] = 0;//C
                            quadruple2[0] = 0, quadruple2[1] = 255, quadruple2[2] = 0;//M
                            quadruple3[0] = quadruple3[1] = 0, quadruple3[2] = 255;//Y
                            quadruple4[0] = 255, quadruple4[1] = 0; quadruple4[2] = 255;//G
                        }
                    }
                    else
                    {
                        //return RGMY;
                        quadruple1[0] = 0, quadruple1[1] = 255, quadruple1[2] = 255;//R
                        quadruple2[0] = 255, quadruple2[1] = 0, quadruple2[2] = 255;//G
                        quadruple3[0] = 0, quadruple3[1] = 255, quadruple3[2] = 0;//M
                        quadruple4[0] = quadruple4[1] = 0, quadruple4[2] = 255;//Y
                    }
                }
                //Getting the distance to calculate the closest vertex
                for (int i = 0; i < 3; i++)
                {
                    pixelWithError[i] = errorMatrix[row][column][i] + currentColor[i];
                }
                distance1 = sqrt ( ((pixelWithError[0]-quadruple1[0])*(pixelWithError[0]-quadruple1[0])) + ((pixelWithError[1]-quadruple1[1])*(pixelWithError[1]-quadruple1[1])) + ((pixelWithError[2]-quadruple1[2])*(pixelWithError[2]-quadruple1[2])) );
                distance2 = sqrt ( ((pixelWithError[0]-quadruple2[0])*(pixelWithError[0]-quadruple2[0])) + ((pixelWithError[1]-quadruple2[1])*(pixelWithError[1]-quadruple2[1])) + ((pixelWithError[2]-quadruple2[2])*(pixelWithError[2]-quadruple2[2])) );
                distance3 = sqrt ( ((pixelWithError[0]-quadruple3[0])*(pixelWithError[0]-quadruple3[0])) + ((pixelWithError[1]-quadruple3[1])*(pixelWithError[1]-quadruple3[1])) + ((pixelWithError[2]-quadruple3[2])*(pixelWithError[2]-quadruple3[2])) );
                distance4 = sqrt ( ((pixelWithError[0]-quadruple4[0])*(pixelWithError[0]-quadruple4[0])) + ((pixelWithError[1]-quadruple4[1])*(pixelWithError[1]-quadruple4[1])) + ((pixelWithError[2]-quadruple4[2])*(pixelWithError[2]-quadruple4[2])) );
                
                if (distance1 < distance2 && distance1 < distance3 && distance1 < distance4)
                {
                    vertex[0] = quadruple1[0]; vertex[1] = quadruple1[1]; vertex[2] = quadruple1[2];
                }
                else if (distance2 < distance1 && distance2 < distance3 && distance2 < distance4)
                {
                    vertex[0] = quadruple2[0]; vertex[1] = quadruple2[1]; vertex[2] = quadruple2[2];
                }
                else if (distance3 < distance1 && distance3 < distance2 && distance3 < distance4)
                {
                    vertex[0] = quadruple3[0]; vertex[1] = quadruple3[1]; vertex[2] = quadruple3[2];
                }
                else
                {
                    vertex[0] = quadruple4[0]; vertex[1] = quadruple4[1]; vertex[2] = quadruple4[2];
                }
                
                //Assign that selected vertex to the current pixel
                originalImage[row][column][0] = vertex[0];
                originalImage[row][column][1] = vertex[1];
                originalImage[row][column][2] = vertex[2];
                
                //Diffuse error using Floyd-Steinbergs matrix
                newError[0] = pixelWithError[0] - vertex[0];
                newError[1] = pixelWithError[1] - vertex[1];
                newError[2] = pixelWithError[2] - vertex[2];
                
                errorMatrix[row][column+1][0] = errorMatrix[row][column+1][0] + (FSerrDiff1 * newError[0]);
                errorMatrix[row+1][column-1][0] =  errorMatrix[row+1][column-1][0] + (FSerrDiff2 * newError[0]);
                errorMatrix[row+1][column][0] =  errorMatrix[row+1][column][0] + (FSerrDiff3 * newError[0]);
                errorMatrix[row+1][column+1][0] = errorMatrix[row+1][column+1][0] + (FSerrDiff4 * newError[0]);
                
                errorMatrix[row][column+1][1] = errorMatrix[row][column+1][1] + (FSerrDiff1 * newError[1]);
                errorMatrix[row+1][column-1][1] = errorMatrix[row+1][column-1][1] + (FSerrDiff2 * newError[1]);
                errorMatrix[row+1][column][1] = errorMatrix[row+1][column][1] + (FSerrDiff3 * newError[1]);
                errorMatrix[row+1][column+1][1] = errorMatrix[row+1][column+1][1] + (FSerrDiff4 * newError[1]);
                
                errorMatrix[row][column+1][2] = errorMatrix[row][column+1][2] + (FSerrDiff1 * newError[2]);
                errorMatrix[row+1][column-1][2] = errorMatrix[row+1][column-1][2] + (FSerrDiff2 * newError[2]);
                errorMatrix[row+1][column][2] = errorMatrix[row+1][column][2] + (FSerrDiff3 * newError[2]);
                errorMatrix[row+1][column+1][2] = errorMatrix[row+1][column+1][2] + (FSerrDiff4 * newError[2]);
                
            }
            
        }
        
    }
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            originalImage[row][column][0] = 255-originalImage[row][column][0];
            originalImage[row][column][1] = 255-originalImage[row][column][1];
            originalImage[row][column][2] = 255-originalImage[row][column][2];
        }
    }
    
    //Write the vector color half-toned image
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite( originalImage, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    
    return 0;
}
