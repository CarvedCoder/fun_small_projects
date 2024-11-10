#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
float A, B, C;
float cubeWidth = 18;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int BackgroundASCII = ' ';
int idx;
float K1 = 40;
float incrementSpeed = 1;
int distanceFromCam = 120;
float x, y, z;
float ooz;
float xp, yp;
float calculateX(int i, int j, int k)
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}
float calculateY(int i, int j, int k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) - k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}
float calculateZ(int i, int j, int k)
{
    return k * cos(B) * cos(A) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch)
{
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;
    ooz = 1 / z;
    xp = (int)(width / 2 - 2 * cubeWidth + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);
    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height)
    {
        if (ooz > zBuffer[idx])
        {
            buffer[idx] = ch;
            zBuffer[idx] = ooz;
        }
    }
}

int main()
{
    printf("\x1b[2J");
    while (1)
    {
        memset(buffer, BackgroundASCII, height * width);
        memset(zBuffer, 0, height * width * 4);
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '#');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '*');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '-');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < height * width; k++)
        {
            putchar(k % width ? buffer[(int)k] : 10);
        }
        A += 0.005;
        B += 0.005;
        usleep(100);
    }
}