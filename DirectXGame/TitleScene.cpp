#include "TitleScene.h"
#include "TextureManager.h"


#include <Input.h>
#include <cmath> // sin関数を使用するために必要

TitleScene::TitleScene() { finished_ = false; }

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
	modelSkydome_ = Model::CreateFromOBJ("space", true);
	skydome_->Initialize(modelSkydome_, &viewProjection_);
	// タイトルを調整
	worldTransform_.translation_ = {4.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {3.5f, 3.5f, 3.5f};

	// 初期Y軸位置と回転初期化
	initialYPosition_ = worldTransform_.translation_.y;
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	// 行列計算
	worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void TitleScene::Update() {
	UpdateRotation();

	UpdateVerticalMovement();

	// 行列計算
	worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
		rotationFinished_ = false; // 回転flgをリセット
		worldTransform_.rotation_.y = 0.0f;
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

void TitleScene::UpdateRotation() {
	// モデルの回転を制御
	static float rotationTime = 0.0f;

	// 回転が完了していない場合
	if (rotationTime < 1.0f && !rotationFinished_) {
		// イージングアウトのcubicを使って緩急をつける
		rotationTime += 0.02f;                                                // 時間の進みを少し遅くする
		float easedRotationTime = 1.0f - std::pow(1.0f - rotationTime, 3.0f); // イージングアウト (cubic)
		worldTransform_.rotation_.y = 4.0f * 3.14f * easedRotationTime;       // 2回転 (2π * 2)
	} else {
		// 回転が完了したのでフラグを設定
		rotationFinished_ = true;
		worldTransform_.rotation_.y = 0.0f;
	}
}

void TitleScene::UpdateVerticalMovement() {
	// モデルが回転し終わった後に上下に揺れる動きをつける
	static float time = 0.0f; // 時間カウンター

	time += 0.02f; // 少しずつ時間を進める

	// sin波に基づくY軸の変化量を計算
	float yOffset = std::sin(time) * 0.5f; // 振幅0.5で上下に動かす

	// タイトルのY軸位置を更新
	worldTransform_.translation_.y = initialYPosition_ + yOffset;
}