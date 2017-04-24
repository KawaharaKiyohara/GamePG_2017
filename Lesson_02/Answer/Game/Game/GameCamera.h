/*!
 *@brief	�Q�[���J�����B
 */

#pragma once

#include "tkEngine/camera/tkSpringCamera.h"
#include "tkEngine/camera/tkCameraCollisionSolver.h"

class GameCamera : public IGameObject{
public:
	/*!
	 *@brief	�R���X�g���N�^�B
	 */
	GameCamera();
	/*!
	 *@brief	�f�X�g���N�^�B
	 */
	~GameCamera();
	/*!
	 *@brief	Update���Ă΂�Ă��珉�߂Ă̍X�V�̒��O�Ɉ�x�����Ă΂�鏈���B
	 */
	bool Start() override;
	/*!
	 *@brief	�X�V�����B
	 */
	void Update() override;
	/*!
	*@brief	�J�������擾�B
	*/
	const CCamera& GetCamera() const
	{
		return camera;
	}
	
private:
	CCamera	camera;		//!<�o�l�J�����B
};

extern GameCamera* gameCamera;