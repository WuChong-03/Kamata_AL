#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <cassert>

using namespace KamataEngine;

namespace {

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	t = std::clamp(t, 0.0f, 1.0f);
	return {
	    a.x + (b.x - a.x) * t,
	    a.y + (b.y - a.y) * t,
	    a.z + (b.z - a.z) * t,
	};
}

} // namespace

void CameraController::Initialize() {

	// カメラの初期化
	camera_.farZ = 2000.0f;
	camera_.Initialize();
	camera_.translation_.y = kTargetCameraY;
	targetDestination_ = camera_.translation_;
	camera_.UpdateMatrix();
}

void CameraController::Update() {

	if (target_) {
		// 追従対象のワールドトランスフォームを参照
		const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
		const Vector3& targetVelocity = target_->GetVelocity();

		// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
		targetDestination_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
		targetDestination_.y = kTargetCameraY;
		targetDestination_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;
	}

	// 座標補間によりゆったり追従
	camera_.translation_ = Lerp(camera_.translation_, targetDestination_, kInterpolationRate);

	if (target_) {
		// 追従対象が画面外に出ないように補正
		const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
		camera_.translation_.x = (std::max)(camera_.translation_.x, targetWorldTransform.translation_.x + kTargetMargin.left);
		camera_.translation_.x = (std::min)(camera_.translation_.x, targetWorldTransform.translation_.x + kTargetMargin.right);
	}

	// 移動範囲制限
	camera_.translation_.x = std::clamp(camera_.translation_.x, movableArea_.left, movableArea_.right);
	camera_.translation_.y = std::clamp(camera_.translation_.y, movableArea_.bottom, movableArea_.top);

	// 行列を更新する
	camera_.UpdateMatrix();
}

void CameraController::Reset() {

	assert(target_);

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットからカメラの座標を計算
	targetDestination_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	targetDestination_.y = kTargetCameraY;
	targetDestination_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	camera_.translation_ = targetDestination_;

	// 移動範囲制限
	camera_.translation_.x = std::clamp(camera_.translation_.x, movableArea_.left, movableArea_.right);
	camera_.translation_.y = std::clamp(camera_.translation_.y, movableArea_.bottom, movableArea_.top);
	targetDestination_ = camera_.translation_;

	// 行列を更新する
	camera_.UpdateMatrix();
}
