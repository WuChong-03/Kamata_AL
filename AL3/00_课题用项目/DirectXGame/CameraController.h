#pragma once
#include "KamataEngine.h"

class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {
public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 瞬間合わせ
	/// </summary>
	void Reset();

	void SetTarget(Player* target) { target_ = target; }
	void SetMovableArea(Rect area) { movableArea_ = area; }

	KamataEngine::Camera& GetCamera() { return camera_; }
	const KamataEngine::Camera& GetCamera() const { return camera_; }

private:
	// カメラの目標Y座標
	static inline const float kTargetCameraY = 6.0f;
	// 座標補間割合
	static inline const float kInterpolationRate = 0.08f;
	// 速度掛け率
	static inline const float kVelocityBias = 30.0f;
	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect kTargetMargin = {-8.0f, 8.0f, -100.0f, 100.0f};

	KamataEngine::Camera camera_;
	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差（オフセット）
	KamataEngine::Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f};
	// カメラの目標座標
	KamataEngine::Vector3 targetDestination_ = {};
	// カメラ移動範囲
	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f};
};
