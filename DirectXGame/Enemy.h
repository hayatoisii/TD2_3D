#pragma once
#include "EnemyBullet.h"
#include "MyMath.h"
#include "Player.h"

#include <Model.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <cassert>
#include <DeathParticles.h>

class Player;

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};



class Enemy {
public:
	
	void Initialize(Model* model, ViewProjection* camera, const Vector3& pos);
	void Update();
	void DeathEnemyUpdate();
	void fallingUpdate();
	bool ShouldTransitionPhase() const;
	void Draw();
	~Enemy();
	void Fire();
	void preFire();

	// 攻撃パターン
	void AttackPattern();
	void NormalFire();
	void BombFire();
	void EnhancedFire();
	void FinalFire();

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 発射間隔
	static const int kFireInterval = 250;
	static const int kFireInterval2 = 200;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	bool IsClear() const { return isDead_; }

	bool EnemyDead = false;
	WorldTransform worldtransfrom_;
private:
	Model* model_ = nullptr;
	ViewProjection* camera_ = nullptr;
	Input* input_ = nullptr;
	Model* modelbullet_ = nullptr;
	Model* atkbullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t spawnTimer = 0;
	int32_t AtkBulletspawnTimer = 0;
	int32_t BomBulletspawnTimer = 0;

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

	// 安全エリアの範囲を定義
	const Vector3 safeAreaCenter = {0.0f, 0.0f, 0.0f}; // 安全エリアの中心
	const float safeAreaRadius = 1.0f;                 // 安全エリアの半径

	int FireTimer_ = 0;


	Sprite* HpSprite_ = nullptr;
	uint32_t HpHandle_ = 0;
	//敵HP
	int enemyhp = 2000;

	// デスパーティクル
	DeathParticles* deathParticles_ = nullptr;
	Model* deathParticlesModel_ = nullptr;
	
	static inline const float kWalkSpeed = 0.05f;
	static inline const float kWalkMotionAngleStart = 0.2f;
	static inline const float kWalkMontionAngleEnd = -0.6f;
	static inline const float kWalkMotionTime = 1.0f;
	Vector3 velocity_ = {};
	float walkTimer = 0.0f;
};