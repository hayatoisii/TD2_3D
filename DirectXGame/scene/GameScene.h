#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Bom.h"
#include "Ground.h"
#include "Ground2.h"
#include"CameraController.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 天球
	Skydome* skydome_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;
	//爆弾
	Bom* bom_ = nullptr;
	//爆弾3Dモデル
	Model* modelBom_ = nullptr;
	//地面
	Ground* ground_ = nullptr;
	//地面モデル
	Model* modelGround_ = nullptr;
	// 地面2
	Ground2* ground2_ = nullptr;
	// 地面2モデル
	Model* modelGround2_ = nullptr;
	

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//カメラ
	CameraController* cameraController_ = nullptr;



	//終了フラグ
	bool finished_ = false;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
