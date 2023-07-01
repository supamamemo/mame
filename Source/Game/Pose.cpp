#include "Pose.h"

#include "../Mame/Graphics/Graphics.h"

Pose::Pose(const wchar_t* filename)
{
    Graphics& graphics = Graphics::Instance();

    sprite = std::make_unique<Sprite>(graphics.GetDevice(), filename);
}

Pose::~Pose()
{
}

void Pose::Render()
{
    Graphics& graphics = Graphics::Instance();
    sprite->render(graphics.GetDeviceContext(), spr.pos.x, spr.pos.y, spr.texPos.x, spr.texPos.y, materialColor);
}
