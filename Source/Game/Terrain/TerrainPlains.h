#pragma once

#include "Terrain.h"

class TerrainPlains : public Terrain
{
public:
    TerrainPlains(const char* const filename);
    ~TerrainPlains() override;

    void Initialize()                       override; // ‰Šú‰»
    void Finalize()                         override; // I—¹‰»
    void Update(const float& elapsedTime)   override; // XVˆ—
    void Render(const float& elapsedTime)   override; // •`‰æˆ—
    void DrawDebug()                        override;
};

