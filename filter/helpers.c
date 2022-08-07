#include "helpers.h"
#include <math.h>

typedef struct {
    int a;
    int b;
}
par;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Determine the scale of gray to make the new pixel
            // Calculate the average (step by step)
            float a = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            float b = a / 3;
            int c = round(b);
            image[i][j].rgbtBlue = c;
            image[i][j].rgbtGreen = c;
            image[i][j].rgbtRed = c;

            /* Why this one line calculation of average didn't work?
            int a = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            */
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Pares para pixeles
    par pares[9];
    pares[0].a = -1, pares[0].b = -1;
    pares[1].a = -1, pares[1].b = 0;
    pares[2].a = -1, pares[2].b = 1;
    pares[3].a = -0, pares[3].b = -1;
    pares[4].a = 0, pares[4].b = 0;
    pares[5].a = 0, pares[5].b = 1;
    pares[6].a = 1, pares[6].b = -1;
    pares[7].a = 1, pares[7].b = 0;
    pares[8].a = 1, pares[8].b = 1;

    // Sumas de los pixeles para el algoritmo
    float sr = 0, sb = 0, sg = 0;

    // La algebra de los pixeles
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Esquinas
            if (i - 1 = -1) {
                if (j - 1 = -1) {
                    // Estoy en la esq. sup. izq. /4
                }
                else if (j + 1 = width) {
                    // Estoy en la esq. sup. der. /4
                }
                else {
                    // Estoy en fila sup. centrica. /6
                }
            else if (i + 1 = height)
            }
            for (int k = 0; k < 9; k++) {
                sr += image[i + pares[k].a][j + pares[k].b].rgbtRed;
                sb += image[i + pares[k].a][j + pares[k].b].rgbtBlue;
                sg += image[i + pares[k].a][j + pares[k].b].rgbtGreen;
            }

            image[i][j].rgbtRed = round(sr / 9);
            image[i][j].rgbtBlue = round(sb / 9);
            image[i][j].rgbtGreen = round(sg / 9);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the whole image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Declare the Gx convolutional matrix
    int gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    // Declare the Gy convolutional matrix
    int gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // Yup
    float gxred = 0, gxgreen = 0, gxblue = 0, gyred = 0, gygreen = 0, gyblue = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute the new value of each pixel by taking a weighted sum of the values for the surrounding pixels.
            // And since edges between objects could take place in both a vertical and a horizontal direction, you’ll actually compute two weighted sums:
            // one for detecting edges in the x direction, and one for detecting edges in the y direction

            // For each of the three color values for each pixel, we’ll compute two values Gx and Gy
            // To compute Gx for the red channel value of a pixel, for instance, we’ll take the original red values for the nine pixels that form a 3x3 box around the pixel,
            // multiply them each by the corresponding value in the Gx kernel, and take the sum of the resulting values.

            gxred = image[i][j].rgbtRed;
            gxgreen = image[i][j].rgbtGreen;
            gxblue = image[i][j].rgbtBlue;

            gyred = image[i][j].rgbtRed;
            gygreen = image[i][j].rgbtGreen;
            gyblue = image[i][j].rgbtBlue;

            // Combine Gx and Gy into a single value
            // (combinecolor = ccolor)
            float cred = gxred*gxred + gyred*gyred;
            float cgreen = gxgreen*gxgreen + gygreen*gygreen;
            float cblue = gxblue*gxblue + gyblue*gyblue;

            // Channel values can only take on integer values from 0 to 255, be sure the resulting value is rounded to the nearest integer and capped to 255.
            // (roundedcolor = rcolor)
            int rred = round(cred);
            int rgreen = round(cgreen);
            int rblue = round(cblue);

            if (rred > 255)
                rred = 255;
            if (rgreen > 255)
                rgreen = 255;
            if (rblue > 255)
                rblue = 255;

            // And what about handling pixels at the edge, or in the corner of the image?
            // Treat the image as if there was a 1 pixel solid black border around the edge of the image: values of 0 for each of red, green, and blue
        }
    }
    return;
}




