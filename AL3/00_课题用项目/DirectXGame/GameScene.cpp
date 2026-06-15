#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// player贴图
	modelTextureHandle_ = TextureManager::Load("test/image.png");
	model_ = Model::Create();

	// 摄像机创建和初始化
	camera_ = new Camera();
	camera_->Initialize();

	// 创建实例和初始化
	player_ = new Player();
	player_->Initialize(model_, modelTextureHandle_, camera_);
}

void GameScene::Update() {

	player_->Update();
}

void GameScene::Draw() {

	Model::PreDraw();

	player_->Draw();

	Model::PostDraw();
}

GameScene::~GameScene() {

	delete player_;
	player_ = nullptr;

	delete model_;
	model_ = nullptr;

	delete camera_;
	camera_ = nullptr;
}