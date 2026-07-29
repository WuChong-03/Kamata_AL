#pragma once
#include "KamataEngine.h"

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// 歩行の速さ
	static inline const float kWalkSpeed = 0.03f;
	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = -10.0f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 35.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;
	// 速度
	KamataEngine::Vector3 velocity_ = {};
	// 経過時間
	float walkTimer_ = 0.0f;
};
