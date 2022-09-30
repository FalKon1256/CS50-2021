#include "helpers.h"
#include <math.h>
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


// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiaRed = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue,
                  sepiaGreen = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue,
                  sepiaBlue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            
            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);
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
    RGBTRIPLE tmp[height][width];

    // Copy orginal array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    // Blur function

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Index of 9 grids a unit (center)
            if ((0 < i) && (i < height - 1) && (0 < j) && (j < width - 1))
            {
                // Avoid uint8_t(BYTE) data truncation
                float sumRed = 0,
                      sumGreen = 0,
                      sumBlue = 0;
                for (int k = -1; k < 2; k++)
                {
                    sumRed += tmp[i + k][j - 1].rgbtRed + tmp[i + k][j].rgbtRed + tmp[i + k][j + 1].rgbtRed;
                    sumGreen += tmp[i + k][j - 1].rgbtGreen + tmp[i + k][j].rgbtGreen + tmp[i + k][j + 1].rgbtGreen;
                    sumBlue += tmp[i + k][j - 1].rgbtBlue + tmp[i + k][j].rgbtBlue + tmp[i + k][j + 1].rgbtBlue;
                }
                image[i][j].rgbtRed = round(sumRed / 9);
                image[i][j].rgbtGreen = round(sumGreen / 9);
                image[i][j].rgbtBlue = round(sumBlue / 9);
            }


            // Index of 6 vertical grids a unit (edge)
            else if (((j == 0) || (j == width - 1)) && (0 < i) && (i < height - 1))
            {
                float sumRed = 0,
                      sumGreen = 0,
                      sumBlue = 0;
                if (j == 0)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i - 1][j + k].rgbtRed + tmp[i][j + k].rgbtRed + tmp[i + 1][j + k].rgbtRed;
                        sumGreen += tmp[i - 1][j + k].rgbtGreen + tmp[i][j + k].rgbtGreen + tmp[i + 1][j + k].rgbtGreen;
                        sumBlue += tmp[i - 1][j + k].rgbtBlue + tmp[i][j + k].rgbtBlue + tmp[i + 1][j + k].rgbtBlue;
                    }
                }

                else if (j == width - 1)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i - 1][j - k].rgbtRed + tmp[i][j - k].rgbtRed + tmp[i + 1][j - k].rgbtRed;
                        sumGreen += tmp[i - 1][j - k].rgbtGreen + tmp[i][j - k].rgbtGreen + tmp[i + 1][j - k].rgbtGreen;
                        sumBlue += tmp[i - 1][j - k].rgbtBlue + tmp[i][j - k].rgbtBlue + tmp[i + 1][j - k].rgbtBlue;
                    }
                }
                image[i][j].rgbtRed = round(sumRed / 6);
                image[i][j].rgbtGreen = round(sumGreen / 6);
                image[i][j].rgbtBlue = round(sumBlue / 6);
            }

            // Index of 6 horizontal grids a unit (edge)
            else if (((i == 0) || (i == height - 1)) && (0 < j) && (j < width - 1))
            {
                float sumRed = 0,
                      sumGreen = 0,
                      sumBlue = 0;
                if (i == 0)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i + k][j - 1].rgbtRed + tmp[i + k][j].rgbtRed + tmp[i + k][j + 1].rgbtRed;
                        sumGreen += tmp[i + k][j - 1].rgbtGreen + tmp[i + k][j].rgbtGreen + tmp[i + k][j + 1].rgbtGreen;
                        sumBlue += tmp[i + k][j - 1].rgbtBlue + tmp[i + k][j].rgbtBlue + tmp[i + k][j + 1].rgbtBlue;
                    }
                }

                else if (i == height - 1)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i - k][j - 1].rgbtRed + tmp[i - k][j].rgbtRed + tmp[i - k][j + 1].rgbtRed;
                        sumGreen += tmp[i - k][j - 1].rgbtGreen + tmp[i - k][j].rgbtGreen + tmp[i - k][j + 1].rgbtGreen;
                        sumBlue += tmp[i - k][j - 1].rgbtBlue + tmp[i - k][j].rgbtBlue + tmp[i - k][j + 1].rgbtBlue;
                    }
                }
                // Average and round value
                image[i][j].rgbtRed = round(sumRed / 6);
                image[i][j].rgbtGreen = round(sumGreen / 6);
                image[i][j].rgbtBlue = round(sumBlue / 6);
            }

            // Index of 4 grids a unit (corner)
            else
            {
                float sumRed = 0,
                      sumGreen = 0,
                      sumBlue = 0;
                if (i == 0 && j == 0)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i + k][j].rgbtRed + tmp[i + k][j + 1].rgbtRed;
                        sumGreen += tmp[i + k][j].rgbtGreen + tmp[i + k][j + 1].rgbtGreen;
                        sumBlue += tmp[i + k][j].rgbtBlue + tmp[i + k][j + 1].rgbtBlue;
                    }
                }

                else if (i == 0 && j == width - 1)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i + k][j - 1].rgbtRed + tmp[i + k][j].rgbtRed;
                        sumGreen += tmp[i + k][j - 1].rgbtGreen + tmp[i + k][j].rgbtGreen;
                        sumBlue += tmp[i + k][j - 1].rgbtBlue + tmp[i + k][j].rgbtBlue;
                    }
                }

                else if (i == height - 1 && j == 0)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i - k][j].rgbtRed + tmp[i - k][j + 1].rgbtRed;
                        sumGreen += tmp[i - k][j].rgbtGreen + tmp[i - k][j + 1].rgbtGreen;
                        sumBlue += tmp[i - k][j].rgbtBlue + tmp[i - k][j + 1].rgbtBlue;
                    }
                }

                else if (i == height - 1 && j == width - 1)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        sumRed += tmp[i - k][j - 1].rgbtRed + tmp[i - k][j].rgbtRed;
                        sumGreen += tmp[i - k][j - 1].rgbtGreen + tmp[i - k][j].rgbtGreen;
                        sumBlue += tmp[i - k][j - 1].rgbtBlue + tmp[i - k][j].rgbtBlue;
                    }
                }
                // Average and round value
                image[i][j].rgbtRed = round(sumRed / 4);
                image[i][j].rgbtGreen = round(sumGreen / 4);
                image[i][j].rgbtBlue = round(sumBlue / 4);
            }
        }
    }
    return;
}
