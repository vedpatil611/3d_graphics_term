#include <array>
#include <iostream>
#include <cmath>
#include <unistd.h>

#define ITER_2D(x,y)                                        \
    for(float x = -H_LEN; x < H_LEN; x += 0.2)              \
        for (float y = -H_LEN; y < H_LEN; y += 0.2)

struct Vec3
{
    float x, y, z;
};

const std::array<char, 12> characters = { '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@' };
const int LEN = 20;
const int H_LEN = LEN / 2;
const int PADDING = LEN;

Vec3 lightNormal { 0.0, 0.0, 1.0 };

std::array<std::array<std::pair<char, char>, LEN + PADDING>, H_LEN + PADDING / 2> framebuffer;

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

constexpr float deg2rad(float degree)
{
    return degree * M_PI / 180.0; 
}

void draw(const Vec3& pos, const Vec3& normal)
{
    float normPow = (normal.x * lightNormal.x + normal.y * lightNormal.y + normal.z * lightNormal.z);

    char charIndex = (char) ((float)(characters.size() - 1) * (1.0f - normPow) / 2);

    if (framebuffer[(pos.y + H_LEN + PADDING / 2) / 2][pos.x + H_LEN + PADDING / 2].second < pos.z)
        framebuffer[(pos.y + H_LEN + PADDING / 2) / 2][pos.x + H_LEN + PADDING / 2] = { *(characters.rbegin() + (charIndex > 0 ? charIndex : -charIndex)), pos.z };
}

int main()
{
    lightNormal.x = cos(M_PI / 6);
    lightNormal.y = 0.0f;
    lightNormal.z = sin(M_PI / 6);

    float yRot = 0.0f;
    float xRot = - deg2rad(30.0);

    for(;;usleep(16000), yRot += 0.03)
    {
        std::fill(&framebuffer[0][0], &framebuffer[0][0] + (LEN + PADDING) * (H_LEN + PADDING / 2) , std::make_pair(' ', (char)-100));

        ITER_2D(x, y)
        {
            float z = -H_LEN;
            Vec3 norm = rotateX(rotateY({0,0,-1}, yRot), xRot);
            draw(rotateX(rotateY({x,y,z}, yRot), xRot), norm);

            z = H_LEN;
            norm = rotateX(rotateY({0,0,1}, yRot), xRot);
            draw(rotateX(rotateY({x,y,z}, yRot), xRot), norm);
        }
        
        ITER_2D(y, z)
        {
            float x = -H_LEN;
            Vec3 norm = rotateX(rotateY({-1,0,0}, yRot), xRot);
            draw(rotateX(rotateY({x,y,z}, yRot), xRot), norm);
 
            x = H_LEN;
            norm = rotateX(rotateY({1,0,0}, yRot), xRot);
            draw(rotateX(rotateY({x,y,z}, yRot), xRot), norm);
        }
        
        ITER_2D(x, z)
        {
            float y = -H_LEN;
            Vec3 norm = rotateX(rotateY({0,-1,0}, yRot), xRot);
            draw(rotateX(rotateY({x,y,z}, yRot), xRot), norm);
 
            y = H_LEN;
            norm = rotateX(rotateY({0,1,0}, yRot), xRot);
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
