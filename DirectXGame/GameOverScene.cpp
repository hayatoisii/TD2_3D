#include "GameOverScene.h"
#include "TextureManager.h"
#include <cassert>

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {

	delete player_;
	delete modelPlayer_;
	delete modelGameover_;
	delete modelSkydome_;
	delete secondPlayer_;
	delete skydome_;
}

void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	secondPlayer_ = new Player(); // 新しいプレイヤーを作成

	// 3Dモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelGameover_ = Model::CreateFromOBJ("GameOver");
	// 天球の生成
	skydome_ = new Skydome();
	// 天球3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("space", true);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// プレイヤーと敵の初期化
	playerPos.z = 0;
	player_->Initialize(modelPlayer_, &viewProjection_, playerPos);
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// 2体目のプレイヤーを少し上の位置に配置
	Vector3 secondPlayerPos = playerPos;
	secondPlayerPos.y += 10.0f; // 1.0fの高さで配置
	secondPlayer_->Initialize(modelGameover_, &viewProjection_, secondPlayerPos);

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
	cameraController_->Reset();
}

void GameOverScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	player_->Update2();
	secondPlayer_->worldtransfrom_.translation_ = player_->worldtransfrom_.translation_ + Vector3(0, 2, 0); // Slight offset
	secondPlayer_->Update3();
	skydome_->Update();

	viewProjection_.matView = cameraController_->GetViewProjection().matView;
	viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
	cameraController_->Update();
}

void GameOverScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	player_->Draw();
	secondPlayer_->Draw(); // 2体目のプレイヤーを描画
	skydome_->Draw();
	/// </summary>

	Model::PostDraw();
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}