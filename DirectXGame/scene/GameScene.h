#pragma once

#include "AABB.h"
#include "Audio.h"

#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Ground.h"
#include "Ground2.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
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

	void Reset();
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

	// 衝突判定と応答
	void CheckAllCollisions();

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	void ChangePhease();

	bool IsFinished() const { return finished_; }

	bool IsClear() const { return Clear_; }


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// プレイヤーモデル
	Model* modelPlayer_ = nullptr;
	// 敵Model
	Model* modelEnemy_ = nullptr;

	Vector3 playerPos = {0, -0.5f, -45};
	Vector3 enemyPos = {0, 3, 10};


	// 天球
	Skydome* skydome_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// 爆弾3Dモデル
	Model* modelBom_ = nullptr;
	// 地面
	Ground* ground_ = nullptr;
	// 地面モデル
	Model* modelGround_ = nullptr;
	// 地面2
	Ground2* ground2_ = nullptr;
	// 地面2モデル
	Model* modelGround2_ = nullptr;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// カメラ
	CameraController* cameraController_ = nullptr;
	// サウンドデータ
	/*int BattleBGMHandle_ = 0;*/
	int BattleBGMHandle_ = 0;
	int BattleAudio_ = -1;
	

	// アタックテクスチャ
	uint32_t AttackTextureHandle_ = 0;
	Sprite* AttackSprite_ = nullptr;

	// アタックバーテクスチャ
	uint32_t AttackBarTextureHandle_ = 0;
	Sprite* AttackBarSprite_ = nullptr;

	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
		kClear, // クリア
	};

	// ゲームの現在フェーズ(変数)
	Phase phase_;

	// バー速度
	float Yspeed = 10.0f;

	// 終了フラグ
	bool finished_ = false;
	bool Clear_ = false;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// BGMが再生されているかを追跡
	bool isBGMPlaying_ = false;
	// アタックテクスチャ描画フラグ
	bool Drawflg = false;
	// ディレイが開始されたかどうかを管理
	bool delayStarted = false;
	// 時間計測用の変数を追加
	clock_t start_time = 0; // スペースキーが押された時の時間を記録


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
