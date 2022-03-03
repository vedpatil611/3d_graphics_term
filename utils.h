struct Vec3
{
    float x, y, z;
};

constexpr float deg2rad(float degree)
{
    return degree * M_PI / 180.0; 
}

const std::array<char, 12> characters = { '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@' };
