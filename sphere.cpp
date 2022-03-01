#include <array>
#include <iostream>
#include <cmath>
#include <cstring>
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

    std::array<std::array<char, 2 * R>, R> framebuffer;
    memset(framebuffer.data(), ' ', sizeof(framebuffer));
    for(;;usleep(16000), t+=0.02)
    {
        lightNormal.x = sin(t);
        lightNormal.y = cos(t);

        for(float phi = 0.0; phi < 2.0 * M_PI; phi += 0.01)
        {
            for(float theta = -M_PI / 2; theta < M_PI / 2; theta += 0.01)
            {
                // Coordiantes of sphere
                // In this case since origin is the centre of sphere, surface normal = coordianate
                float x = (float) R * sin(theta) * cos(phi);
                float y = (float) R * cos(theta);
                float z = (float) R * sin(theta) * sin(phi);
            
                // Transforming data from range[ -R , R ] to range[ 0, 12 ]
                char charIndex = (int) (x * lightNormal.x + y * lightNormal.y + z * lightNormal.z);
                charIndex = 6 * (charIndex + R) / R;

                framebuffer[(z + R)/2][x + R] = *(characters.rbegin() + (charIndex > 0 ? charIndex : -charIndex));
            }
        }
        printf("\x1b[2J");
        printf("\x1b[H");
        for(const auto& rows: framebuffer) {
            for(const auto& x: rows) {
                printf("%c", x);
            }
            printf("\n");
        }
    }
    // usleep(20000);
    return 0;
}
