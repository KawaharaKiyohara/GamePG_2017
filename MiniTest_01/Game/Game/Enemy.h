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
	void AnimationControl();
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
	
	CCharacterController	characterController;
	CVector3				moveSpeed = CVector3::Zero;		//移動速度。
	bool					isAttackFlag = false;			//攻撃中フラグ。
};

