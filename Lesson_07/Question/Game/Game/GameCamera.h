/*!
 *@brief	ゲームカメラ。
 */

#pragma once

#include "tkEngine/camera/tkSpringCamera.h"
#include "tkEngine/camera/tkCameraCollisionSolver.h"

class GameCamera : public IGameObject{
public:
	/*!
	 *@brief	コンストラクタ。
	 */
	GameCamera();
	/*!
	 *@brief	デストラクタ。
	 */
	~GameCamera();
	/*!
	 *@brief	Updateが呼ばれてから初めての更新の直前に一度だけ呼ばれる処理。
	 */
	bool Start() override;
	/*!
	 *@brief	更新処理。
	 */
	void Update() override;
	/*!
	*@brief	カメラを取得。
	*/
	const CCamera& GetCamera() const
	{
		return camera;
	}
private:
	/*!
	*@brief	カメラの移動。
	*/
	void Move();
private:
	CCamera	camera;		//!<バネカメラ。
};

extern GameCamera* gameCamera;