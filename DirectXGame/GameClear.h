#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "SkyDome.h"
#include "Player.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameClearScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameClearScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameClearScene();

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
	Player* earth_ = nullptr;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Skydome* skydome_ = nullptr;

	Vector3 playerPos = {};
	Vector3 earthPos = {};
	Model* modelPlayer_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelClear_ = nullptr;
	Model* modelearth_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 終了フラグ
	bool finished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
