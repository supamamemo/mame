#pragma once
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

private:
    // ���݂̃X�e�[�g
    State* currentState = nullptr;
    // �e�X�e�[�g��ێ�����z��
    std::vector<State*> statePool;

};

