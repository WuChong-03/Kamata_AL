#pragma once
#include "Collision.h"
#include "KamataEngine.h"
#include <array>

class Enemy;
class MapChipField;

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	KamataEngine::Vector3 GetWorldPosition() const;
	AABB GetAABB() const;
	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	// 衝突応答
	void OnCollision(const Enemy* enemy);

private:
	// 振るまい
	enum class Behavior {
		kUnknown,
		kRoot,
		kAttack,
	};

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
	// 接地チェック用の微小な下方向ずらし
	static inline const float kGroundSearchHeight = 0.06f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.05f;
	// 壁接触時の速度減衰率
	static inline const float kAttenuationWall = 0.05f;

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);
	std::array<KamataEngine::Vector3, kNumCorner> CornerPositions(const KamataEngine::Vector3& center);
	bool IsMapChipBlock(const KamataEngine::Vector3& position);
	// 通常行動更新
	void BehaviorRootUpdate();
	// 攻撃行動更新
	void BehaviorAttackUpdate();
	// 通常行動初期化
	void BehaviorRootInitialize();
	// 攻撃行動初期化
	void BehaviorAttackInitialize();
	void MoveInput();
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void MoveByCollisionResult(const CollisionMapInfo& info);
	void ProcessCeilingCollision(const CollisionMapInfo& info);
	void ProcessWallCollision(const CollisionMapInfo& info);
	void SwitchGroundState(const CollisionMapInfo& info);
	void UpdateTurn();
	void UpdateMatrix();

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;

	// 接地状態フラグ
	bool onGround_ = true;
	// デスフラグ
	bool isDead_ = false;

	// 現在の振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;
	// 攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;
	// 攻撃行動中の移動速度（ImGui調整用）
	float attackMoveSpeed_ = 0.40f;
	// 攻撃行動の継続時間（フレーム、ImGui調整用）
	int attackDuration_ = 20;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
};
