#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_internal.h"
//#include "../../imgui/imgui.h"
//#include "../../imgui/imgui_internal.h"

class Transform
{
public:
	enum class CoordinateSystem
	{
		cRightYup,
		cLeftYup,
		cRightZup,
		cLeftZup,
		cNone,
	};

	Transform() :
		position(DirectX::XMFLOAT3())
		, scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
		, rotation(DirectX::XMFLOAT3())
		, scaleFactor(1.0f)
		, coordinateSystem(static_cast<int>(CoordinateSystem::cRightYup))
	{}
    ~Transform() {}

	void DrawDebug();
	void Reset();
    
    DirectX::XMMATRIX CalcWorld();
	DirectX::XMMATRIX CalcWorldMatrix(float scale_factor);
	DirectX::XMFLOAT3 CalcForward()const;
	DirectX::XMFLOAT3 CalcUp()const;
	DirectX::XMFLOAT3 CalcRight()const;

	DirectX::XMFLOAT3 GetPosition()const { return position; }
	DirectX::XMFLOAT3 GetScale()const { return scale; }
	DirectX::XMFLOAT3 GetRotation()const { return rotation; }

	void SetPosition(const DirectX::XMFLOAT3& pos) { position = pos; }
	void SetScale(const DirectX::XMFLOAT3& scl) { scale = scl; }
	void SetRotation(const DirectX::XMFLOAT3& rot) { rotation = rot; }

	void AddPosition(const DirectX::XMFLOAT3& pos)
	{
		position.x += pos.x;
		position.y += pos.y;
		position.z += pos.z;
	}

	float GetScaleFactor()const { return scaleFactor; }
	void SetScaleFactor(float f) { scaleFactor = f; }

private:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT3 rotation;

	float scaleFactor;

	int coordinateSystem;
	const DirectX::XMFLOAT4X4 CoordinateSystemTransforms[static_cast<int>(CoordinateSystem::cNone)]
	{
		{
			-1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1,
		},

		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		},

		{
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		},
		{
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		}
	};

	const std::string coordinateSystemName[static_cast<int>(CoordinateSystem::cNone)]
	{
		"Right Hand, Y up",
		"Left Hand, Y up",
		"Right Hand, Z up",
		"Left Hand, Z up"
	};
};
