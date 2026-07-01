#include "Skydome.h"
#include "WorldTransformUpdate.h"
#include <cassert>

using namespace KamataEngine;

void Skydome::Initialize(Model* model) {

	assert(model);

	model_ = model;
	worldTransform_.Initialize();
}

void Skydome::Update() { WorldTransformUpdate(worldTransform_); }

void Skydome::Draw(Camera& camera) {

	// 3Dモデル描画
	model_->Draw(worldTransform_, camera);
}
