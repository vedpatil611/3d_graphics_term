#include <array>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <unistd.h>
#include "utils.h"

#define ITER_2D(x,y)                                        \
    for(float x = -H_LEN; x < H_LEN; x += 0.2)              \
        for (float y = -H_LEN; y < H_LEN; y += 0.2)

const int LEN = 32;
const int H_LEN = LEN / 2;
const int PADDING = LEN;

Vec3 lightNormal { 0.0, 0.0, 1.0 };

struct BufferData
{
    char character;
    char depth;
    char color;

    BufferData() {}
    BufferData(char character, char depth, char color)
        :character(character), depth(depth), color(color) {}
};

std::array<std::array<BufferData, LEN + PADDING>, H_LEN + PADDING / 2> framebuffer;
std::unordered_map<char, const char*> colorMap;

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

void draw(const Vec3& pos, const Vec3& normal, char color)
{
    float normPow = (normal.x * lightNormal.x + normal.y * lightNormal.y + normal.z * lightNormal.z);

    char charIndex = (char) ((float)(characters.size() - 1) * (1.0f - normPow) / 2);

    if (framebuffer[(pos.y + H_LEN + PADDING / 2) / 2][pos.x + H_LEN + PADDING / 2].depth < pos.z)
        framebuffer[(pos.y + H_LEN + PADDING / 2) / 2][pos.x + H_LEN + PADDING / 2] = { *(characters.rbegin() + (charIndex > 0 ? charIndex : -charIndex)), (char) pos.z, color };
}

int main()
{
    colorMap = std::unordered_map<char, const char*> {
        { 0, "\e[0;31m" },      // RED
        { 1, "\e[0;32m" },      // GREEN
        { 2, "\e[0;33m" },      // YELLOW
        { 3, "\e[0;34m" },      // BLUE
        { 4, "\e[0;35m" },      // MAGENTA
        { 5, "\e[0;36m" }       // CYAN
    };

    lightNormal.x = cos(M_PI / 6);
    lightNormal.y = 0.0f;
    lightNormal.z = sin(M_PI / 6);

    float yRot = 0.0f;
    float xRot = - deg2rad(30.0);

    for(;;usleep(16000), yRot += 0.03)
    {
        std::fill(&framebuffer[0][0], &framebuffer[0][0] + (LEN + PADDING) * (H_LEN + PADDING / 2) , BufferData(' ', (char)-100, 0));

        ITER_2D(x, y)
        {
            float z = -H_LEN;
            Vec3 norm = rotateXY({0,0,-1}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm, 0);
            
            z = H_LEN;
            norm = rotateXY({0,0,1}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm, 1);
        }
        
        ITER_2D(y, z)
        {
            float x = -H_LEN;
            Vec3 norm = rotateXY({-1,0,0}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm, 2);
 
            x = H_LEN;
            norm = rotateXY({1,0,0}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm, 3);
        }
        
        ITER_2D(x, z)
        {
            float y = -H_LEN;
            Vec3 norm = rotateXY({0,-1,0}, xRot, yRot);
            draw(rotateXY({x,y,z}, xRot, yRot), norm, 4);
 
            y = H_LEN;
            // This draw call is not required because bottom surface is never rendered
            // Vec3 norm = rotateXY({0,1,0}, xRot, yRot);
            // draw(rotateXY({x,y,z}, xRot, yRot), norm);
        }

        printf("\x1b[2J");
        printf("\x1b[H");
        for(const auto& rows: framebuffer) {
            for(const auto& x: rows) {
                printf("%s%c", colorMap[x.color], x.character);
            }
            printf("\n");
        }
    }

    return 0;
}
