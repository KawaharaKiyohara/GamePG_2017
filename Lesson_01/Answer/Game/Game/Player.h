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
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
private:
	void Move();
private:
	CCharacterController	characterController;			//キャラクターコントローラー。
	CVector3				moveSpeed = CVector3::Zero;		//移動速度。
};

extern Player* player;