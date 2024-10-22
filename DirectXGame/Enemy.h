#pragma once
#include "EnemyBullet.h"
#include "MyMath.h"
#include "Player.h"


#include <Model.h>
#include <TextureManager.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <cassert>
#include <Sprite.h>

class Player;

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
public:
	void Initialize(Model* model, ViewProjection* camera, const Vector3& pos);
	void Update();
	bool ShouldTransitionPhase() const;
	void Draw();
	~Enemy();
	void Fire();
	void preFire();

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 発射間隔
	static const int kFireInterval = 200;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	bool IsClear() const { return isDead_; }

private:
	
	WorldTransform worldtransfrom_;
	Model* model_ = nullptr;
	ViewProjection* camera_ = nullptr;
	Input* input_ = nullptr;
	Model* modelbullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t spawnTimer = 0;
	int hp = 2000;

	bool isDamage_ = false;
	bool isDead_ = false;

	Player* player_ = nullptr;

	Phase phase_ = Phase::Approach;

	Phase Bulletphase_ = Phase::Approach;

	// 点滅関連の変数
	static const int kBlinkDuration = 60; // 点滅の継続時間（フレーム数）
	bool isBlinking_ = false;             // 点滅中かどうかのフラグ
	int blinkTimer_ = 0;                  // 点滅のためのタイマー
	bool isClear_ = false;
	clock_t clearStartTime_ = 0;           // 敵が倒された瞬間の時間を記録する変数
	Vector3 initialPosition_ = {0, 5, 10}; // 初期位置を保存するための変数

	int FireTimer_ = 0;
};