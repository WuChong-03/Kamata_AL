#include "Player.h"
#include "WorldTransformUpdate.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;

namespace {

float EaseInOut(float t) {
	t = std::clamp(t, 0.0f, 1.0f);
	return t * t * (3.0f - 2.0f * t);
}

} // namespace

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	onGround_ = true;

	// 行列を更新して定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Player::Update() {

	// 移動入力
	// 接地状態
	if (onGround_) {
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				// 右向きに変更
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;

				// 左向きに変更
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
		}
	} else {
		// 落下速度
		velocity_.y -= kGravityAcceleration;
		// 落下速度制限
		velocity_.y = (std::max)(velocity_.y, -kLimitFallSpeed);
	}

	// 旋回制御
	{
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,
		};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<int>(lrDirection_)];

		// 自キャラの角度を設定する
		if (turnTimer_ > 0.0f) {
			// 旋回タイマーを1/60秒だけカウントダウンする
			turnTimer_ = (std::max)(turnTimer_ - (1.0f / 60.0f), 0.0f);
			float t = 1.0f - (turnTimer_ / kTimeTurn);
			float easedT = EaseInOut(t);
			worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * easedT;
		} else {
			worldTransform_.rotation_.y = destinationRotationY;
		}
	}

	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		bool landing = false;

		// 地面との当たり判定
		// 下降中
		if (velocity_.y < 0.0f) {
			// Y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= kGroundHeight) {
				landing = true;
			}
		}

		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = kGroundHeight;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// 行列を更新して定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }