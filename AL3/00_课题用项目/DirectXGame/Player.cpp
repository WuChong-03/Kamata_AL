#include "Player.h"
#include "WorldTransformUpdate.h"
#include <cassert>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Player::Update() {

	// 行列を更新して定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }
