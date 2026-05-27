// GameScene.h
#pragma once
#include "KamataEngine.h"

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	~GameScene();

private:
#pragma region 01_图片
	uint32_t spriteTextureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
#pragma endregion

#pragma region 02_模型
	uint32_t modelTextureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
#pragma endregion

#pragma region 03_音频
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
#pragma endregion

#pragma region 04_ImGui
#ifdef _DEBUG
	KamataEngine::ImGuiManager* imguiManager_ = nullptr;
	float GuiTestArray_[3] = {0, 0, 0};
#endif
#pragma endregion

#pragma region 06_Debug摄像机
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
#pragma endregion

};