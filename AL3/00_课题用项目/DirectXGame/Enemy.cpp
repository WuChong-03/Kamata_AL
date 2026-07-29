#include "Enemy.h"
#include "WorldTransformUpdate.h"
#include <cassert>
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 初期座標を設定
	worldTransform_.translation_ = position;
	// 自キャラと逆向きにして左方向を向かせる
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	// 速度を設定
	velocity_ = {-kWalkSpeed, 0.0f, 0.0f};
	// アニメーション用タイマーを初期化
	walkTimer_ = 0.0f;

	// 行列を更新して定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Enemy::Update() {

	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション
	float param = std::sin(2.0f * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float t = (param + 1.0f) / 2.0f;
	float degree = kWalkMotionAngleStart + (kWalkMotionAngleEnd - kWalkMotionAngleStart) * t;
	worldTransform_.rotation_.x = degree * std::numbers::pi_v<float> / 180.0f;

	// 行列を更新して定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Enemy::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}
