#pragma once
#include "KamataEngine.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	~TitleScene();

	// 終了フラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// タイトル文字のモデル
	KamataEngine::Model* modelTitleFont_ = nullptr;
	// 自キャラのモデル
	KamataEngine::Model* modelPlayer_ = nullptr;
	// タイトル文字のワールド変換
	KamataEngine::WorldTransform worldTransformTitleFont_;
	// 自キャラのワールド変換
	KamataEngine::WorldTransform worldTransformPlayer_;
	// カメラ
	KamataEngine::Camera camera_;
	// アニメーション用タイマー
	float animationTimer_ = 0.0f;
	// 終了フラグ
	bool finished_ = false;
};
