// GameScene.cpp
#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

#pragma region 06_Debug摄像机初始化
	// 创建 DebugCamera
	debugCamera_ = new DebugCamera(1280, 720);
#pragma endregion

#pragma region 01_图片初始化
	// 图片句柄
	spriteTextureHandle_ = TextureManager::Load("test/image.png");

	// 图片实例
	sprite_ = Sprite::Create(spriteTextureHandle_, {100, 50});
#pragma endregion

#pragma region 03_音频初始化
	// 音频句柄
	soundDataHandle_ = Audio::GetInstance()->LoadWave("test/music.wav");

	// 播放音源(循环)并 保存播放中的音频句柄
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
#pragma endregion

#pragma region 04_ImGui初始化
#ifdef _DEBUG
	// 取得实例对象
	imguiManager_ = ImGuiManager::GetInstance();
#endif
#pragma endregion

#pragma region 02_模型初始化
	// 模型贴图句柄
	modelTextureHandle_ = TextureManager::Load("test/image.png");

	// 模型实例
	model_ = Model::Create();

	// 操作模型用的 WorldTransform 初始化
	worldTransform_.Initialize();
#pragma endregion

#pragma region 05_3D基础图形绘制(线)初始化
	// 取得参考摄象机
	PrimitiveDrawer::GetInstance()->SetCamera(&debugCamera_->GetCamera());
#pragma endregion

#pragma region 07_AxisIndicator坐标指示器
	AxisIndicator::GetInstance()->SetVisible(true);//开启坐标轴

	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());//指定摄像机
#pragma endregion
}

void GameScene::Update() {

#pragma region 01_图片更新处理
	// 图片移动
	Vector2 position1 = sprite_->GetPosition();
	position1.x += 2.0f;
	sprite_->SetPosition(position1);
#pragma endregion

#pragma region 03_音频控制与键盘输入
	// 按下空格键瞬间停止音频
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceHandle_);
	}
#pragma endregion

#pragma region 04_ImGui更新处理
#ifdef _DEBUG
	imguiManager_->Begin();

	ImGui::ShowDemoWindow();

	// Debug 文本
	ImGui::Text("TestText %d", 2050);

#pragma region 输入框与滑动条

	// 输入框初始化
	ImGui::InputFloat(
	    "array[0][0]",      // 输入框名字
	    &GuiTestArray_[0]); // 控制的变量

	ImGui::SliderFloat(
	    "array[0][0]",     // 滑动条名字
	    &GuiTestArray_[0], // 控制的变量
	    0.0f,              // 可滑动的最小值
	    1.0f);             // 可滑动的最大值

	ImGui::InputFloat3(
	    "array",        // 输入框名字
	    GuiTestArray_); // 控制的变量

	// 滑动条初始化
	ImGui::SliderFloat3(
	    "array",       // 滑动条名字
	    GuiTestArray_, // 控制的变量
	    0.0f,          // 可滑动的最小值
	    1.0f);         // 可滑动的最大值

#pragma endregion

	imguiManager_->End();
#endif
#pragma endregion

#pragma region 06_Debug摄像机更新处理
	// 更新 DebugCamera
	debugCamera_->Update();
#pragma endregion

}

void GameScene::Draw() {

#pragma region 绘制

	// 01_Sprite
	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();

	// 02_Model
	Model::PreDraw();
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), modelTextureHandle_);
	Model::PostDraw();

	// 05_3D基础图形绘制(线)
	// 绘制一条红色线
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// 绘制一条绿色线
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 0}, {0.0f, 1.0f, 0.0f, 1.0f});

	// 04_Imgui
#ifdef _DEBUG
	imguiManager_->Draw();
#endif

#pragma endregion
}

GameScene::~GameScene() {

#pragma region 释放区
	delete sprite_;
	sprite_ = nullptr;

	delete model_;
	model_ = nullptr;

	delete debugCamera_;
	debugCamera_ = nullptr;
#pragma endregion
}