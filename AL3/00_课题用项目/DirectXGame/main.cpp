#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

// シーン
enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
};

// 現在シーン
Scene scene = Scene::kUnknown;
// 各シーン
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

void ChangeScene();
void UpdateScene();
void DrawScene();

void ChangeScene() {

	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	default:
		break;
	}
}

void UpdateScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	default:
		break;
	}
}

void DrawScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	default:
		break;
	}
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	Initialize(L"GC2C_02_ゴ_チュウ");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

	while (true) {

		if (Update()) {
			break;
		}

		// 現在シーンの更新
		UpdateScene();
		// シーン切り替え
		ChangeScene();

		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		dxCommon->PostDraw();
	}

	// シーン解放
	delete titleScene;
	titleScene = nullptr;
	delete gameScene;
	gameScene = nullptr;

	Finalize();
	return 0;
}
