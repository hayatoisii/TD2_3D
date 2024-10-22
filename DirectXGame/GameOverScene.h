#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SkyDome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameOverScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameOverScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOverScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() const { return finished_; }

	Player* player_ = nullptr;
	Player* secondPlayer_ = nullptr;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* GamOverAudio_ = nullptr;
	int gameoverAudioHandle = 0;
	int gameoverAudio = -1;
	bool isBGMPlaying_ = false;


	int32_t ClearTexHandle = 0;
	// スプライト
	Sprite* Clearsprite_ = nullptr;

	// 終了フラグ
	bool finished_ = false;

	CameraController* cameraController_;
	Vector3 playerPos = {};
	Model* modelPlayer_ = nullptr;
	Model* modelGameover_ = nullptr;
	Model* modelSkydome_ = nullptr;

	Skydome* skydome_ = nullptr;

	ViewProjection viewProjection_;
	WorldTransform worldTransform_;

	bool isDebugCameraActive_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};