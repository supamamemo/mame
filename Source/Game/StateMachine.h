#pragma once

#ifdef USE_IMGUI
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_internal.h"
#include "../../External/imgui/imgui_impl_dx11.h"
#include "../../External/imgui/imgui_impl_win32.h"
#endif

#include <vector>
#include "State.h"

// �X�e�[�g�}�V�[��
class StateMachine
{
public:
    StateMachine() {};
    ~StateMachine();

    // �X�V����
    void Update(float elapsedTime);

    // �X�e�[�g�Z�b�g
    void SetState(int newState);

    // �X�e�[�g�ύX
    void ChangeState(int newState);

    // �X�e�[�g�o�^
    void RegisterState(State* state);

    // ���݂̃X�e�[�g�ԍ��擾
    int GetStateIndex();

    // imgui�p
    void DrawDebug();

    // ��
    void SetMoveRight(bool r) { moveRight = r; }
    bool GetMoveRight() { return moveRight; }

private:
    // ���݂̃X�e�[�g
    State* currentState = nullptr;
    // �e�X�e�[�g��ێ�����z��
    std::vector<State*> statePool;

    // ���ɂ����ɒu���܂�
    bool moveRight = false;
};

