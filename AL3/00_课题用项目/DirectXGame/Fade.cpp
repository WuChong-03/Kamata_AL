#include "Fade.h"
#include <algorithm>

using namespace KamataEngine;

void Fade::Initialize() {

	// 白色1x1テクスチャからスプライトを生成
	uint32_t textureHandle = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(textureHandle, {0.0f, 0.0f});

	// 画面全体を覆う大きさにする
	sprite_->SetSize({1280.0f, 720.0f});
	// RGBAはそれぞれ0.0f～1.0fで指定する（完全不透明の黒）
	sprite_->SetColor({0.0f, 0.0f, 0.0f, 1.0f});

	status_ = Status::None;
	duration_ = 0.0f;
	counter_ = 0.0f;
}

void Fade::Update() {

	// フェード状態による分岐
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		counter_ = (std::min)(counter_, duration_);
		// 時間の経過に合わせて黒スプライトを透明にする
		sprite_->SetColor({0.0f, 0.0f, 0.0f, 1.0f - std::clamp(counter_ / duration_, 0.0f, 1.0f)});
		break;
	case Status::FadeOut:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		counter_ = (std::min)(counter_, duration_);
		// 時間の経過に合わせて黒スプライトを不透明にする
		sprite_->SetColor({0.0f, 0.0f, 0.0f, std::clamp(counter_ / duration_, 0.0f, 1.0f)});
		break;
	}
}

void Fade::Draw() {

	// フェード状態でなければ描画しない
	if (status_ == Status::None) {
		return;
	}

	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration) {

	status_ = status;
	duration_ = (std::max)(duration, 0.001f);
	counter_ = 0.0f;

	if (status_ == Status::FadeIn) {
		sprite_->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
	} else if (status_ == Status::FadeOut) {
		sprite_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
	}
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {

	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:
		return counter_ >= duration_;
	case Status::None:
	default:
		return true;
	}
}

Fade::~Fade() {

	delete sprite_;
	sprite_ = nullptr;
}
