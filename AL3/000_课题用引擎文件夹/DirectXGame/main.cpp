#include <Windows.h>
#include "KamataEngine.h"

using namespace KamataEngine;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初始化区
	Initialize(L"GC2C_02_ゴ_チュウ");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	#pragma region 01_图片初始化
		// 图片句柄
		uint32_t spriteTextureHandle = 0;
		spriteTextureHandle = TextureManager::Load("test/image.png");

		// 图片实例
		Sprite* sprite = nullptr;
		sprite = Sprite::Create(spriteTextureHandle, {100, 50});
	#pragma endregion

	#pragma region 02_模型初始化
		// 模型贴图句柄
		uint32_t modelTextureHandle = 0;
		modelTextureHandle = TextureManager::Load("test/image.png");

		// 模型实例
		Model* model = nullptr;
		model = Model::Create();

		// 操作模型用的 WorldTransform 与 Camera 的实例创建, 初始化
		WorldTransform worldTransform;
		worldTransform.Initialize();
		Camera camera;
		camera.Initialize();
	#pragma endregion

	#pragma region 03_音频初始化
		// 音频句柄
		uint32_t soundDataHandle = 0;
		soundDataHandle = Audio::GetInstance()->LoadWave("test/music.wav");

		// 播放音源(循环)并 保存播放中的音频句柄
		uint32_t voiceHandle = 0;
		voiceHandle = Audio::GetInstance()->PlayWave(soundDataHandle, true);
		#pragma endregion

	#pragma region 04_ImGui初始化
	#ifdef _DEBUG
		// 取得实例对象
		ImGuiManager* imguiManager = ImGuiManager::GetInstance();

		// 输入框与滑动条演示用数组
	    float GuiTestArray[3] = {0, 0, 0};
	#endif
	#pragma endregion

	#pragma region 05_3D基础图形绘制(线)初始化
		//取得参考摄象机
	    PrimitiveDrawer::GetInstance()->SetCamera(&camera);
	#pragma endregion
	// 主循环
	while (true) {	
		if (Update()) break;

	#pragma region 04_ImGui更新处理
	#ifdef _DEBUG
		imguiManager->Begin();

		ImGui::ShowDemoWindow();

		// Debug 文本
		ImGui::Text("TestText %d", 2050);

		#pragma region 输入框与滑动条

		// 输入框初始化
		ImGui::InputFloat(
		    "array[0][0]",		// 输入框名字
		    &GuiTestArray[0]);		// 控制的变量

		ImGui::SliderFloat(
		    "array[0][0]", // 滑动条名字
		    &GuiTestArray[0],    // 控制的变量
		    0.0f,           // 可滑动的最小值
		    1.0f);          // 可滑动的最大值

		ImGui::InputFloat3(
		    "array", // 输入框名字
		    GuiTestArray);  // 控制的变量

		// 滑动条初始化
		ImGui::SliderFloat3(
		    "array", // 滑动条名字
		    GuiTestArray,    // 控制的变量
		    0.0f,           // 可滑动的最小值
		    1.0f);          // 可滑动的最大值
	#pragma endregion

		imguiManager->End();
	#endif
	#pragma endregion

	#pragma region 03_音频控制与键盘输入
		// 按下空格键瞬间停止音频
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			Audio::GetInstance()->StopWave(voiceHandle);
		}
	#pragma endregion

	#pragma region 01_图片更新处理
			//图片移动
			Vector2 position1 = sprite->GetPosition();
			position1.x += 2.0f;
			sprite->SetPosition(position1);
	#pragma endregion

	#pragma region 绘制
	dxCommon->PreDraw();

	// 01_Sprite
	//Sprite::PreDraw();
	//sprite->Draw();
	//Sprite::PostDraw();

	// 02_Model
	Model::PreDraw();
	model->Draw(worldTransform, camera, modelTextureHandle);
	Model::PostDraw();

	// 04_Imgui
	#ifdef _DEBUG
	imguiManager->Draw();
	#endif

	// 05_3D基础图形绘制(线)
	// 绘制一条红色线
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// 绘制一条绿色线
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 0}, {0.0f, 1.0f, 0.0f, 1.0f});

	dxCommon->PostDraw();
	#pragma endregion

	}

	#pragma region 释放区
		delete sprite;
		sprite = nullptr;

		delete model;
		model = nullptr;
	#pragma endregion

	Finalize();
	return 0;
}
