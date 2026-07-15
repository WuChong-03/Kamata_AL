#include "GameScene.h"
#include "WorldTransformUpdate.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelBlock_ = Model::CreateFromOBJ("block", true);
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);

	// カメラコントローラの生成と初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();

#ifdef _DEBUG
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	debugCamera_->SetFarZ(2000.0f);
#endif

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// カメラ移動範囲の指定
	CameraController::Rect cameraArea = {
	    12.0f,
	    MapChipField::kBlockWidth * MapChipField::kNumBlockHorizontal - 12.0f,
	    0.0f,
	    MapChipField::kBlockHeight * MapChipField::kNumBlockVirtical,
	};
	cameraController_->SetMovableArea(cameraArea);

	// 自キャラの生成と初期化
	player_ = new Player();
	const uint32_t kPlayerStartX = 1;
	const uint32_t kPlayerStartY = 18;
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(kPlayerStartX, kPlayerStartY);
	player_->Initialize(modelPlayer_, &cameraController_->GetCamera(), playerPosition);
	player_->SetMapChipField(mapChipField_);

	// カメラコントローラに追従対象をセット
	cameraController_->SetTarget(player_);
	// カメラを追従対象へ瞬間合わせ
	cameraController_->Reset();

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
		Camera& camera = cameraController_->GetCamera();
		camera.matView = debugCamera_->GetCamera().matView;
		camera.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera.TransferMatrix();
	} else {
		// カメラコントローラの更新
		cameraController_->Update();
	}
#else
	// カメラコントローラの更新
	cameraController_->Update();
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

	Camera& camera = cameraController_->GetCamera();

	// 天球の描画
	skydome_->Draw(camera);

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			modelBlock_->Draw(*worldTransformBlock, camera);
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

	delete cameraController_;
	cameraController_ = nullptr;

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
