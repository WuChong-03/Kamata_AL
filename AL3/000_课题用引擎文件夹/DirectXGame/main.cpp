#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
using namespace KamataEngine;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初期化区
	Initialize(L"GC2C_02_ゴ_チュウ");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	// 主循环
	while (true) {
		if (Update()) {
			break;
		}
		gameScene->Update();

		dxCommon->PreDraw();

		gameScene->Draw();

		dxCommon->PostDraw();
	}

	// 释放区
	delete gameScene;
	gameScene = nullptr;
	Finalize();
	return 0;
}
