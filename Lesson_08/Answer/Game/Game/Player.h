/*!
 *@brief	�v���C���[�B
 */

#pragma once

#include "Lib/Object3D.h"
#include "tkEngine/character/tkCharacterController.h"

/*!
 *@brief	�v���C���[�B
 */
class Player : public Object3D{
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//�����B
		AnimationWalk,		//�����B
		AnimationRun,		//����B
		AnimationJump,		//�W�����v�B
		AnimationAttack_Start,
		AnimationAttack_00 = AnimationAttack_Start,	//�U��00�B
		AnimationAttack_01,	//�U��01�B
		AnimationAttack_02,	//�U��02�B
		AnimationAttack_End = AnimationAttack_02,
		AnimationDamage,	//�_���[�W�B
		AnimationDeath,		//���S�B
		NumAnimation,		//�A�j���[�V�����̐��B
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
	CCharacterController	characterController;				//�L�����N�^�[�R���g���[���[�B
	CVector3				moveSpeed = CVector3::Zero;			//�ړ����x�B
	AnimationNo				playAnimNo = AnimationInvalid;		//�A�j���[�V�����ԍ��B
	State					state = State_Idle;					//��ԁB
	
	float					timer = 0.0f;						//�^�C�}�[�B
};

extern Player* player;