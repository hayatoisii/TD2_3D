#include "GameClear.h"
#include "TextureManager.h"
#include <cassert>

GameClearScene::GameClearScene() {}

GameClearScene::~GameClearScene() { 
	delete modelSkydome_;
	delete modelClear_;
	delete modelPlayer_;
	delete modelearth_;
	delete modelEnemy_;
	delete modelbatu_;
	delete modelfalling_;
	delete skydome_;
	delete player_;
	delete secondPlayer_;
	delete earth_;
	delete enemydeath_;
	delete enemybatu_;
	delete enemyfalling_;
}

void GameClearScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	secondPlayer_ = new Player();
	earth_ = new Player(); // 新しいプレイヤーを作成
	enemydeath_ = new Enemy();
	enemybatu_ = new Enemy();
	enemyfalling_  = new Enemy();

	// 天球の生成
	skydome_ = new Skydome();
	// 天球3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("space", true);
	modelClear_ = Model::CreateFromOBJ("GameClear");
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelearth_ = Model::CreateFromOBJ("earth");
	modelEnemy_ = Model::CreateFromOBJ("enemydeath");
	modelbatu_ = Model::CreateFromOBJ("enemybatu");
	modelfalling_ = Model::CreateFromOBJ("falling");

	viewProjection_.Initialize();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	playerPos.z = -35;
	player_->Initialize(modelPlayer_, &viewProjection_, playerPos);

	// 2体目のプレイヤーを少し上の位置に配置
	Vector3 secondPlayerPos = playerPos;
	secondPlayerPos.y += 20.0f; // 1.0fの高さで配置
	secondPlayer_->Initialize(modelClear_, &viewProjection_, secondPlayerPos);

	earthPos.y = -1;
	earthPos.z = -38;
	earth_->Initialize(modelearth_, &viewProjection_, earthPos);


	enemyPos.x = -11;
	enemyPos.y = 12;
	enemyPos.z = -26;
	
	enemydeath_->Initialize(modelEnemy_, &viewProjection_, enemyPos);
	enemybatu_->Initialize(modelbatu_, &viewProjection_, enemyPos);
	enemyfalling_->Initialize(modelfalling_, &viewProjection_, enemyPos);

}

void GameClearScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	player_->ClearUpdate();
	secondPlayer_->worldtransfrom_.translation_ = Vector3(0, 2, -40); // Slight offset
	secondPlayer_->Update3();
	earth_->Update3();
	enemydeath_->DeathEnemyUpdate();
	enemybatu_->DeathEnemyUpdate();
	enemyfalling_->worldtransfrom_.translation_ = enemydeath_->worldtransfrom_.translation_ + Vector3(0, 3, 0); // Slight offset
	enemyfalling_->fallingUpdate();
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
	enemydeath_->Draw();
	enemybatu_->Draw();
	enemyfalling_->Draw();

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
