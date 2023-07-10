#include "StageDummy.h"

StageDummy::StageDummy()
{
}

void StageDummy::Initialize()
{
    SetStageType(static_cast<int>(Mame::Stage::TYPE::DUMMY));
}

void StageDummy::Finalize()
{
}

void StageDummy::Begin()
{
}

void StageDummy::Update(const float& elapsedTime)
{
}

void StageDummy::End()
{
}

void StageDummy::Render(const float& elapsedTime)
{
}

void StageDummy::DrawDebug()
{
}
