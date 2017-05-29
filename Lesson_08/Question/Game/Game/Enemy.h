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
		State_Idle,		//�ҋ@���
		State_Chase,	//�v���C���[��ǂ�������B
		State_Attack,	//�U����ԁB
	};
	CCharacterController	characterController;
	CVector3				moveSpeed = CVector3::Zero;		//�ړ����x�B
	State					state = State_Idle;				//�X�e�[�g�B
};

