//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 1B
//This code perfroms the perspective transformation and imaging geometry. I gives the image of a 3D cube that gets captured in the camera.
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <cmath>

using namespace std;

int main(int argc,char *argv[])
{
    //char *argv[] = {"baby.raw","baby_cat.raw","baby_dog.raw","baby_panda.raw","baby_bear.raw", "final2Dimage.raw"};
    
    /* Define file pointer and variable */
    FILE *file;
    
    int bytesPerPixel = 3;
    int width = 200;
    int height = 200;
    unsigned char image[height][width][bytesPerPixel];
    
    // For part 1 of pre-processing
    double cartesianX, cartesianY, cartesianZ, worldX, worldY, worldZ;
    double midpoint = double(width)/double(2);
    double oldRangeLower = -99.5;
    double oldRangeHigher = 99.5;
    double oldRangeDifference = double(oldRangeHigher - oldRangeLower);
    int newRangeLower = -1;
    int newRangeHigher = 1;
    
    //For part 2
    double Xc[3] = { (-1/sqrt(2)), (1/sqrt(2)), 0 };
    double Yc[3] = { (1/sqrt(6)), (1/sqrt(6)), (-2/sqrt(6)) };
    double Zc[3] = { (-1/sqrt(3)), (-1/sqrt(3)), (-1/sqrt(3)) };
    
    double tX = (-(5 * Xc[0] + 5 * Xc[1] + 5 * Xc[2]) );
    double tY = (-(5 * Yc[0] + 5 * Yc[1] + 5 * Yc[2]) );
    double tZ = (-(5 * Zc[0] + 5 * Zc[1] + 5 * Zc[2]) );
    
    //double extrinsic[3][4] = { {Xc[0], Xc[1], Xc[2], tX}, {Yc[0], Yc[1], Yc[2], tY}, {Zc[0], Zc[1], Zc[2], tZ} };
    double intrinsic[3][3] = { {sqrt(3), 0, 0}, {0, sqrt(3), 0}, {0, 0, 1} };
    double finalImage[height][width][5];
    double resultExt[3][1];
    double resultInt[3][1];
    
    //For scaling
    double minimumX = -0.303263;
    double maximumX = 0.303046;
    double oldRangeX = maximumX - minimumX;
    double minimumY = -0.306186;
    double maximumY = 0.34955;
    double oldRangeY = maximumY - minimumY;
    int newXmin = 20;
    int newXmax = 280;
    int newYmin = 20;
    int newYmax = 280;
    int cubeX = 300;
    int cubeY = 300;
    // For final cube capture by camera
    unsigned char cubeImage[cubeX][cubeY][3];
    for (int row = 0; row < cubeX; row++)
    {
        for (int column = 0; column < cubeY; column++)
        {
            cubeImage[row][column][0] = 0;
            cubeImage[row][column][1] = 0;
            cubeImage[row][column][2] = 0;
            
        }
    }
    
    //READ IMAGE 1
    
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(image, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    //Place image 1 at the top of the cube
    double finalImage1[height][width][2];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cartesianX = double(row - midpoint);
            cartesianY = double(column - midpoint);
            
            //Scale these values between -1 to 1
            worldX = double (newRangeLower * (1-((cartesianX - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianX - oldRangeLower)/oldRangeDifference) );
            worldY = double (newRangeLower * (1-((cartesianY - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianY - oldRangeLower)/oldRangeDifference) );
            worldZ = 1;
            
            resultExt[0][0] = Xc[0] * worldX + Xc[1] * worldY + Xc[2] * worldZ + tX * 1;
            resultExt[1][0] = Yc[0] * worldX + Yc[1] * worldY + Yc[2] * worldZ + tY * 1;
            resultExt[2][0] = Zc[0] * worldX + Zc[1] * worldY + Zc[2] * worldZ + tZ * 1;
            
            resultInt[0][0] = intrinsic[0][0]*resultExt[0][0] + intrinsic[0][1] * resultExt[1][0] + intrinsic[0][2] * resultExt[2][0];
            resultInt[1][0] = intrinsic[1][0]*resultExt[0][0] + intrinsic[1][1] * resultExt[1][0] + intrinsic[1][2] * resultExt[2][0];
            resultInt[2][0] = intrinsic[2][0]*resultExt[0][0] + intrinsic[2][1] * resultExt[1][0] + intrinsic[2][2] * resultExt[2][0];
            
            finalImage1[row][column][0] = (resultInt[0][0] / resultInt[2][0]);//Store x
            finalImage1[row][column][1] = (resultInt[1][0] / resultInt[2][0]);//Store y
        }
    }
    
    int image1xy[height][width][2];//To store x and y
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image1xy[row][column][0] = (newXmin * (1-((finalImage1[row][column][0] - minimumX)/oldRangeX))) + (newXmax * ((finalImage1[row][column][0]-minimumX)/oldRangeX));
            image1xy[row][column][1] = (newYmin * (1-((finalImage1[row][column][1] - minimumY)/oldRangeY))) + (newYmax * ((finalImage1[row][column][1]-minimumY)/oldRangeY));
        }
    }
    // For the final cube capture of image 1
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cubeImage[image1xy[row][column][0]][image1xy[row][column][1]][0] = image[row][column][0];
            cubeImage[image1xy[row][column][0]][image1xy[row][column][1]][1] = image[row][column][1];
            cubeImage[image1xy[row][column][0]][image1xy[row][column][1]][2] = image[row][column][2];
            
        }
    }
    
    //READ IMAGE 2
    if (!(file=fopen(argv[2],"rb")))
    {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(image, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    //Place image 2 at the front side of the cube
    double finalImage2[height][width][2];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cartesianY = double(row - midpoint);
            cartesianZ = double(column - midpoint);
            
            //Scale these values between -1 to 1
            worldY = double (newRangeLower * (1-((cartesianY - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianY - oldRangeLower)/oldRangeDifference) );
            worldZ = double (newRangeLower * (1-((cartesianZ - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianZ - oldRangeLower)/oldRangeDifference) );
            worldX = 1;
            resultExt[0][0] = Xc[0] * worldX + Xc[1] * worldY + Xc[2] * worldZ + tX * 1;
            resultExt[1][0] = Yc[0] * worldX + Yc[1] * worldY + Yc[2] * worldZ + tY * 1;
            resultExt[2][0] = Zc[0] * worldX + Zc[1] * worldY + Zc[2] * worldZ + tZ * 1;
            
            resultInt[0][0] = intrinsic[0][0]*resultExt[0][0] + intrinsic[0][1] * resultExt[1][0] + intrinsic[0][2] * resultExt[2][0];
            resultInt[1][0] = intrinsic[1][0]*resultExt[0][0] + intrinsic[1][1] * resultExt[1][0] + intrinsic[1][2] * resultExt[2][0];
            resultInt[2][0] = intrinsic[2][0]*resultExt[0][0] + intrinsic[2][1] * resultExt[1][0] + intrinsic[2][2] * resultExt[2][0];
            
            finalImage2[row][column][0] = (resultInt[0][0] / resultInt[2][0]);//Store x
            finalImage2[row][column][1] = (resultInt[1][0] / resultInt[2][0]);//Store y
        }
    }
    
    int image2xy[height][width][2];//To store x and y
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image2xy[row][column][0] = (newXmin * (1-((finalImage2[row][column][0] - minimumX)/oldRangeX))) + (newXmax * ((finalImage2[row][column][0]-minimumX)/oldRangeX));
            image2xy[row][column][1] = (newYmin * (1-((finalImage2[row][column][1] - minimumY)/oldRangeY))) + (newYmax * ((finalImage2[row][column][1]-minimumY)/oldRangeY));
        }
    }
    // For the final cube capture of image 2
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cubeImage[image2xy[row][column][0]][image2xy[row][column][1]][0] = image[row][column][0];
            cubeImage[image2xy[row][column][0]][image2xy[row][column][1]][1] = image[row][column][1];
            cubeImage[image2xy[row][column][0]][image2xy[row][column][1]][2] = image[row][column][2];
            
        }
    }
    
    //READ IMAGE 3
    if (!(file=fopen(argv[3],"rb")))
    {
		cout << "Cannot open file: " << argv[3] <<endl;
		exit(1);
	}
	fread(image, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    //Place image 3 at the right side of the cube
    double finalImage3[height][width][2];//To store x and y
    
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cartesianX = double(row - midpoint);
            cartesianZ = double(column - midpoint);
            
            //Scale these values between -1 to 1
            worldX = double (newRangeLower * (1-((cartesianX - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianX - oldRangeLower)/oldRangeDifference) );
            worldZ = double (newRangeLower * (1-((cartesianZ - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianZ - oldRangeLower)/oldRangeDifference) );
            worldY = 1;
            
            resultExt[0][0] = Xc[0] * worldX + Xc[1] * worldY + Xc[2] * worldZ + tX * 1;
            resultExt[1][0] = Yc[0] * worldX + Yc[1] * worldY + Yc[2] * worldZ + tY * 1;
            resultExt[2][0] = Zc[0] * worldX + Zc[1] * worldY + Zc[2] * worldZ + tZ * 1;
            
            resultInt[0][0] = intrinsic[0][0]*resultExt[0][0] + intrinsic[0][1] * resultExt[1][0] + intrinsic[0][2] * resultExt[2][0];
            resultInt[1][0] = intrinsic[1][0]*resultExt[0][0] + intrinsic[1][1] * resultExt[1][0] + intrinsic[1][2] * resultExt[2][0];
            resultInt[2][0] = intrinsic[2][0]*resultExt[0][0] + intrinsic[2][1] * resultExt[1][0] + intrinsic[2][2] * resultExt[2][0];
            
            finalImage3[row][column][0] = (resultInt[0][0] / resultInt[2][0]);//Store x
            finalImage3[row][column][1] = (resultInt[1][0] / resultInt[2][0]);//Store y
        }
    }
    
    int image3xy[height][width][2];//To store x and y
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image3xy[row][column][0] = (newXmin * (1-((finalImage3[row][column][0] - minimumX)/oldRangeX))) + (newXmax * ((finalImage3[row][column][0]-minimumX)/oldRangeX));
            image3xy[row][column][1] = (newYmin * (1-((finalImage3[row][column][1] - minimumY)/oldRangeY))) + (newYmax * ((finalImage3[row][column][1]-minimumY)/oldRangeY));
        }
    }
    // For the final cube capture of image 2
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cubeImage[image3xy[row][column][0]][image3xy[row][column][1]][0] = image[row][column][0];
            cubeImage[image3xy[row][column][0]][image3xy[row][column][1]][1] = image[row][column][1];
            cubeImage[image3xy[row][column][0]][image3xy[row][column][1]][2] = image[row][column][2];
            
        }
    }
    
    //UNCOMMENT THIS TO PLACE IMAGE 4 AND 5 ON THE CUBE
    /*
    //READ IMAGE 4
    if (!(file=fopen(argv[4],"rb")))
    {
		cout << "Cannot open file: " << argv[4] <<endl;
		exit(1);
	}
	fread(image, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    //Place image 4 at the back side of the cube
    double worldCoordsForImage4[height][width][planes];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cartesianY = double(column - midpoint);
            cartesianZ = double(row - midpoint);
            
            //Scale these values between -1 to 1
            worldY = double (newRangeLower * (1-((cartesianY - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianY - oldRangeLower)/oldRangeDifference) );
            worldZ = double (newRangeLower * (1-((cartesianZ - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianZ - oldRangeLower)/oldRangeDifference) );
            worldX = -1;
            
            worldCoordsForImage4[row][column][0] = worldX;
            worldCoordsForImage4[row][column][1] = worldY;
            worldCoordsForImage4[row][column][2] = worldZ;
            worldCoordsForImage4[row][column][3] = image4[row][column][0]; //Red
            worldCoordsForImage4[row][column][4] = image4[row][column][1]; //Green
            worldCoordsForImage4[row][column][5] = image4[row][column][2]; //Blue
        }
    }
    //READ IMAGE 5
    if (!(file=fopen(argv[5],"rb")))
    {
		cout << "Cannot open file: " << argv[5] <<endl;
		exit(1);
	}
	fread(image, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    //Place image 5 at the left side of the cube
    double worldCoordsForImage5[height][width][planes];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            cartesianX = double(column - midpoint);
            cartesianZ = double(row - midpoint);
            
            //Scale these values between -1 to 1
            worldX = double (newRangeLower * (1-((cartesianX - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianX - oldRangeLower)/oldRangeDifference) );
            worldZ = double (newRangeLower * (1-((cartesianZ - oldRangeLower)/oldRangeDifference))
                             + newRangeHigher * ((cartesianZ - oldRangeLower)/oldRangeDifference) );
            worldY = -1;
            
            worldCoordsForImage5[row][column][0] = worldX;
            worldCoordsForImage5[row][column][1] = worldY;
            worldCoordsForImage5[row][column][2] = worldZ;
            worldCoordsForImage5[row][column][3] = image5[row][column][0]; //Red
            worldCoordsForImage5[row][column][4] = image5[row][column][1]; //Green
            worldCoordsForImage5[row][column][5] = image5[row][column][2]; //Blue
        }
    }
    
    */
    //Write the final cube image
    if (!(file=fopen(argv[6],"wb")))
    {
		cout << "Cannot open file: " << argv[6] << endl;
		exit(1);
	}
	fwrite(cubeImage, sizeof(unsigned char), cubeX*cubeY*bytesPerPixel, file);
	fclose(file);
    
    return 0;
}



