//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 3B Part 2
//This code performs thinning on the horse image
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main()
{
    char *argv[] = {"hole-filledHorse.raw","pre-processedHorse","ThinnedHorseWithPre-processing.raw"};
    /* Define file pointer and variable */
    FILE *file;
    int width = 360;
    int height = 275;
    
    /* Allocate image data array */
    unsigned char originalImage[height][width];
    /* Read image */
    if (!(file=fopen(argv[0],"rb")))
    {
		cout << "Cannot open file: " << argv[0] <<endl;
		exit(1);
	}
	fread(originalImage, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    // Remove noise using dilation and erosion
    int N = 3;
    int halfLength = (N-1)/2;
    int window[N][N];
    unsigned char dilated[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            dilated[row][column] = originalImage[row][column];
        }
    }
    
    // Apply Dilation
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (originalImage[row][column] == 0)
            {
                int count = 0;
                //Get the 3 by 3 window
                for ( int i = 0; i < N; i++)
                {
                    for ( int j = 0; j < N; j++)
                    {
                        window[i][j] = originalImage[row + (i - halfLength)][column + (j - halfLength)];
                        if (window[i][j] == 255)
                            count = count + 1;
                    }
                }
                if (count > 1)
                    dilated[row][column] = 255;
                
            }
        }
    }
    unsigned char eroded[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            eroded[row][column] = dilated[row][column];
        }
    }
    // Apply erosion
    
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (dilated[row][column] == 255)
            {
                int count = 0;
                //Get the 3 by 3 window
                for ( int i = 0; i < N; i++)
                {
                    for ( int j = 0; j < N; j++)
                    {
                        window[i][j] = originalImage[row + (i - halfLength)][column + (j - halfLength)];
                        if (window[i][j] == 0)
                            count = count + 1;
                    }
                }
                if (count > 1)
                    eroded[row][column] = 0;
                
            }
        }
    }
    //Write the eroded Horseshoe image i.e. the pre-processed image
    if (!(file=fopen(argv[1],"wb")))
    {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fwrite(eroded, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    // We first need to convert the pre-processed image to 0 and 1
    int image[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (eroded[row][column] == 255)
                image[row][column] = 1;
            else
                image[row][column] = 0;
        }
    }
    
    // Input this image to the first stage to create conditional Mark patterns
    int first[height][width];
    int second[height][width];
    int modifiedImage[height][width];
    unsigned char finalImage[height][width];
    int bond;
    int unequal = 1;
    int count;
    int iteration = 0;
    
    // Create conditional pattern tables for bonds from 4 to 10
    int pattern4[8][8] = { {0,1,0,1,0,0,0,0}, {0,1,0,0,0,0,0,1}, {0,0,0,0,0,1,0,1}, {0,0,0,1,0,1,0,0},
        {0,0,1,1,1,0,0,0}, {1,1,1,0,0,0,0,0}, {1,0,0,0,0,0,1,1}, {0,0,0,0,1,1,1,0} };
    int pattern5[8][8] = { {1,1,0,1,0,0,0,0}, {0,1,0,1,1,0,0,0}, {0,1,1,0,0,0,0,1}, {0,0,1,1,0,1,0,0},
        {0,1,1,1,0,0,0,0}, {1,1,0,0,0,0,0,1}, {0,0,0,0,0,1,1,1}, {0,0,0,1,1,1,0,0} };
    int pattern6[10][8] = { {1,1,0,1,1,0,0,0}, {0,1,1,0,0,0,1,1},
        {1,1,1,1,0,0,0,0}, {0,1,1,1,1,0,0,0},{1,1,1,0,0,0,0,1}, {1,1,0,0,0,0,1,1},
        {1,0,0,0,0,1,1,1}, {0,0,0,0,1,1,1,1},{0,0,0,1,1,1,1,0}, {0,0,1,1,1,1,0,0} };
    int pattern7[4][8] = { {1,1,1,1,1,0,0,0}, {1,1,1,0,0,0,1,1}, {1,0,0,0,1,1,1,1}, {0,0,1,1,1,1,1,0} };
    int pattern8[4][8] = { {0,1,1,1,1,1,0,0}, {1,1,1,1,0,0,0,1}, {1,1,0,0,0,1,1,1}, {0,0,0,1,1,1,1,1} };
    int pattern9[8][8] = { {1,1,1,1,1,1,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,0,0,1,1}, {1,1,1,1,1,0,0,1},
        {1,1,1,0,0,1,1,1}, {1,1,0,0,1,1,1,1}, {1,0,0,1,1,1,1,1}, {0,0,1,1,1,1,1,1} };
    int pattern10[4][8] = { {1,1,1,1,1,1,1,0}, {1,1,1,1,1,0,1,1}, {1,1,1,0,1,1,1,1}, {1,0,1,1,1,1,1,1} };
    // Create unconditional patterns
    int spur[2][8] = { {0,0,1,0,0,0,0}, {1,0,0,0,0,0,0} };
    int single4Connected[2][8] = { {0,0,0,0,0,1,0,0}, {0,0,0,1,0,0,0,0} };
    int Lcluster[8][8] = { {0,0,1,1,0,0,0,0}, {0,1,1,0,0,0,0,0}, {1,1,0,0,0,0,0,0}, {1,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,1,1}, {0,0,0,0,0,1,1,0}, {0,0,0,0,1,1,0,0}, {0,0,0,1,1,0,0,0} };
    int connected4Offset[4][8] = { {0,1,1,0,0,0,0,1}, {1,1,0,1,0,0,0,0}, {0,1,0,1,1,0,0,0}, {0,0,1,1,0,1,0,0} };
    
    int spurextra[2][8] = { {0,0,0,0,1,0,0,0}, {0,0,0,0,0,0,1,0} };
    //STAGE ONE
    while (unequal == 1)
    {
        for (int row = 0; row < height; row++)
        {
            for (int column = 0; column < width; column++)
            {
                first[row][column] = 0; // Initialize everytime so that the previous output is not carried forward
            }
        }
        
        for (int row = 1; row < height-1; row++)
        {
            for (int column = 1; column < width-1; column++)
            {
                // If the pixel is 1 then calculate its bond else leave it unchanged
                if (image[row][column] == 1)
                {
                    bond = (2 * (image[row][column-1] + image[row-1][column] + image[row][column+1] + image[row+1][column])) +
                    (image[row-1][column-1] + image[row-1][column+1] + image[row+1][column-1] + image[row+1][column+1]);
                    
                    int current[8] = {image[row-1][column-1],image[row-1][column],image[row-1][column+1],image[row][column+1],image[row+1][column+1],image[row+1][column],image[row+1][column-1],image[row][column-1]};
                    // Patterns will now be decided by the bond value
                    if (bond == 4)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[3])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[4])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[5])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[6])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[7])))
                            first[row][column] = 1;
                        
                    }
                    else if (bond == 5)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[3])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[4])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[5])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[6])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern5[7])))
                            first[row][column] = 1;
                    }
                    else if (bond == 6)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[3])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[4])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[5])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[6])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[7])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[8])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern6[9])))
                            first[row][column] = 1;
                        
                    }
                    else if (bond == 7)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern7[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern7[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern7[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern7[3])))
                            first[row][column] = 1;
                    }
                    else if (bond == 8)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern8[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern8[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern8[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern8[3])))
                            first[row][column] = 1;
                    }
                    else if (bond == 9)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[3])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[4])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[5])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[6])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern9[7])))
                            first[row][column] = 1;
                    }
                    else if (bond == 10)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern10[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern10[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern10[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern10[3])))
                            first[row][column] = 1;
                    }
                    else first[row][column] = 0;// For bond value not between 4 to 10
                }
                else first[row][column] = 0; //For current pixel not equal to 1
            }
        }
        // STAGE TWO
        for (int row = 0; row < height; row++)
        {
            for (int column = 0; column < width; column++)
            {
                second[row][column] = 1-first[row][column];
            }
        }
        
        for (int row = 1; row < height-1; row++)
        {
            for (int column = 1; column < width-1; column++)
            {
                if (first[row][column] == 1) // Means that it has been marked for deletion
                {
                    // Take the neighboring window
                    int window[8] = {first[row-1][column-1],first[row-1][column],first[row-1][column+1],first[row][column+1],first[row+1][column+1],first[row+1]    [column],first[row+1][column-1],first[row][column-1]};
                    
                    // Spur
                    if (std::equal(std::begin(window), std::end(window), std::begin(spur[0])))                      //1
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(spur[1])))                 //2
                        second[row][column] = 1;
                    
                    // single 4 connected
                    else if (std::equal(std::begin(window), std::end(window), std::begin(single4Connected[0])))     //1
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(single4Connected[1])))     //2
                        second[row][column] = 1;
                    
                    // L cluster
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[0])))             //1
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[1])))             //2
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[2])))             //3
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[3])))             //4
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[4])))             //5
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[5])))             //6
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[6])))             //7
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(Lcluster[7])))             //8
                        second[row][column] = 1;
                    
                    // 4 connected offset
                    else if (std::equal(std::begin(window), std::end(window), std::begin(connected4Offset[0])))             //1
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(connected4Offset[1])))             //2
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(connected4Offset[2])))             //3
                        second[row][column] = 1;
                    else if (std::equal(std::begin(window), std::end(window), std::begin(connected4Offset[3])))             //4
                        second[row][column] = 1;
                    
                    // Spur corner connected
                    else if ( (window[1]|window[3]) & (window[0] == 0) & (window[2] == 1) & (window[4] == 0) &              //1
                             (window[5] == 0) & (window[6] == 1) & (window[7] == 0) )
                        second[row][column] = 1;
                    else if ( (window[1]|window[7]) & (window[0] == 1) & (window[2] == 0) & (window[3] == 0) &              //2
                             (window[4] == 1) & (window[5] == 0) & (window[6] == 0) )
                        second[row][column] = 1;
                    else if ( (window[5]|window[7]) & (window[0] == 0) & (window[1] == 0) & (window[2] == 1) &              //3
                             (window[3] == 0) & (window[4] == 0) & (window[6] == 1) )
                        second[row][column] = 1;
                    else if ( (window[3]|window[5]) & (window[0] == 1) & (window[1] == 0) & (window[2] == 0) &              //4
                             (window[4] == 1) & (window[6] == 0) & (window[7] == 0) )
                        second[row][column] = 1;
                    
                    // Corner Cluster
                    else if ( (window[0] == 1) & (window[1] == 1) & (window[7] == 1) )                                      //1
                        second[row][column] = 1;
                    
                    // Tee Branch
                    else if ( (window[1] == 1) & (window[2] == 0) & (window[3] == 1) & (window[4] == 0) &                   //1
                             (window[5] == 0) & (window[7] == 1))
                        second[row][column] = 1;
                    else if ( (window[0] == 0) & (window[1] == 1) & (window[3] == 1) & (window[5] == 0) &                   //2
                             (window[6] == 0) & (window[7] == 1))
                        second[row][column] = 1;
                    else if ( (window[0] == 0) & (window[1] == 0) & (window[3] == 1) & (window[5] == 1) &                   //3
                             (window[6] == 0) & (window[7] == 1))
                        second[row][column] = 1;
                    else if ( (window[1] == 0) & (window[2] == 0) & (window[3] == 1) & (window[4] == 0) &                   //4
                             (window[5] == 1) & (window[7] == 1))
                        second[row][column] = 1;
                    else if ( (window[1] == 1) & (window[3] == 0) & (window[4] == 0) & (window[5] == 1) &                   //5
                             (window[6] == 0) & (window[7] == 1))
                        second[row][column] = 1;
                    else if ( (window[0] == 0) & (window[1] == 1) & (window[2] == 0) & (window[3] == 0) &                   //6
                             (window[5] == 1) & (window[7] == 1))
                        second[row][column] = 1;
                    else if ( (window[0] == 0) & (window[1] == 1) & (window[2] == 0) & (window[3] == 1) &                   //7
                             (window[5] == 1) & (window[7] == 0))
                        second[row][column] = 1;
                    else if ( (window[1] == 1) & (window[3] == 1) & (window[4] == 0) & (window[5] == 1) &                   //8
                             (window[6] == 0) & (window[7] == 0))
                        second[row][column] = 1;
                    
                    // Vee Branch
                    else if ( (window[4]|window[5]|window[6]) & (window[0] == 1) & (window[2] == 1) )                       //1
                        second[row][column] = 1;
                    else if ( (window[2]|window[3]|window[4]) & (window[0] == 1) & (window[6] == 1) )                       //2
                        second[row][column] = 1;
                    else if ( (window[0]|window[1]|window[2]) & (window[4] == 1) & (window[6] == 1) )                       //3
                        second[row][column] = 1;
                    else if ( (window[0]|window[6]|window[7]) & (window[2] == 1) & (window[4] == 1) )                       //4
                        second[row][column] = 1;
                    
                    // Diagonal Branch
                    else if ( (window[1] == 1) & (window[2] == 0) & (window[3] == 1) & (window[5] == 0) &                   //1
                             (window[6] == 1) & (window[7] == 0) )
                        second[row][column] = 1;
                    else if ( (window[0] == 0) & (window[1] == 1) & (window[3] == 0) & (window[4] == 1) &                   //2
                             (window[5] == 0) & (window[7] == 1) )
                        second[row][column] = 1;
                    else if ( (window[1] == 0) & (window[2] == 1) & (window[3] == 0) & (window[5] == 1) &                   //3
                             (window[6] == 0) & (window[7] == 1) )
                        second[row][column] = 1;
                    else if ( (window[0] == 1) & (window[1] == 0) & (window[3] == 1) & (window[4] == 0) &                   //4
                             (window[5] == 1) & (window[7] == 0) )
                        second[row][column] = 1;
                    
                    // None of the baove patterns matched so now delete the pixel in the image
                    else second[row][column] = 0;
                }
            }
        }
        for (int row = 0; row < height; row++)
        {
            for (int column = 0; column < width; column++)
            {
                modifiedImage[row][column] = image[row][column] * second[row][column];
            }
        }
        count = 0;
        for (int row = 0 ; row < height; row++)
        {
            for (int column = 0; column < width; column++)
            {
                if (modifiedImage[row][column] != image[row][column])
                    count = count+1;
            }
        }
        if (count == 0)
        {
            unequal = 0;
            for (int row = 0; row < height; row++)
            {
                for (int column = 0; column < width; column++)
                {
                    if (modifiedImage[row][column] == 1)
                        finalImage[row][column] = 255;
                    else
                        finalImage[row][column] = 0;
                }
            }
            
        }
        else
            for (int row = 0; row < height; row++)
            {
                for (int column = 0; column < width; column++)
                {
                    image[row][column] = modifiedImage[row][column];
                }
            }
        iteration = iteration + 1;
        
    }
    cout << "The number of Iterations for thinning the Horse image with pre-processing is : " << iteration << endl;
    //Write the shrinked Horseshoe image
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(finalImage, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    return 0;
}

    
