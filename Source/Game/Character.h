#pragma once

#include "../Mame/Graphics/Model.h"

#ifdef USE_IMGUI
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_internal.h"
#include "../../External/imgui/imgui_impl_dx11.h"
#include "../../External/imgui/imgui_impl_win32.h"
//#include "../../imgui/imgui.h"
//#include "../../imgui/imgui_internal.h"
//#include "../../imgui/imgui_impl_dx11.h"
//#include "../../imgui/imgui_impl_win32.h"
#endif

class Character
{
public:
    Character() {}
    virtual ~Character() {}

    virtual void DrawDebug();
    
public:
    Model* model = nullptr;
};

