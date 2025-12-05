#pragma once

class MathUtils
{
public:
    template <typename T>
    static T clamp(T value, T min_val, T max_val)
    {
        return (value < min_val) ? min_val : (value > max_val) ? max_val : value;
    }
};
