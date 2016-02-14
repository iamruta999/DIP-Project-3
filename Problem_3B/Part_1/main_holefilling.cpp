//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 1 Nov 2015

//Problem 3B Part 1
//This code performs hole-filling on the horse image
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main()
{
    char *argv[] = {"Horse1.raw","AllTheEdgesOfHorse.raw","TrueHorseEdge.raw","hole-filledHorse.raw"};

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
    // We first need to convert the image to 0 and 1
    int image[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (originalImage[row][column] == 255)
                image[row][column] = 1;
            else
                image[row][column] = 0;
        }
    }
    
    // Input this image to the first stage to create conditional Mark patterns
    unsigned char first[height][width];
    int bond;
    int count;
    
    // Create conditional pattern tables for bonds from 1 to 10
    int pattern1[4][8] = { {0,0,1,0,0,0,0,0}, {1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,1,0,0,0} };
    int pattern2[4][8] = { {0,0,0,1,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1}, {0,0,0,0,0,1,0,0} };
    int pattern3[8][8] = { {0,0,1,1,0,0,0,0}, {0,1,1,0,0,0,0,0}, {1,1,0,0,0,0,0,0}, {1,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,1,1}, {0,0,0,0,0,1,1,0}, {0,0,0,0,1,1,0,0}, {0,0,0,1,1,0,0,0} };
    int pattern4[4][8] = { {0,0,1,1,1,0,0,0}, {1,1,1,0,0,0,0,0}, {1,0,0,0,0,0,1,1}, {0,0,0,0,1,1,1,0} };
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
    
    //STAGE ONE
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
                    if (bond == 1)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern1[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern1[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern1[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern1[3])))
                            first[row][column] = 1;
                    }
                    else if (bond == 2)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern2[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern2[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern2[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern2[3])))
                            first[row][column] = 1;
                    }
                    else if (bond == 3)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[3])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[4])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[5])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[6])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern3[7])))
                            first[row][column] = 1;
                    }
                    else if (bond == 4)
                    {
                        if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[0])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[1])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[2])))
                            first[row][column] = 1;
                        else if (std::equal(std::begin(current), std::end(current), std::begin(pattern4[3])))
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
                    else first[row][column] = 0;// For bond value not between 1 to 10
                }
                else first[row][column] = 0; //For current pixel not equal to 1   
            }
        }
    
            for (int row = 0; row < height; row++)
            {
                for (int column = 0; column < width; column++)
                {
                    if (first[row][column] == 1)
                        first[row][column] = 255;
                }
            }
        int column = 358;
        for (int row = 231; row < 237; row++)
        first[row][column] = 255;
    
            //Write the shrinked Horseshoe image
            if (!(file=fopen(argv[1],"wb")))
            {
                cout << "Cannot open file: " << argv[1] << endl;
                exit(1);
            }
            fwrite(first, sizeof(unsigned char), height*width, file);
            fclose(file);
    // Now get the true edge
    unsigned char trueEdge[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            trueEdge[row][column] = 0;
            if (first[row][column] == 255)
                first[row][column] = 1;
        }
    }
    int currentRow, currentColumn;
    int iteration = 0;
    int flag = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (first[i][j] == 1)
            {
                flag = 1;
                currentRow = i;
                currentColumn = j;
                trueEdge[currentRow][currentColumn] = 255;
            }
            if (flag == 1)
                break;
        }
        if (flag == 1)
            break;
    }
    
    while ( flag == 1 )
    {
        if (iteration == 0) // Starting now
        {
            if (first[currentRow][currentColumn+1] == 1)
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow;
                currentColumn = currentColumn + 1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow+1][currentColumn+1] == 1)
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow + 1;
                currentColumn = currentColumn + 1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow+1][currentColumn] == 1)
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow + 1;
                currentColumn = currentColumn;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
        }
        else
        {
            
            if (first[currentRow][currentColumn+1] == 1)//Right
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow;
                currentColumn = currentColumn + 1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow-1][currentColumn+1] == 1)//top-right
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow - 1;
                currentColumn = currentColumn + 1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow-1][currentColumn] == 1)//Top
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow - 1;
                currentColumn = currentColumn;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow+1][currentColumn+1] == 1)//bottom-right
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow + 1;
                currentColumn = currentColumn + 1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            
            
            else if (first[currentRow+1][currentColumn] == 1)//Bottom
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow + 1;
                currentColumn = currentColumn;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow+1][currentColumn-1] == 1)//Bottom-left
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow + 1;
                currentColumn = currentColumn-1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow][currentColumn-1] == 1)//Left
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow;
                currentColumn = currentColumn-1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else if (first[currentRow-1][currentColumn-1] == 1)//top-left
            {
                first[currentRow][currentColumn] = 0;
                currentRow = currentRow - 1;
                currentColumn = currentColumn - 1;
                trueEdge[currentRow][currentColumn] = 255;
                iteration = iteration + 1;
            }
            
            else
                
                flag = 0;
            
        }
    }
    //Write the edge Horseshoe image
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(trueEdge, sizeof(unsigned char), height*width, file);
	fclose(file);
    
    // Now compare the first edge with true edge and then fill in the holes of horse
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (first[row][column] != trueEdge[row][column])
            {
                originalImage[row-1][column-1] = 255;
                originalImage[row-1][column] = 255;
                originalImage[row-1][column+1] = 255;
                originalImage[row][column-1] = 255;
                originalImage[row][column+1] = 255;
                originalImage[row+1][column-1] = 255;
                originalImage[row+1][column] = 255;
                originalImage[row+1][column+1] = 255;
            }
        }
    }
    
    count = 0;
    int N = 5;
    int halfLength = (N-1)/2;
    int window[N][N];
    for (int iteration = 0; iteration < 4; iteration++)
    {
        for (int row = 0; row < height; row++)
        {
            for (int column = 0; column < width; column++)
            {
                if (originalImage[row][column] == 0)
                {
                    count = 0;
                    //Get the N * N window
                    for ( int i = 0; i < N; i++)
                    {
                        for ( int j = 0; j < N; j++)
                        {
                            window[i][j] = originalImage[row + (i - halfLength)][column + (j - halfLength)];
                            if (window[i][j] == 255)
                                count = count+1;
                        }
                    }
                    
                    if (count > 15)
                        originalImage[row][column] = 255;
                }
            }
        }
    }

    //Write the hole-filled horse image
    if (!(file=fopen(argv[3],"wb")))
    {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(originalImage, sizeof(unsigned char), height*width, file);
	fclose(file);
    


    
            return 0;
        }

    
