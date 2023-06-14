#include "Mathf.h"
#include <stdlib.h>

float Mathf::Lerp(const float& a, const float& b, const float& t)
{
    return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(const float& min, const float& max)
{
    // 0.0�`1.0�܂ł̃����_���l
    const float randValue = static_cast<float>(rand()) / RAND_MAX;

    // min�`max�܂ł̃����_���l�ɕϊ�
    return min + (max - min) * randValue;
}
