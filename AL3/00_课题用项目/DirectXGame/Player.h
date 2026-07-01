#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle, Camera* camera);

	void Update();

	void Draw();

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Camera* camera_ = nullptr;
};
