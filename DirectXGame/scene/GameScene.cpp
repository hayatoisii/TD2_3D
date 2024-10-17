#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <cassert>

// 3秒wait

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelSkydome_;
	delete modelBom_;
	delete modelGround_;
	delete modelGround2_;
	delete AttackSprite_;
}

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

	// テクスチャデータの読み込み
	AttackTextureHandle_ = TextureManager::Load("attack/attack.png");
	AttackSprite_ = Sprite::Create(AttackTextureHandle_, {800, 50});

	AttackBarTextureHandle_ = TextureManager::Load("attack/attackbar.png");
	AttackBarSprite_ = Sprite::Create(AttackBarTextureHandle_, {800, 60}); // 上55,下490
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

	// スプライト
	Vector2 AttackBarPos = AttackBarSprite_->GetPosition();

	AttackBarPos.y += Yspeed;

	if (AttackBarPos.y <= 55 || AttackBarPos.y >= 490) {
		Yspeed = -Yspeed;
	}

	if (input_->TriggerKey(DIK_SPACE)) { // スペースキーが押されたら
		Yspeed = 0;
		start_time = clock(); // スペースキーを押した時の時間を記録
		delayStarted = true;  // ディレイが開始されたことをフラグで管理
	}
	// ディレイ中かどうかをチェック
	if (delayStarted) {
		// 3秒が経過したかをチェック
		if (clock() >= start_time + 3000) { // 3秒 (3000ミリ秒) 経過後
			Drawflg = false;                // 描画フラグをfalseにする
			delayStarted = false;           // ディレイ終了
		}
	}
	AttackBarSprite_->SetPosition(AttackBarPos);

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
	// if (input_->PushKey(DIK_C)) {

	if (Drawflg == true) {
		AttackSprite_->Draw();
		AttackBarSprite_->Draw();
	}
	//}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
