#include <array>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "utils.h"

#define ITER_2D(x,y)                                        \
    for(float x = 0; x < LEN; x += 0.2)                     \
        for (float y = 0; y < LEN - x; y += 0.2)

const int LEN = 20;
const int PADDING = LEN;

Vec3 lightNormal { 0.0, 0.0, 1.0 };

std::array<std::array<std::pair<char, char>, 2 * (LEN + PADDING)>, LEN + PADDING> framebuffer;

Vec3 rotateX(const Vec3& vec, float angle)
{
    Vec3 res;
    res.x = vec.x;
    res.y = vec.y * cos(angle) - vec.z * sin(angle);
    res.z = vec.y * sin(angle) + vec.z * cos(angle);
    return res;
}

Vec3 rotateY(const Vec3& vec, float angle)
{
    Vec3 res;
    res.x = vec.x * cos(angle) + vec.z * sin(angle);
    res.y = vec.y;
    res.z = - vec.x * sin(angle) + vec.z * cos(angle);
    return res;
}

// Optimized combined equation for x and y rotation
Vec3 rotateXY(const Vec3& vec, float x, float y)
{
    float ca = cos(y);
    float sa = sin(y);
    float ct = cos(x);
    float st = sin(x);
    
    Vec3 res;
    res.x = ca * vec.x + vec.z * sa;
    res.y = st * sa * vec.x + ct * vec.y - st * ca * vec.z;
    res.z = - ct * sa * vec.x + st * vec.y + ct * ca * vec.z;
    return res;
}

void draw(const Vec3& pos, const Vec3& normal)
{
    float normPow = (normal.x * lightNormal.x + normal.y * lightNormal.y + normal.z * lightNormal.z);

    char charIndex = (char) ((float)(characters.size() - 1) * (1.0f - normPow) / 2);

    if (framebuffer[(pos.y + LEN + PADDING) / 2][pos.x + LEN + PADDING].second < pos.z)
        framebuffer[(pos.y + LEN + PADDING) / 2][pos.x + LEN + PADDING] = { *(characters.rbegin() + (charIndex > 0 ? charIndex : -charIndex)), pos.z };
}

int main()
{
    lightNormal.x = cos(M_PI / 6);
    lightNormal.y = 0.0f;
    lightNormal.z = sin(M_PI / 6);

    float yRot = 0.0f;
    float xRot = 0.0f;

    const float slantSurfNorm = 1.0 / sqrtf(3);

    for(;;usleep(16000), yRot += 0.03, xRot += 0.05)
    {
        std::fill(&framebuffer[0][0], &framebuffer[0][0] + 2 * (LEN + PADDING) * (LEN + PADDING) , std::make_pair(' ', (char)-100));

        ITER_2D(x, y)
        {
            float z = 0;
            Vec3 norm = rotateXY({0,0,-1}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm);
        }
        
        ITER_2D(y, z)
        {
            float x = 0;
            Vec3 norm = rotateXY({-1,0,0}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm);
        }
        
        ITER_2D(x, z)
        {
            float y = 0;
            Vec3 norm = rotateXY({0,-1,0}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm);
        }

        ITER_2D(x, y)
        {
            float z = (float) LEN - x - y;
            Vec3 norm = rotateXY({slantSurfNorm, slantSurfNorm, slantSurfNorm}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm);
        }

        printf("\x1b[2J");
        printf("\x1b[H");
        for(const auto& rows: framebuffer) {
            for(const auto& x: rows) {
                printf("%c", x.first);
            }
            printf("\n");
        }
    }

    return 0;
}
