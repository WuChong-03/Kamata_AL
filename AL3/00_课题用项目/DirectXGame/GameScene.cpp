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

	// 自キャラの生成と初期化
	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera_);

	// 天球の生成と初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			// 穴あき配置
			if ((i + j) % 2 == 1) {
				worldTransformBlocks_[i][j] = nullptr;
				continue;
			}

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
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

GameScene::~GameScene() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

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
