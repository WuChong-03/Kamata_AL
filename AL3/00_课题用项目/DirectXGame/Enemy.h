#pragma once
#include "Collision.h"
#include "KamataEngine.h"

class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	KamataEngine::Vector3 GetWorldPosition() const;
	AABB GetAABB() const;

	// 衝突応答
	void OnCollision(const Player* player);

private:
	// 敵の当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// 歩行の速さ
	static inline const float kWalkSpeed = 0.03f;
	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = -10.0f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 35.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	// 速度
	KamataEngine::Vector3 velocity_ = {};
	// 経過時間
	float walkTimer_ = 0.0f;
};
