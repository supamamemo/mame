#pragma once

// ���������Z�p
class Mathf
{
public:
    // ���`�⊮(a�F�ϓ��l�@b�F�ڕW�l�@t�F�⊮�l�i��]���x�j(0.0f�`1.0f))
    static float Lerp(const float& a, const float& b, const float& t);

    // �w��͈͂̃����_���l���v�Z����
    static float RandomRange(const float& min, const float& max);
};

