#pragma once
#include "CameraController.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>
using namespace KamataEngine;

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	~GameScene();

private:
	void GenerateBlocks();

	CameraController* cameraController_ = nullptr;
	Player* player_ = nullptr;

	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// 自キャラ用3Dモデル
	Model* modelPlayer_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;
	// 天球用3Dモデル
	Model* modelSkydome_ = nullptr;

#ifdef _DEBUG
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
#endif
};
