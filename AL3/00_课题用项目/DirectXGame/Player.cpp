#include "Player.h"
#include "MapChipField.h"
#include "WorldTransformUpdate.h"
#include <algorithm>
#include <array>
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
	isDead_ = false;

	// 行列を更新して定数バッファに転送
	UpdateMatrix();
}

void Player::Update() {

	// 移動入力
	MoveInput();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.movement = velocity_;

	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	// 判定結果を反映して移動させる
	MoveByCollisionResult(collisionMapInfo);

	// 天井に接触している場合の処理
	ProcessCeilingCollision(collisionMapInfo);

	// 壁に接触している場合の処理
	ProcessWallCollision(collisionMapInfo);

	// 接地状態の切り替え
	SwitchGroundState(collisionMapInfo);

	// 旋回制御
	UpdateTurn();

	// 行列を更新して定数バッファに転送
	UpdateMatrix();
}

void Player::MoveInput() {

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
}

void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	if (!mapChipField_) {
		return;
	}

	if (info.movement.y <= 0.0f) {
		return;
	}

	Vector3 movedPosition = {
	    worldTransform_.translation_.x + info.movement.x,
	    worldTransform_.translation_.y + info.movement.y,
	    worldTransform_.translation_.z + info.movement.z,
	};

	// 移動前と移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsOld = CornerPositions(worldTransform_.translation_);
	std::array<Vector3, kNumCorner> positionsNew = CornerPositions(movedPosition);

	// 真上の当たり判定を行う
	bool hit = false;
	float limitedMovementY = info.movement.y;
	const Corner checkCorners[] = {kLeftTop, kRightTop};

	for (Corner corner : checkCorners) {
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[corner]);
		MapChipField::IndexSet indexSetOld = mapChipField_->GetMapChipIndexSetByPosition(positionsOld[corner]);
		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		MapChipType mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);

		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock && indexSetOld.yIndex != indexSet.yIndex) {
			hit = true;

			// めり込みを排除する方向に移動量を設定する
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float possibleMovementY = rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f) - kBlank;
			limitedMovementY = (std::min)(limitedMovementY, possibleMovementY);
		}
	}

	// ブロックにヒット？
	if (hit) {
		info.movement.y = (std::max)(0.0f, limitedMovementY);
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

	if (!mapChipField_) {
		return;
	}

	// 下降あり？
	if (info.movement.y >= 0.0f) {
		return;
	}

	Vector3 movedPosition = {
	    worldTransform_.translation_.x + info.movement.x,
	    worldTransform_.translation_.y + info.movement.y,
	    worldTransform_.translation_.z + info.movement.z,
	};

	// 移動前と移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsOld = CornerPositions(worldTransform_.translation_);
	std::array<Vector3, kNumCorner> positionsNew = CornerPositions(movedPosition);

	// 真下の当たり判定を行う
	bool hit = false;
	float limitedMovementY = info.movement.y;
	const Corner checkCorners[] = {kLeftBottom, kRightBottom};

	for (Corner corner : checkCorners) {
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[corner]);
		MapChipField::IndexSet indexSetOld = mapChipField_->GetMapChipIndexSetByPosition(positionsOld[corner]);
		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		MapChipType mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock && indexSetOld.yIndex != indexSet.yIndex) {
			hit = true;

			// めり込みを排除する方向に移動量を設定する
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float possibleMovementY = rect.top - worldTransform_.translation_.y + (kHeight / 2.0f) + kBlank;
			limitedMovementY = (std::max)(limitedMovementY, possibleMovementY);
		}
	}

	// ブロックにヒット？
	if (hit) {
		info.movement.y = (std::min)(0.0f, limitedMovementY);
		// 地面に当たったことを記録する
		info.landing = true;
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

	if (!mapChipField_) {
		return;
	}

	// 右移動あり？
	if (info.movement.x <= 0.0f) {
		return;
	}

	Vector3 movedPosition = {
	    worldTransform_.translation_.x + info.movement.x,
	    worldTransform_.translation_.y + info.movement.y,
	    worldTransform_.translation_.z + info.movement.z,
	};

	// 移動前と移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsOld = CornerPositions(worldTransform_.translation_);
	std::array<Vector3, kNumCorner> positionsNew = CornerPositions(movedPosition);

	// 右側の当たり判定を行う
	bool hit = false;
	float limitedMovementX = info.movement.x;
	const Corner checkCorners[] = {kRightTop, kRightBottom};

	for (Corner corner : checkCorners) {
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[corner]);
		MapChipField::IndexSet indexSetOld = mapChipField_->GetMapChipIndexSetByPosition(positionsOld[corner]);
		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		MapChipType mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);

		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock && indexSetOld.xIndex != indexSet.xIndex) {
			hit = true;

			// めり込みを排除する方向に移動量を設定する
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float possibleMovementX = rect.left - worldTransform_.translation_.x - (kWidth / 2.0f) - kBlank;
			limitedMovementX = (std::min)(limitedMovementX, possibleMovementX);
		}
	}

	// ブロックにヒット？
	if (hit) {
		info.movement.x = (std::max)(0.0f, limitedMovementX);
		// 壁に当たったことを記録する
		info.hitWall = true;
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

	if (!mapChipField_) {
		return;
	}

	// 左移動あり？
	if (info.movement.x >= 0.0f) {
		return;
	}

	Vector3 movedPosition = {
	    worldTransform_.translation_.x + info.movement.x,
	    worldTransform_.translation_.y + info.movement.y,
	    worldTransform_.translation_.z + info.movement.z,
	};

	// 移動前と移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsOld = CornerPositions(worldTransform_.translation_);
	std::array<Vector3, kNumCorner> positionsNew = CornerPositions(movedPosition);

	// 左側の当たり判定を行う
	bool hit = false;
	float limitedMovementX = info.movement.x;
	const Corner checkCorners[] = {kLeftTop, kLeftBottom};

	for (Corner corner : checkCorners) {
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[corner]);
		MapChipField::IndexSet indexSetOld = mapChipField_->GetMapChipIndexSetByPosition(positionsOld[corner]);
		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		MapChipType mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);

		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock && indexSetOld.xIndex != indexSet.xIndex) {
			hit = true;

			// めり込みを排除する方向に移動量を設定する
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float possibleMovementX = rect.right - worldTransform_.translation_.x + (kWidth / 2.0f) + kBlank;
			limitedMovementX = (std::max)(limitedMovementX, possibleMovementX);
		}
	}

	// ブロックにヒット？
	if (hit) {
		info.movement.x = (std::min)(0.0f, limitedMovementX);
		// 壁に当たったことを記録する
		info.hitWall = true;
	}
}

