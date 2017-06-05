#pragma once

#include "Lib/Object3D.h"
#include "tkEngine/character/tkCharacterController.h"

class Enemy : public Object3D
{
public:
	Enemy();
	~Enemy();
	bool Start() override;
	void Update() override;
	void UpdateFSM();
	void Turn();
	void CheckDamage();
	void EmitAttackCollision(CVector3 pos, float delayTime);
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,
		AnimationWalk,
		AnimationAttack,
		AnimationDamage,
		AnimationDeath,
		NumAnimation
	};
	enum State {
		State_Idle,		//待機状態
		State_Chase,	//プレイヤーを追いかける。
		State_Attack,	//攻撃状態。
		State_Damage,	//ダメージ状態。
		State_Dead,		//死亡状態。
	};
	CVector3				direction = CVector3::Zero;		//エネミーの向き。
	CCharacterController	characterController;
	CVector3				moveSpeed = CVector3::Zero;		//移動速度。
	State					state = State_Idle;				//ステート。
};

