#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 天球の生成
	skydome_ = new Skydome();
	// 天球3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("space", true);

	// Bom生成
	bom_ = new Bom();
	// Bom3Dモデルの生成
	modelBom_ = Model::CreateFromOBJ("bom", true);

	// 地面生成
	ground_ = new Ground();
	// 地面出る生成
	modelGround_ = Model::CreateFromOBJ("ground", true);
	// 地面2生成
	ground2_ = new Ground2();
	// 地面2出る生成
	modelGround2_ = Model::CreateFromOBJ("ground2", true);
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);
	// Bom初期化
	bom_->Initialize(modelBom_, &viewProjection_);
	// 地面初期化
	ground_->Initialize(modelGround_, &viewProjection_);
	// 地面2初期化
	ground2_->Initialize(modelGround_, &viewProjection_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// カメラコントロールの初期化
	cameraController_ = new CameraController(); // 生成
	cameraController_->Initialize();

	// サウンドデータの読み込み
	BattleBGMHandle_ = audio_->LoadWave("./sound/battle.wav");
}

void GameScene::Update() {
	// BGMが再生されていない場合のみ再生する
	if (!isBGMPlaying_) {
		audio_->PlayWave(BattleBGMHandle_, true, 0.1f);
		isBGMPlaying_ = true; // フラグを立てる
	}

	// 天球の更新
	skydome_->Update();
	// Bomの更新
	bom_->Update();
	// 地面更新
	ground_->Update();
	ground2_->Update();

	// カメラ更新
	cameraController_->Update();

	// カメラ処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	}
	if (Input::GetInstance()->PushKey(DIK_2)) {
		finished_ = true;
	}

	if (input_->TriggerKey(DIK_F1)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;
	}
	// imGui
	ImGui::Begin("debug");
	ImGui::Text("IsDebugCamera: %d", isDebugCameraActive_); // シーン名を表示
	// ImGui::Text("IsRotate: %f", skydomeRotate); // シーン名を表示
	ImGui::End();
}

void GameScene::Draw() {

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

	// Bomの描画
	// bom_->Draw();

	// 地面描画
	ground_->Draw();
	ground2_->Draw();
	// 天球の描画
	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	///

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
