#include "StateMachine.h"


// �f�X�g���N�^
StateMachine::~StateMachine()
{
    // �o�^����t�X�e�[�g���폜����
    for (State* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

// �X�V����
void StateMachine::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}

// �X�e�[�g�Z�b�g
void StateMachine::SetState(int newState)
{
    currentState = statePool.at(newState);
    currentState->Enter();
}

// �X�e�[�g�ύX
void StateMachine::ChangeState(int newState)
{
    currentState->Exit();
    SetState(newState);
}

// �X�e�[�g�o�^
void StateMachine::RegisterState(State* state)
{
    statePool.emplace_back(state);
}

// �X�e�[�g�ԍ��擾
int StateMachine::GetStateIndex()
{
    int i = 0;
    for (State* state : statePool)
    {
        if (state == currentState)
        {
            // i�ԍ��ڂ̃X�e�[�g�����^�[��
            return i;
        }
        ++i;
    }

    // �X�e�[�g��������Ȃ������Ƃ�
    return -1;
}

// debug�p
void StateMachine::DrawDebug()
{
#if USE_IMGUI
    if (ImGui::TreeNode("stateMachine"))
    {
        ImGui::Text(currentState->GetName());

        float temp = currentState->GetTimer();
        ImGui::DragFloat("time", &temp);

        //float moveSpeed = currentState->GetMoveSpeed();
        //ImGui::DragFloat("moveSpeed", &moveSpeed);

/*        float rotSpeed = currentState->GetRotationSpeed();
        ImGui::DragFloat("rotationSpeed", &rotSpeed)*/;

        ImGui::TreePop();
    }
#endif // USE_IMGUI
}
