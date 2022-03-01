#include <array>
#include <iostream>
#include <cmath>
#include <cstring>
#include <utility>
#include <unistd.h>

struct Vec3
{
    float x, y, z;
};

int main()
{
    const std::array<char, 12> characters = { '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@' };
    const int R = 30;
    Vec3 lightNormal = { 0.0, -1.0, 0.0 };
    float t = 0.0f;

    // First element of pair is dot product of surface normal and light normal
    // Second element is depth value
    std::array<std::array<std::pair<char, char>, 2 * R>, R> framebuffer;

    for(;;usleep(16000), t+=0.02)
    {
        std::fill(&framebuffer[0][0], &framebuffer[0][0] + 2 * R * R, std::make_pair(' ', (char)0));
        
        // Rotating light normal
        lightNormal.x = sin(t);
        lightNormal.y = cos(t);

        for(float phi = 0.0; phi < 2.0 * M_PI; phi += 0.02)
        {
            for(float theta = 0; theta < 2.0 * M_PI; theta += 0.02)
            {
                // Coordiantes of sphere
                // In this case since origin is the centre of sphere, surface normal = coordianate
                float x = (float) R * sin(theta) * cos(phi);
                float y = (float) R * cos(theta);
                float z = (float) R * sin(theta) * sin(phi);
            
                // Transforming data from range[ -R , R ] to range[ 0, 12 ]
                char charIndex = (int) (x * lightNormal.x + y * lightNormal.y + z * lightNormal.z);
                charIndex = characters.size() - 1 - 6 * (charIndex + R) / R;

                if(framebuffer[(z + R) / 2][x + R].second < y)
                    framebuffer[(z + R) / 2][x + R] = { *(characters.rbegin() + (charIndex > 0 ? charIndex : -charIndex)), y };
            }
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
