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
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
private:
	void Move();
	void Rotation();
	void AnimationControl();
	void PlayAnimation(AnimationNo animNo);
private:
	CCharacterController	characterController;			//�L�����N�^�[�R���g���[���[�B
	CVector3				moveSpeed = CVector3::Zero;		//�ړ����x�B
	AnimationNo				playAnimNo = AnimationInvalid;	//�A�j���[�V�����ԍ��B
};

extern Player* player;