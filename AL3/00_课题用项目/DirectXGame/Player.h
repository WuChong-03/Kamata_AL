#pragma once
#include "KamataEngine.h"

class MapChipField;

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

private:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 movement = {};
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
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// めり込み防止用の微小な余白
	static inline const float kBlank = 0.05f;
	// 仮の地面高さ
	static inline const float kGroundHeight = 1.0f;

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);
	void MoveInput();
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void MoveByCollisionResult(const CollisionMapInfo& info);
	void ProcessCeilingCollision(const CollisionMapInfo& info);
	void UpdateTurn();
	void UpdateGroundState();
	void UpdateMatrix();

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
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
};
