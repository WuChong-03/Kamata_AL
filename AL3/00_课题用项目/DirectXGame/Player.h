#pragma once
#include "KamataEngine.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

private:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.2f;
	static inline const float kTimeTurn = 0.3f;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.015f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.8f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 0.30f;
	// 仮の地面高さ
	static inline const float kGroundHeight = 1.0f;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;

	// 接地状態フラグ
	bool onGround_ = true;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
};
