#pragma once

#include "Player.h"

class PlayerManager
{
private:
    PlayerManager()  {}
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);  // �X�V����
    void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime);  // �`�揈��
    void DrawDebug();                       // �f�o�b�O�`��

    // �v���C���[�擾
    NO_CONST std::unique_ptr<Player>& GetPlayer() { return player; }

    void UpdateSelectStage(const float& elapsedTime,int* state);

private:
    std::unique_ptr<Player> player = nullptr;

};

