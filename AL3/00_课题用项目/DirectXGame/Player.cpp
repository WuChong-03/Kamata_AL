#include "Player.h"
#include <cassert> 

using namespace KamataEngine;

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {

	// 检查空指针
	assert(model);
	assert(camera);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	// 初始化世界变换
	worldTransform_.Initialize();
}

void Player::Update() {
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}
