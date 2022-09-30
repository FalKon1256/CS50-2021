#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            image[i][j].rgbtBlue = round(sum / 3);
            image[i][j].rgbtGreen = round(sum / 3);
            image[i][j].rgbtRed = round(sum / 3);
        }
    }
    return;
}


// Reflect image horizontally

void swap(RGBTRIPLE *a, RGBTRIPLE *b);

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            swap(&image[i][j], &image[i][(width - 1) - j]);
        }
    }
    return;
}

void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Zero padding orginal array value to convolution input (same mode)

    RGBTRIPLE(*tmp)[width + 2] = calloc((height + 2), (width + 2) * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // tmp is a (height + 2) x (width + 2) array (edge gird values are 0)
            tmp[i + 1][j + 1] = image[i][j];
        }
    }
    
    // Filter (Kernel) is a 3 x 3 array filled with 1
    int g[3][3] = { { 1, 1, 1 },
        { 1, 1, 1 },
        { 1, 1, 1 }
    };

    // Apply convolution by Filter (Kernel)
    for (int i = 1; i < height  + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            // Avoid uint8_t(BYTE) data truncation
            float sumRed = 0,
                  sumGreen = 0,
                  sumBlue = 0;
            
            // Sum up surrounding grids per unit grid (inclide each unit grid per se)
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    sumRed += g[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtRed;
                    sumGreen += g[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtGreen;
                    sumBlue += g[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtBlue;
                }
            }

            // Get feature map
            
            // 9-grids center case
            if ((0 < i - 1) && (i - 1 < height - 1) && (0 < j - 1) && (j - 1 < width - 1))
            {
                image[i - 1][j - 1].rgbtRed = round(sumRed / 9);
                image[i - 1][j - 1].rgbtGreen = round(sumGreen / 9);
                image[i - 1][j - 1].rgbtBlue = round(sumBlue / 9);
            }
            
            // 4-grids corner case
            else if (((i - 1 == 0) && (j - 1 == 0)) || ((i - 1 == 0) && (j - 1 == width - 1)) || ((i - 1 == height - 1) && (j - 1 == 0))
                     || ((i - 1 == height - 1) && (j - 1 == width - 1)))
            {
                image[i - 1][j - 1].rgbtRed = round(sumRed / 4);
                image[i - 1][j - 1].rgbtGreen = round(sumGreen / 4);
                image[i - 1][j - 1].rgbtBlue = round(sumBlue / 4);
            }
            
            // 6-grids edge case
            else
            {
                image[i - 1][j - 1].rgbtRed = round(sumRed / 6);
                image[i - 1][j - 1].rgbtGreen = round(sumGreen / 6);
                image[i - 1][j - 1].rgbtBlue = round(sumBlue / 6);
            }
        }
    }
    free(tmp);
    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Zero padding orginal array value to convolution input (same mode)

    RGBTRIPLE(*tmp)[width + 2] = calloc((height + 2), (width + 2) * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // tmp is a (height + 2) x (width + 2) array (edge gird values are 0)
            tmp[i + 1][j + 1] = image[i][j];
        }
    }

    // Filters (Kernel) gx, gy
    int gX[3][3] = { { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };
                     
    int gY[3][3] = { { -1, -2, -1 },
        {  0,  0, 0 },
        {  1,  2, 1 }
    };

    // Apply convolution by Filter (Kernel)
    for (int i = 1; i < height  + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            // X-direction case: Sum up surrounding grids per unit grid (inclide each unit grid per se)
            float sumRedX = 0,
                  sumGreenX = 0,
                  sumBlueX = 0;
            
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    int a = gX[k][l];
                    int b = tmp[i + (k - 1)][j + (l - 1)].rgbtRed;

                    sumRedX += gX[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtRed;
                    sumGreenX += gX[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtGreen;
                    sumBlueX += gX[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtBlue;
                }
            }
            
            // Y-direction case: Sum up surrounding grids per unit grid (inclide each unit grid per se)
            float sumRedY = 0,
                  sumGreenY = 0,
                  sumBlueY = 0;
            
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    sumRedY += gY[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtRed;
                    sumGreenY += gY[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtGreen;
                    sumBlueY += gY[k][l] * tmp[i + (k - 1)][j + (l - 1)].rgbtBlue;
                }
            }
            
            // Take average by Sobel filter algorithm (no division)
            int avgRed = round(sqrtf(powf(round(sumRedX), 2) + powf(round(sumRedY), 2))),
                avgGreen = round(sqrtf(powf(round(sumGreenX), 2) + powf(round(sumGreenY), 2))),
                avgBlue = round(sqrtf(powf(round(sumBlueX), 2) + powf(round(sumBlueY), 2)));
            
            // Value capping to 255 as upper limit
            if (avgRed > 255)
            {
                avgRed = 255;
            }
            if (avgGreen > 255)
            {
                avgGreen = 255;
            }
            if (avgBlue > 255)
            {
                avgBlue = 255;
            }
            
            // Get feature map
            image[i - 1][j - 1].rgbtRed = avgRed;
            image[i - 1][j - 1].rgbtGreen = avgGreen;
            image[i - 1][j - 1].rgbtBlue = avgBlue;
        }
    }
    free(tmp);
    return;
}