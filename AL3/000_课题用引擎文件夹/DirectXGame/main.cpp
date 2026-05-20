#include <Windows.h>
#include "KamataEngine.h"

using namespace KamataEngine;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	//初期化区
	Initialize(L"GC2C_02_ゴ_チュウ");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 图片句柄
	uint32_t spriteTextureHandle = 0;
	spriteTextureHandle = TextureManager::Load("image.png");

	// 模型贴图句柄
	uint32_t modelTextureHandle = 0;
	modelTextureHandle = TextureManager::Load("uvChecker.png");

	//图片实例
	Sprite* sprite = nullptr;
	sprite = Sprite::Create(spriteTextureHandle, {100, 50});

	//模型实例
	Model* model = nullptr;
	model = Model::Create();

	// WorldTransform 与 Camera 的实例创建, 初始化
	WorldTransform worldTransform;
	worldTransform.Initialize();
	Camera camera;
	camera.Initialize();

	//主循环
	while (true) {
		if (Update()) {
			break;
		}

		//图片移动
		Vector2 position1 = sprite->GetPosition();
		position1.x += 2.0f;
		sprite->SetPosition(position1);

		dxCommon->PreDraw();

		//图片绘制
		Sprite::PreDraw();
		sprite->Draw();
		Sprite::PostDraw();

		// Model 绘制
		Model::PreDraw();
		model->Draw(worldTransform, camera, modelTextureHandle);
		Model::PostDraw();

		dxCommon->PostDraw();
	}

	//释放区
	delete sprite;
	sprite = nullptr;

	delete model;
	model = nullptr;

	Finalize();
	return 0;
}
