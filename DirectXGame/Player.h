#pragma once
#include "AABB.h"
#include "Audio.h"
#include "Mymath.h"
#include "PlayerBullet.h"
#include <3d/Model.h>
#include <Input.h>
#include <ViewProjection.h>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Sprite.h"

class Enemy;

class Player {
public:
	void Initialize(Model* model, ViewProjection* camera, const Vector3& pos);
	void Update();
	void Draw();
	void HpDraw();
	~Player();
	void Attack();
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// パリィを検出したら呼び出されるコールバック関数
	void Parry();

	void Dead();

	Vector3 GetWorldPosition();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// AABBを取得
	AABB GetAABB();

	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	bool IsDead() const { return isDead_; }

	Vector3 velocity_ = {};
	const WorldTransform& GetWorldTransform() const { return worldtransfrom_; }
	const Vector3& GetVelocity() const { return velocity_; }

	void Update2();

	void Update3();

	WorldTransform worldtransfrom_;

private:
	Audio* PryAudio_ = nullptr;
	Audio* DmgAudio_ = nullptr;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	ViewProjection* camera_ = nullptr;
	Model* modelbullet_ = nullptr;

	// 跳ね返す音
	int ParryAudioHandle_ = 0;
	int ParryAudio_ = -1;
	// ダメージ音
	int DamageAudioHandle_ = 0;
	int DamageAudio_ = -1;

	// プレイヤーHP
	int hp = 500;

	// 弾
	std::list<PlayerBullet*> bullets_;

	bool isDead_ = false;
	bool isParry_ = false;
	bool isDamage_ = false;

	int Damage = 100;

	Enemy* enemy_ = nullptr;

	// 発射タイマー
	int32_t spawnTimer = 0;
	int rotationTimer_ = 0;           // 回転用のタイマー
	const int kRotationDuration = 15; // 回転にかかるフレーム数

	// 点滅関連の変数
	static const int kBlinkDuration = 60; // 点滅の継続時間 (1秒間のフレーム数)
	bool isBlinking_ = false;             // 点滅中かどうかのフラグ
	int blinkTimer_ = 0;                  // 点滅のためのタイマー

	// プレイヤーHP
	Sprite* HpSprite1_ = nullptr;
	Sprite* HpSprite2_ = nullptr;
	Sprite* HpSprite3_ = nullptr;
	Sprite* HpSprite4_ = nullptr;
	Sprite* HpSprite5_ = nullptr;

	uint32_t HpHandle1_ = 0;
	uint32_t HpHandle2_ = 0;
	uint32_t HpHandle3_ = 0;
	uint32_t HpHandle4_ = 0;
	uint32_t HpHandle5_ = 0;

};