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
		State_Idle,		//�ҋ@���
		State_Chase,	//�v���C���[��ǂ�������B
		State_Attack,	//�U����ԁB
		State_Damage,	//�_���[�W��ԁB
		State_Dead,		//���S��ԁB
	};
	CVector3				direction = CVector3::Zero;		//�G�l�~�[�̌����B
	CCharacterController	characterController;
	CVector3				moveSpeed = CVector3::Zero;		//�ړ����x�B
	State					state = State_Idle;				//�X�e�[�g�B
};

