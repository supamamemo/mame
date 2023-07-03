#include "BaseScene.h"

namespace Mame::Scene
{
    void BaseScene::ChangeStage(int stage)
    {
        switch (stage)
        {
        case static_cast<int>(Mame::Scene::STAGE::Tutorial):
            changeStageTutorial = true;
            break;
        case static_cast<int>(Mame::Scene::STAGE::Plains):
            changeStagePlains = true;
            break;
        case static_cast<int>(Mame::Scene::STAGE::Boss):
            changeStageBoss = true;
            break;
        }
    }
}