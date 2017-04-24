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
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
private:
	void Move();
	void Rotation();
private:
	CCharacterController	characterController;			//�L�����N�^�[�R���g���[���[�B
	CVector3				moveSpeed = CVector3::Zero;		//�ړ����x�B
};

extern Player* player;