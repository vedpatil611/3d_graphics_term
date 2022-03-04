#include <array>
#include <cmath>
#include <iostream>
#include <unistd.h>

#include "utils.h"

const int R = 20;
const int r = 10;
const int PADDING = 4;

Vec3 lightNormal = { -cosf(M_PI / 4), -sinf(M_PI / 4), 0.0f };
std::array<std::array<std::pair<char, char>, 2 * (R + r + PADDING)>, R + r + PADDING> buffer;

int main()
{
    float xRot = 0.0f;

    for(;;usleep(16000), xRot += 0.02)
    {
        std::fill(&buffer[0][0], &buffer[0][0] + 2 * (R + r + PADDING) * (R + r + PADDING), std::make_pair(' ', (char) -100));

        for(float theta = 0.0f; theta < M_PI * 2; theta += 0.03f)
        {
            for(float phi = 0.0f; phi < M_PI * 2; phi += 0.03f)
            {
                float st = sin(theta);
                float ct = cos(theta);
                float sp = sin(phi);
                float cp = cos(phi);
                float sx = sin(xRot);
                float cx = cos(xRot);

                // True coordianate without any transformation applied
                // float x = cp * (R + r * ct);
                // float y = r * st;
                // float z = -sp * (R + r * ct);

                // True normal without any transformations appied
                // float nx = ct * cp;
                // float ny = st;
                // float nz = - sp * ct;

                // coordianates after applying transformations (Rotation about x-axis)
                float x = cp * (R + r * ct);
                float y = r * cx * st + sx * sp * (R + r * ct);
                float z = r * sx * st - cx * sp * (R + r * ct);

                // normals after applying transformations (Rotation about x-axis)
                float nx = cp * ct;
                float ny = cx * st + sx * sp * ct;
                float nz = sx * st - cx * sp * ct;

                float normPow = nx * lightNormal.x + ny * lightNormal.y + nz * lightNormal.z;
                char charIndex = (char) ((float)(characters.size() - 1) * (1.0f - normPow) / 2.0);

                if (buffer[(y + R + r + PADDING) / 2][x + R + r + PADDING].second < z)
                    buffer[(y + R + r + PADDING) / 2][x + R + r + PADDING] = { *(characters.rbegin() + (charIndex > 0 ? charIndex : -charIndex)), z };
            }
        }
        printf("\x1b[2J");
        printf("\x1b[H");
        for(const auto& rows: buffer) {
            for(const auto& x: rows) {
                printf("%c", x.first);
            }
            printf("\n");
        }
    }
    return 0;
}
