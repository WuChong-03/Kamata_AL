#include "GameScene.h"
#include "WorldTransformUpdate.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// フェードの生成と初期化
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeDuration);

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
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

	// 敵の生成と初期化
	const float enemyPositionXs[] = {18.0f, 26.0f};
	for (float enemyPositionX : enemyPositionXs) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = playerPosition;
		enemyPosition.x += enemyPositionX;
		newEnemy->Initialize(modelEnemy_, &cameraController_->GetCamera(), enemyPosition);
		enemies_.push_back(newEnemy);
	}

	// カメラコントローラに追従対象をセット
	cameraController_->SetTarget(player_);
	// カメラを追従対象へ瞬間合わせ
	cameraController_->Reset();

	// 天球の生成と初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// ブロックの生成
	GenerateBlocks();
	// フェードイン中にも正しい位置で表示できるように更新
	skydome_->Update();
	UpdateBlocks();

	// フェードインフェーズから開始
	phase_ = Phase::kFadeIn;
	finished_ = false;
}

void GameScene::Update() {

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		break;
	case Phase::kPlay:
		UpdatePlayPhase();
		break;
	case Phase::kDeath:
		UpdateDeathPhase();
		break;
	case Phase::kFadeOut:
		fade_->Update();
		break;
	}

	// フェーズの切り替え
	ChangePhase();
}

void GameScene::UpdatePlayPhase() {

	player_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	skydome_->Update();

	// 全ての当たり判定を行う
	CheckAllCollisions();

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

	UpdateBlocks();
}

void GameScene::UpdateDeathPhase() {

	// 天球の更新
	skydome_->Update();

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// デスパーティクルの更新
	if (deathParticles_) {
		deathParticles_->Update();
	}

	UpdateBlocks();
}

void GameScene::UpdateBlocks() {

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

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kFadeIn:
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kPlay;
		}
		break;
	case Phase::kPlay:
		if (player_->IsDead()) {
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// 自キャラの座標にデスパーティクルを発生、初期化
			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelDeathParticle_, &cameraController_->GetCamera(), deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, kFadeDuration);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
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
	if (phase_ == Phase::kFadeIn || phase_ == Phase::kPlay) {
		player_->Draw();
	}

	// 敵の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// デスパーティクルの描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();

	// 必ずシーンの最後に描画して最前面に表示する
	fade_->Draw();
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

void GameScene::CheckAllCollisions() {

	if (!player_) {
		return;
	}

#pragma region 自キャラと敵キャラの当たり判定
	AABB aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
		if (!enemy) {
			continue;
		}

		AABB aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 今回は確認用にprintだけ行う
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}
#pragma endregion
}

GameScene::~GameScene() {

	delete fade_;
	fade_ = nullptr;

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

	delete deathParticles_;
	deathParticles_ = nullptr;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	delete cameraController_;
	cameraController_ = nullptr;

	delete skydome_;
	skydome_ = nullptr;

	delete modelPlayer_;
	modelPlayer_ = nullptr;

	delete modelEnemy_;
	modelEnemy_ = nullptr;

	delete modelDeathParticle_;
	modelDeathParticle_ = nullptr;

	delete modelBlock_;
	modelBlock_ = nullptr;

	delete modelSkydome_;
	modelSkydome_ = nullptr;

#ifdef _DEBUG
	delete debugCamera_;
	debugCamera_ = nullptr;
#endif
}
