#pragma once
#include "Enemy.h"
#include "CannonBallManager.h"

// ‘å–C
class EnemyCannon : public Enemy
{
public:
    EnemyCannon();
    ~EnemyCannon() override;

    void Initialize()               override; // ‰Šú‰»
    void Finalize()                 override; // I—¹‰»
    void Begin()                    override; // –ˆƒtƒŒ[ƒ€ˆê”ÔÅ‰‚ÉŒÄ‚Î‚ê‚é
    void Update(float elapsedTime)  override; // XVˆ—
    void End()                      override; // –ˆƒtƒŒ[ƒ€ˆê”ÔÅŒã‚ÉŒÄ‚Î‚ê‚é
    void Render(float elapsedTime)  override; // •`‰æˆ—
    void DrawDebug()                override;

public:
    // Imgui–¼‘O”í‚è–h~‚Ì‚½‚ß‚Ì‚â‚Â //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui–¼‘O”í‚è–h~‚Ì‚½‚ß‚Ì‚â‚Â //

private:
    std::string name = {};};

