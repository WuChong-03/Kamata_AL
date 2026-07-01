#include "WorldTransformUpdate.h"

using namespace KamataEngine;
using namespace KamataEngine::MathUtility;

void WorldTransformUpdate(WorldTransform& worldTransform) {

	// スケール、回転、平行移動を合成して行列を計算する
	worldTransform.matWorld_ = MakeScaleMatrix(worldTransform.scale_) * MakeRotateXMatrix(worldTransform.rotation_.x) * MakeRotateYMatrix(worldTransform.rotation_.y) *
	                           MakeRotateZMatrix(worldTransform.rotation_.z) * MakeTranslateMatrix(worldTransform.translation_);

	// 定数バッファへの書き込み
	worldTransform.TransferMatrix();
}