void Player::MoveByCollisionResult(const CollisionMapInfo& info) {

	// 移動
	worldTransform_.translation_.x += info.movement.x;
	worldTransform_.translation_.y += info.movement.y;
	worldTransform_.translation_.z += info.movement.z;
}

void Player::ProcessCeilingCollision(const CollisionMapInfo& info) {

	// 天井に当たった？
	if (info.ceiling) {
		velocity_.y = 0.0f;
	}
}

void Player::ProcessWallCollision(const CollisionMapInfo& info) {

	// 壁に当たった？
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::SwitchGroundState(const CollisionMapInfo& info) {

	// 自キャラが接地状態？
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
			return;
		}

		if (!mapChipField_) {
			return;
		}

		// 地面がなくなった場合の落下判定
		bool hit = false;
		Vector3 leftBottom = CornerPosition(worldTransform_.translation_, kLeftBottom);
		Vector3 rightBottom = CornerPosition(worldTransform_.translation_, kRightBottom);
		leftBottom.y -= kGroundSearchHeight;
		rightBottom.y -= kGroundSearchHeight;

		const Vector3 checkPositions[] = {leftBottom, rightBottom};

		for (const Vector3& position : checkPositions) {
			if (IsMapChipBlock(position)) {
				hit = true;
				break;
			}
		}

		// 落下開始
		if (!hit) {
			// 空中状態に切り替える
			onGround_ = false;
		}
	} else {
		// 着地
		if (info.landing) {
			// 着地状態に切り替える
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

void Player::UpdateTurn() {

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

void Player::UpdateMatrix() { WorldTransformUpdate(worldTransform_); }

Vector3 Player::GetWorldPosition() const { return worldTransform_.translation_; }

AABB Player::GetAABB() const {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {

	(void)enemy;
	// デスフラグを立てる
	isDead_ = true;
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	static const Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // kLeftTop
	};

	const Vector3& offset = offsetTable[static_cast<uint32_t>(corner)];
	return {
	    center.x + offset.x,
	    center.y + offset.y,
	    center.z + offset.z,
	};
}

std::array<Vector3, Player::kNumCorner> Player::CornerPositions(const Vector3& center) {

	std::array<Vector3, kNumCorner> positions;

	for (uint32_t i = 0; i < positions.size(); ++i) {
		positions[i] = CornerPosition(center, static_cast<Corner>(i));
	}

	return positions;
}

bool Player::IsMapChipBlock(const Vector3& position) {

	if (!mapChipField_) {
		return false;
	}

	MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(position);
	MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	return mapChipType == MapChipType::kBlock;
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }
