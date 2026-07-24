#include "TitleScene.h"
#include "WorldTransformUpdate.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void TitleScene::Initialize() {

	// フェードの生成と初期化
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeDuration);
	phase_ = Phase::kFadeIn;

	// 3Dモデルデータの生成
	modelTitleFont_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	// ワールド変換の初期化
	worldTransformTitleFont_.Initialize();
	worldTransformTitleFont_.translation_ = {0.0f, 2.0f, 0.0f};
	worldTransformTitleFont_.scale_ = {1.5f, 1.5f, 1.5f};

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

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		animationTimer_ += 1.0f / 60.0f;

		// タイトル文字と自キャラに動きをつける
		worldTransformTitleFont_.rotation_.y = std::sin(animationTimer_) * 0.15f;
		worldTransformTitleFont_.translation_.y = 2.0f + std::sin(animationTimer_ * 2.0f) * 0.15f;
		worldTransformPlayer_.rotation_.y += 0.02f;
		worldTransformPlayer_.translation_.y = -2.0f + std::sin(animationTimer_ * 3.0f) * 0.2f;

		WorldTransformUpdate(worldTransformTitleFont_);
		WorldTransformUpdate(worldTransformPlayer_);

		// スペースキーでフェードアウトを開始
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, kFadeDuration);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void TitleScene::Draw() {

	Model::PreDraw();

	modelTitleFont_->Draw(worldTransformTitleFont_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	Model::PostDraw();

	// 必ずシーンの最後に描画して最前面に表示する
	fade_->Draw();
}

TitleScene::~TitleScene() {

	delete fade_;
	fade_ = nullptr;

	delete modelTitleFont_;
	modelTitleFont_ = nullptr;

	delete modelPlayer_;
	modelPlayer_ = nullptr;
}
