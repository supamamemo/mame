#pragma once

class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    virtual void Initialize() = 0;  // ������
    virtual void Finalize() = 0;    // �I����
    virtual void Begin() = 0;       // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime) = 0;      // �X�V����
    virtual void End() = 0;         // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& elapsedTime) = 0; // �`�揈��
    virtual void DrawDebug() = 0;
};

