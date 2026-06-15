#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>
using namespace KamataEngine;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初始化区
	Initialize(L"GC2C_02_ゴ_チュウ");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	// 主循环
	while (true) {

	//更新处理
		if (Update())
			break;

		gameScene->Update();

	//绘制处理
		dxCommon->PreDraw();

		gameScene->Draw();

		dxCommon->PostDraw();
	}

	//释放区
	delete gameScene;
	gameScene = nullptr;

	Finalize();
	return 0;
}