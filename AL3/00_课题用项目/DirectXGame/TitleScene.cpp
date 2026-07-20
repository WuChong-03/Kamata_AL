#include "TitleScene.h"
#include "WorldTransformUpdate.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void TitleScene::Initialize() {

	// 3Dモデルデータの生成
	modelTitleFont_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	// ワールド変換の初期化
	worldTransformTitleFont_.Initialize();
	worldTransformTitleFont_.translation_ = {0.0f, 2.0f, 0.0f};
	worldTransformTitleFont_.scale_ = {0.8f, 0.8f, 0.8f};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.translation_ = {0.0f, -2.0f, 0.0f};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;

	// カメラの初期化
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -15.0f};
	camera_.UpdateMatrix();

	animationTimer_ = 0.0f;
	finished_ = false;

	WorldTransformUpdate(worldTransformTitleFont_);
	WorldTransformUpdate(worldTransformPlayer_);
}

void TitleScene::Update() {

	animationTimer_ += 1.0f / 60.0f;

	// タイトル文字と自キャラに動きをつける
	worldTransformTitleFont_.rotation_.y = std::sin(animationTimer_) * 0.15f;
	worldTransformTitleFont_.translation_.y = 2.0f + std::sin(animationTimer_ * 2.0f) * 0.15f;
	worldTransformPlayer_.rotation_.y += 0.02f;
	worldTransformPlayer_.translation_.y = -2.0f + std::sin(animationTimer_ * 3.0f) * 0.2f;

	WorldTransformUpdate(worldTransformTitleFont_);
	WorldTransformUpdate(worldTransformPlayer_);

	// スペースキーでタイトルシーンを終了
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {

	Model::PreDraw();

	modelTitleFont_->Draw(worldTransformTitleFont_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	Model::PostDraw();
}

TitleScene::~TitleScene() {

	delete modelTitleFont_;
	modelTitleFont_ = nullptr;

	delete modelPlayer_;
	modelPlayer_ = nullptr;
}
