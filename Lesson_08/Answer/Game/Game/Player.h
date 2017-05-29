/*!
 *@brief	プレイヤー。
 */

#pragma once

#include "Lib/Object3D.h"
#include "tkEngine/character/tkCharacterController.h"

/*!
 *@brief	プレイヤー。
 */
class Player : public Object3D{
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//立ち。
		AnimationWalk,		//歩き。
		AnimationRun,		//走り。
		AnimationJump,		//ジャンプ。
		AnimationAttack_Start,
		AnimationAttack_00 = AnimationAttack_Start,	//攻撃00。
		AnimationAttack_01,	//攻撃01。
		AnimationAttack_02,	//攻撃02。
		AnimationAttack_End = AnimationAttack_02,
		AnimationDamage,	//ダメージ。
		AnimationDeath,		//死亡。
		NumAnimation,		//アニメーションの数。
	};
	enum State {
		State_Idle,
		State_Run,
		State_Jump,
		State_Attack,
		State_Damage,
	};
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void Move();
	void Rotation();
	void AnimationControl();	
	void PlayAnimation(AnimationNo animNo);
	void EmitAttackCollision(CVector3 pos, float delayTime);
	void CheckDamage();
	CCharacterController	characterController;				//キャラクターコントローラー。
	CVector3				moveSpeed = CVector3::Zero;			//移動速度。
	AnimationNo				playAnimNo = AnimationInvalid;		//アニメーション番号。
	State					state = State_Idle;					//状態。
	
	float					timer = 0.0f;						//タイマー。
};

extern Player* player;