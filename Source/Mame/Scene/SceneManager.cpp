#include "SceneManager.h"

#include "../Graphics/Graphics.h"

#include "../Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

namespace Mame::Scene
{
    // 初期化
    void SceneManager::Initialize()
    {        
        sprite[0] = std::make_unique<Pose>(L"./resources/pose/pose.png");
        sprite[1] = std::make_unique<Pose>(L"./resources/pose/returnWhite.png");
        sprite[2] = std::make_unique<Pose>(L"./resources/pose/return.png");
        sprite[3] = std::make_unique<Pose>(L"./resources/pose/arrowWhite.png");
        sprite[4] = std::make_unique<Pose>(L"./resources/pose/arrow.png");
        sprite[5] = std::make_unique<Pose>(L"./resources/pose/poseTitle.png");

        sprite[1]->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));
        sprite[3]->SetMaterialColor(DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f));
    }

    // 更新処理
    void SceneManager::Update(float elapsedTime)
    {
        if (nextScene)
        {
            // 古いシーンを終了
            Clear();

            // 新しいシーンを設定
            currentScene = nextScene;
            nextScene = nullptr;

            // シーン初期化処理(マルチスレッド処理をしていない場合に行う)
            if (!currentScene->IsReady())
            {
                currentScene->Initialize();
            }
        }

        if (currentScene)
        {
            //bool hitStopTiming = false;
            if (hitStopTimer_ > 0.0f)
            {
                hitStopTimer_ -= elapsedTime;
                //hitStopTiming = (hitStopTimer_ > 0.8f) || (static_cast<int>(hitStopTimer_ * 10.0f) % 2 == 0);
            } 
            else
            {
                isHitStop_ = false;
            }

            // pose中じゃない
            //if (!GetPose() && !hitStopTiming)
            if (!GetPose() && !isHitStop_)
            {
                currentScene->Begin();
                currentScene->Update(elapsedTime);
                currentScene->End();
            }
            // pose中
            else
            {
                switch (currentScene->GetSceneType())
                {
                    // タイトル
                case static_cast<int>(Mame::Scene::TYPE::TITLE):
                    PoseUpdateTitle(elapsedTime);
                    break;

                    // ゲーム
                case static_cast<int>(Mame::Scene::TYPE::GAME):
                    PoseUpdateGame(elapsedTime);
                    break;
                }
            }

#ifdef USE_IMGUI
            currentScene->DrawDebug();
            DrawDebug();
#endif

            // pose切り替え処理
            GamePad& gamePad = Input::Instance().GetGamePad();
            if (gamePad.GetButtonDown() & (GamePad::BTN_START | GamePad::BTN_BACK))
            {
                // sceneLoadingじゃなかったら処理する
                if(currentScene->GetSceneType()!=static_cast<int>(Mame::Scene::TYPE::LOAD))
                    Mame::Scene::SceneManager::Instance().SetPose(Mame::Scene::SceneManager::Instance().GetPose() ? false : true);
            }
        }
    }

    // 描画処理
    void SceneManager::Render(float elapsedTime)
    {
        if (!currentScene)return;

        currentScene->Render(elapsedTime);    

        if (!GetPose())return;
        
        switch (currentScene->GetSceneType())
        {
            // タイトル
        case static_cast<int>(Mame::Scene::TYPE::TITLE):
            PoseRenderTitle(elapsedTime);
            break;

            // ゲーム
        case static_cast<int>(Mame::Scene::TYPE::GAME):
            PoseRenderGame(elapsedTime);
            break;
        }
    }

    // imgui
    void SceneManager::DrawDebug()
    {
        ImGui::Begin("sceneManager");

        ImGui::SliderInt("rasterize", &RS, 0, 3);
        ImGui::SliderInt("depthstencil", &DS, 0, 3);
        ImGui::SliderInt("samplerstate", &SS, 0, 2);

        ImGui::DragInt("state", &state);

        if (GetPose())ImGui::Text("pose");

        ImGui::End();
    }

    // シーンクリア
    void SceneManager::Clear()
    {
        if (!currentScene)return;
        
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }

    // pose中のupdate(sceneTitle)
    void SceneManager::PoseUpdateTitle(float elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        switch (state)
        {
        case 0:
            // 表示の設定
            sprite[1]->SetIndication(true);
            sprite[3]->SetIndication(false);

            // スティック左に傾けたら
            {
                float aLx = gamePad.GetAxisLX();
                float aRx = gamePad.GetAxisRX();

                if (aLx < 0 || aRx < 0)state = 1;
            }

            // pose解除
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                state = 0;
                Mame::Scene::SceneManager::Instance().SetPose(false);
            }
            break;
        case 1:
            // 表示の設定
            sprite[1]->SetIndication(false);
            sprite[3]->SetIndication(true);

            // スティック右に傾けたら
            {
                float aLx = gamePad.GetAxisLX();
                float aRx = gamePad.GetAxisRX();

                if (aLx > 0 || aRx > 0)state = 0;
            }

            // 終了
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                PostQuitMessage(0);
            }

            break;
        }
    }

    // pose中のupdate(sceneGame)
    void SceneManager::PoseUpdateGame(float elapsedTime)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();

        switch (state)
        {
            // "もどる"を選択中
        case 0:
            // 表示の設定
            sprite[1]->SetIndication(true);
            sprite[3]->SetIndication(false);
            
            // スティック左に傾けたら
            {
                float aLx = gamePad.GetAxisLX();
                float aRx = gamePad.GetAxisRX();
                
                if (aLx < 0 || aRx < 0)state = 1;
            }

            // pose解除
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                state = 0;
                Mame::Scene::SceneManager::Instance().SetPose(false);
            }

            break;
            // "たいとる"を選択中
        case 1:
            // 表示の設定
            sprite[1]->SetIndication(false);
            sprite[3]->SetIndication(true);

            // スティック右に傾けたら
            {
                float aLx = gamePad.GetAxisLX();
                float aRx = gamePad.GetAxisRX();

                if (aLx > 0 || aRx > 0)state = 0;
            }

            // タイトルに戻る処理
            if (gamePad.GetButtonDown() & GamePad::BTN_A)
            {
                state = 0;
                Mame::Scene::SceneManager::Instance().SetPose(false);
                ChangeScene(new SceneTitle);
            }

            break;
        }
    }

    // pose中のupdate(sceneTitle)
    void SceneManager::PoseRenderTitle(float elapsedTime)
    {
        Graphics& graphics = Graphics::Instance();
        Shader* shader = graphics.GetShader();
        // 2Dsprite描画設定
        shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);

        if (sprite[5]->IsIndication())sprite[5]->Render();
        for (int i = 1; i < 5; ++i)
        {
            if (sprite[i]->IsIndication())sprite[i]->Render();
        }
    }


    // pose中のrender(sceneGame)
    void SceneManager::PoseRenderGame(float elapsedTime)
    {
        Graphics& graphics = Graphics::Instance();
        Shader* shader = graphics.GetShader();
        // 2Dsprite描画設定
        shader->SetState(graphics.GetDeviceContext(), 3, 0, 0);
        
        for (int i = 0; i < 5; ++i)
        {
            if (sprite[i]->IsIndication())sprite[i]->Render();
        }
    }


    // シーン切り替え
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // 新しいシーンを設定
        nextScene = scene;
    }
}