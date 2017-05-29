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
private:
	void UpdateFSM();
private:
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
	};
	CCharacterController	characterController;
	CVector3				moveSpeed = CVector3::Zero;		//移動速度。
	State					state = State_Idle;				//ステート。
};