/* BLUR FILTER
    // Copiamos toda la imagen original en una nueva.
    RGBTRIPLE copia[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copia[i][j] = image[i][j];
        }
    }

    // Yup
    float sumared = 0, sumablue = 0, sumagreen = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // CORNERS
            // Casos esquina inferior izq
            if (i - 1 == -1 && j - 1 == -1)
            {
                sumared = copia[i][j].rgbtRed + copia[i][j + 1].rgbtRed + copia[i + 1][j].rgbtRed + copia[i + 1][j + 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i][j + 1].rgbtGreen + copia[i + 1][j].rgbtGreen + copia[i + 1][j + 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i][j + 1].rgbtBlue + copia[i + 1][j].rgbtBlue + copia[i + 1][j + 1].rgbtBlue;
            }

            // Casos esquina inferior der
            else if (i - 1 == -1 && j + 1 == width)
            {
                sumared = copia[i][j].rgbtRed + copia[i][j - 1].rgbtRed + copia[i + 1][j].rgbtRed + copia[i + 1][j - 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i][j - 1].rgbtGreen + copia[i + 1][j].rgbtGreen + copia[i + 1][j - 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i][j - 1].rgbtBlue + copia[i + 1][j].rgbtBlue + copia[i + 1][j - 1].rgbtBlue;
            }

            // Casos esquina superior izq
            else if (i + 1 == height && j - 1 == -1)
            {
                sumared = copia[i][j].rgbtRed + copia[i][j + 1].rgbtRed + copia[i - 1][j].rgbtRed + copia[i - 1][j + 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i][j + 1].rgbtGreen + copia[i - 1][j].rgbtGreen + copia[i - 1][j + 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i][j + 1].rgbtBlue + copia[i - 1][j].rgbtBlue + copia[i - 1][j + 1].rgbtBlue;
            }

            // Casos esquina superior der
            else if (i + 1 == height && j + 1 == width)
            {
                sumared = copia[i][j].rgbtRed + copia[i][j - 1].rgbtRed + copia[i - 1][j].rgbtRed + copia[i - 1][j - 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i][j - 1].rgbtGreen + copia[i - 1][j].rgbtGreen + copia[i - 1][j - 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i][j - 1].rgbtBlue + copia[i - 1][j].rgbtBlue + copia[i - 1][j - 1].rgbtBlue;
            }

            // EDGES
            // LEFT SIDE EDGE
            else if (j - 1 == -1)
            {
                sumared = copia[i][j].rgbtRed + copia[i][j + 1].rgbtRed + copia[i + 1][j].rgbtRed + copia[i - 1][j].rgbtRed + copia[i + 1][j + 1].rgbtRed + copia[i - 1][j + 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i][j + 1].rgbtGreen + copia[i + 1][j].rgbtGreen + copia[i - 1][j].rgbtGreen + copia[i + 1][j + 1].rgbtGreen + copia[i - 1][j + 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i][j + 1].rgbtBlue + copia[i + 1][j].rgbtBlue + copia[i - 1][j].rgbtBlue + copia[i + 1][j + 1].rgbtBlue + copia[i - 1][j + 1].rgbtBlue;
            }

            // RIGHT SIDE EDGE
            else if (j + 1 == width)
            {
                sumared = copia[i][j].rgbtRed + copia[i + 1][j].rgbtRed + copia[i - 1][j].rgbtRed + copia[i - 1][j - 1].rgbtRed + copia[i][j - 1].rgbtRed + copia[i + 1][j - 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i + 1][j].rgbtGreen + copia[i - 1][j].rgbtGreen + copia[i - 1][j - 1].rgbtGreen + copia[i][j - 1].rgbtGreen + copia[i + 1][j - 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i + 1][j].rgbtBlue + copia[i - 1][j].rgbtBlue + copia[i - 1][j - 1].rgbtBlue + copia[i][j - 1].rgbtBlue + copia[i + 1][j - 1].rgbtBlue;

            }

            // UPPER EDGE
            else if (i - 1 == -1)
            {
                sumared = copia[i][j].rgbtRed + copia[i + 1][j].rgbtRed + copia[i][j - 1].rgbtRed + copia[i + 1][j - 1].rgbtRed + copia[i][j + 1].rgbtRed + copia[i + 1][j + 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i + 1][j].rgbtGreen + copia[i][j - 1].rgbtGreen + copia[i + 1][j - 1].rgbtGreen + copia[i][j + 1].rgbtGreen + copia[i + 1][j + 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i + 1][j].rgbtBlue + copia[i][j - 1].rgbtBlue + copia[i + 1][j - 1].rgbtBlue + copia[i][j + 1].rgbtBlue + copia[i + 1][j + 1].rgbtBlue;
            }

            // BOTTOM EDGE
            else if (i + 1 == height)
            {
                sumared = copia[i][j].rgbtRed + copia[i][j - 1].rgbtRed + copia[i][j + 1].rgbtRed + copia[i - 1][j - 1].rgbtRed + copia[i - 1][j].rgbtRed + copia[i - 1][j + 1].rgbtRed;
                sumagreen = copia[i][j].rgbtGreen + copia[i][j - 1].rgbtGreen + copia[i][j + 1].rgbtGreen + copia[i - 1][j - 1].rgbtGreen + copia[i - 1][j].rgbtGreen + copia[i - 1][j + 1].rgbtGreen;
                sumablue = copia[i][j].rgbtBlue + copia[i][j - 1].rgbtBlue + copia[i][j + 1].rgbtBlue + copia[i - 1][j - 1].rgbtBlue + copia[i - 1][j].rgbtBlue + copia[i - 1][j + 1].rgbtBlue;

            }

            // CENTERS
            else
            {
                sumared = copia[i - 1][j - 1].rgbtRed + copia[i - 1][j].rgbtRed + copia[i - 1][j + 1].rgbtRed + copia[i][j - 1].rgbtRed + copia[i][j].rgbtRed + copia[i][j + 1].rgbtRed + copia[i + 1][j - 1].rgbtRed + copia[i + 1][j].rgbtRed + copia[i + 1][j + 1].rgbtRed;
                sumagreen = copia[i - 1][j - 1].rgbtGreen + copia[i - 1][j].rgbtGreen + copia[i - 1][j + 1].rgbtGreen + copia[i][j - 1].rgbtGreen + copia[i][j].rgbtGreen + copia[i][j + 1].rgbtGreen + copia[i + 1][j - 1].rgbtGreen + copia[i + 1][j].rgbtGreen + copia[i + 1][j + 1].rgbtGreen;
                sumablue = copia[i - 1][j - 1].rgbtBlue + copia[i - 1][j].rgbtBlue + copia[i - 1][j + 1].rgbtBlue + copia[i][j - 1].rgbtBlue + copia[i][j].rgbtBlue + copia[i][j + 1].rgbtBlue + copia[i + 1][j - 1].rgbtBlue + copia[i + 1][j].rgbtBlue + copia[i + 1][j + 1].rgbtBlue;
            }

            // NEW IF MOTHERFUCKERS yEAH!
            if ((i - 1 == -1 && j - 1 == -1) || (i - 1 == -1 && j + 1 == width) || (i + 1 == height && j - 1 == -1) || (i + 1 == height && j + 1 == width))
            {
                sumared = sumared / 4;
                sumagreen = sumagreen / 4;
                sumablue = sumablue / 4;
            }
            else if ((j - 1 == -1) || (j + 1 == width) || (i - 1 == -1) || (i + 1 == height))
            {
                sumared = sumared / 6;
                sumagreen = sumagreen / 6;
                sumablue = sumablue / 6;
            }
            else
            {
                sumared = sumared / 9;
                sumagreen = sumagreen / 9;
                sumablue = sumablue / 9;
            }

            image[i][j].rgbtRed = round(sumared);
            image[i][j].rgbtGreen = round(sumagreen);
            image[i][j].rgbtBlue = round(sumablue);
        }
    }
    return;
*/