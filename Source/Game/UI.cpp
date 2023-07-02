#include "UI.h"

#include "../Mame/Graphics/Graphics.h"

int UI::nameNum = 0;

UI::UI(const wchar_t* filename)
{
    Graphics& graphics = Graphics::Instance();

    uiSprite = std::make_unique<Sprite>(graphics.GetDevice(), filename);

    // imgui名前かぶり起きないように...
    name = "UI" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

UI::~UI()
{
}

void UI::Render()
{
    Graphics& graphics = Graphics::Instance();

    uiSprite->render(graphics.GetDeviceContext(), spr.pos.x, spr.pos.y, spr.size.x, spr.size.y);
}

void UI::DrawDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin(GetName());

    if (ImGui::TreeNode("UI"))
    {
        ImGui::DragFloat2("pos", &spr.pos.x);
        ImGui::DragFloat2("size", &spr.size.x);

        ImGui::TreePop();
    }

    ImGui::End();
#endif // USE_IMGUI
}
