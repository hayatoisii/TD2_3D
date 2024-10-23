#include "GameClear.h"
#include "TextureManager.h"
#include <cassert>

GameClearScene::GameClearScene() {}

GameClearScene::~GameClearScene() { 
	delete modelSkydome_;
	delete skydome_;
}

void GameClearScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	secondPlayer_ = new Player();
	earth_ = new Player(); // 新しいプレイヤーを作成

	// 天球の生成
	skydome_ = new Skydome();
	// 天球3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("space", true);
	modelClear_ = Model::CreateFromOBJ("GameOver");
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelearth_ = Model::CreateFromOBJ("earth");

	viewProjection_.Initialize();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	playerPos.z = -35;
	player_->Initialize(modelPlayer_, &viewProjection_, playerPos);

	// 2体目のプレイヤーを少し上の位置に配置
	Vector3 secondPlayerPos = playerPos;
	secondPlayerPos.y += 10.0f; // 1.0fの高さで配置
	secondPlayer_->Initialize(modelClear_, &viewProjection_, secondPlayerPos);

	earthPos.y = -2;
	earthPos.z = -35;
	earth_->Initialize(modelearth_, &viewProjection_, earthPos);

}

void GameClearScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	player_->ClearUpdate();
	secondPlayer_->worldtransfrom_.translation_ = Vector3(0, 2, -40); // Slight offset
	secondPlayer_->Update3();
	earth_->Update3();
	skydome_->Update();
}

void GameClearScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	 
	player_->Draw();
	secondPlayer_->Draw(); // 2体目のプレイヤーを描画
	skydome_->Draw();
	earth_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
