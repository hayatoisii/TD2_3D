#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "MyMath.h"
#include "SkyDome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>

class TitleScene {

public:
	TitleScene();
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	void UpdateRotation();

	void UpdateVerticalMovement();

	bool IsFinished() const { return finished_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	uint32_t titleTextureHandle_ = 0;


	// モデル
	Model* model_ = nullptr;

	// 終了フラグ
	bool finished_ = false;
	bool rotationFinished_ = false;
	 // タイトルの初期Y位置
    float initialYPosition_ = 0.0f;  // 初期のY位置を保存する変数


	// SkyDome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;


};