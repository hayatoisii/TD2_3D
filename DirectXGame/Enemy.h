#pragma once
#include "EnemyBullet.h"
#include "MyMath.h"
#include "Player.h"
#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <cassert>

class Player;

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
public:
	void Initialize(Model* model, ViewProjection* camera, const Vector3& pos);
	void Update();
	void Draw();
	~Enemy();
	void Fire();

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 発射間隔
	static const int kFireInterval = 200;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:
	WorldTransform worldtransfrom_;
	Model* model_ = nullptr;
	ViewProjection* camera_ = nullptr;

	Model* modelbullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t spawnTimer = 0;
	int hp = 5000;
	bool isDamage_ = false;
	bool isDead_ = false;

	Player* player_ = nullptr;

	Phase phase_ = Phase::Approach;

	Phase Bulletphase_ = Phase::Approach;
};
