#include "TitleScene.h"
#include "TextureManager.h"
#include <Input.h>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete model_;
	delete skydome_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	model_ = Model::CreateFromOBJ("title", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// SkyDome
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydomeTitle", true);
	skydome_->Initialize(modelSkydome_, &viewProjection_);
	// タイトルを調整
	worldTransform_.translation_ = {-18.0f, 13.0f, 0.0f};
	// 行列計算
	worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_1)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// sprite_->Draw();
	//  スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	model_->Draw(worldTransform_, viewProjection_);

	skydome_->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
}