#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <list>
#include <vector>
using namespace KamataEngine;

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	~GameScene();
	// 終了フラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// ゲームのフェーズ
	enum class Phase {
		kPlay,
		kDeath,
	};

	void GenerateBlocks();
	void CheckAllCollisions();
	void UpdatePlayPhase();
	void UpdateDeathPhase();
	void UpdateBlocks();
	void ChangePhase();

	// ゲームの現在フェーズ
	Phase phase_ = Phase::kPlay;
	// 終了フラグ
	bool finished_ = false;

	CameraController* cameraController_ = nullptr;
	Player* player_ = nullptr;
	std::list<Enemy*> enemies_;

	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// 自キャラ用3Dモデル
	Model* modelPlayer_ = nullptr;
	// 敵用3Dモデル
	Model* modelEnemy_ = nullptr;
	// デスパーティクル用3Dモデル
	Model* modelDeathParticle_ = nullptr;
	// デスパーティクル
	DeathParticles* deathParticles_ = nullptr;

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
