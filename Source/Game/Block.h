#pragma once

#include "../Mame/Graphics/Model.h"

class Block
{
public:
    Block() {}
    virtual ~Block() {}

    virtual void DrawDebug();

public:
    std::unique_ptr<Model> model;
};