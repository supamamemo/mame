#pragma once

// �}�N���萔
// const�s�v�̖ڈ�
#define NO_CONST 


// �֐��|�C���^
// ���W�A���ϊ��֐��|�C���^
extern float (* const ToRadian)(float); 


// inline�֐�
// �|�C���^�����֐�
template <typename T>
inline void safe_delete(T*& p)
{
    if (!p) return;

    delete p;
    p = nullptr;
}