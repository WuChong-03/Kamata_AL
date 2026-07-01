#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
using namespace KamataEngine;

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	~GameScene();

private:
	uint32_t modelTextureHandle_ = 0;
	Model* model_ = nullptr;
	Camera camera_;
	Player* player_ = nullptr;

	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
};
