#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
using namespace KamataEngine;

//branchTest
//branchTest
//branchTest
//branchTest
//branchTest
//branchTest
//branchTest
//branchTest
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初期化
	KamataEngine::Initialize(L"GC2C_02_ゴ_チュウ");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();// 🟡取得DXCommon实例

	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	//メインループ
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}
		// 更新
		gameScene->Update();



		// 描画
		dxCommon->PreDraw();	// 🟡准备画

		gameScene->Draw();

		dxCommon->PostDraw();	// 🟡显示到屏幕
	}

	// 解放
	delete gameScene;
	gameScene = nullptr;
	KamataEngine::Finalize();
	return 0;
}
