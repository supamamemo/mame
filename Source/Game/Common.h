#pragma once

// マクロ定数
// const不要の目印
#define NO_CONST 


// 関数ポインタ
// ラジアン変換関数ポインタ
extern float (* const ToRadian)(float); 


// inline関数
// ポインタ消去関数
template <typename T>
inline void safe_delete(T*& p)
{
    if (!p) return;

    delete p;
    p = nullptr;
}