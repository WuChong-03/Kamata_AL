#include "GameScene.h"

using namespace KamataEngine;
using namespace KamataEngine::MathUtility;

void GameScene::Initialize() {

	// player貼图
	modelTextureHandle_ = TextureManager::Load("test/image.png");
	model_ = Model::Create();

	// 3Dモデルデータの生成
	modelBlock_ = Model::Create();

	// 摄像机创建和初始化
	camera_.Initialize();

#ifdef _DEBUG
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
#endif

	// 创建实例和初始化
	player_ = new Player();
	player_->Initialize(model_, modelTextureHandle_, &camera_);

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

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

#ifdef _DEBUG
	// デバッグカメラの切り替え
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
#ifdef _DEBUG
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
#endif
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			// アフィン変換行列の作成
			worldTransformBlock->matWorld_ = MakeScaleMatrix(worldTransformBlock->scale_) * MakeRotateXMatrix(worldTransformBlock->rotation_.x) * MakeRotateYMatrix(worldTransformBlock->rotation_.y) *
			                                 MakeRotateZMatrix(worldTransformBlock->rotation_.z) * MakeTranslateMatrix(worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {

	Model::PreDraw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

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

	delete model_;
	model_ = nullptr;

	delete modelBlock_;
	modelBlock_ = nullptr;

	delete debugCamera_;
	debugCamera_ = nullptr;
}
