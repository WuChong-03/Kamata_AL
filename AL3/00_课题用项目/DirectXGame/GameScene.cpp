#include "GameScene.h"
#include "WorldTransformUpdate.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelBlock_ = Model::CreateFromOBJ("block", true);
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);

	// カメラの初期化
	camera_.farZ = 2000.0f;
	camera_.Initialize();
	camera_.UpdateMatrix();

#ifdef _DEBUG
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	debugCamera_->SetFarZ(2000.0f);
#endif

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成と初期化
	player_ = new Player();
	const uint32_t kPlayerStartX = 1;
	const uint32_t kPlayerStartY = 18;
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(kPlayerStartX, kPlayerStartY);
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	// 天球の生成と初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// ブロックの生成
	GenerateBlocks();
}

void GameScene::Update() {

	player_->Update();
	skydome_->Update();

#ifdef _DEBUG
	// デバッグカメラの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}
#else
	// ビュープロジェクション行列の更新と転送
	camera_.UpdateMatrix();
#endif

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			// 行列を更新して定数バッファに転送
			WorldTransformUpdate(*worldTransformBlock);
		}
	}
}

void GameScene::Draw() {

	Model::PreDraw();

	// 天球の描画
	skydome_->Draw(camera_);

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 自キャラの描画
	player_->Draw();

	Model::PostDraw();
}

void GameScene::GenerateBlocks() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

GameScene::~GameScene() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete mapChipField_;
	mapChipField_ = nullptr;

	delete player_;
	player_ = nullptr;

	delete skydome_;
	skydome_ = nullptr;

	delete modelPlayer_;
	modelPlayer_ = nullptr;

	delete modelBlock_;
	modelBlock_ = nullptr;

	delete modelSkydome_;
	modelSkydome_ = nullptr;

#ifdef _DEBUG
	delete debugCamera_;
	debugCamera_ = nullptr;
#endif
}
