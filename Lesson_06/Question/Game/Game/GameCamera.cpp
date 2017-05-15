/*!
 *@brief	ゲームカメラ。
 */
#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera* gameCamera = nullptr;


GameCamera::GameCamera()
{
}
GameCamera::~GameCamera()
{
}
bool GameCamera::Start()
{
	//カメラの初期化。
	camera.SetNear(0.01f);
	camera.SetFar(10000.0f);
	camera.SetPosition(CVector3(0.0f, 1.5f, -4.0f));
	camera.SetTarget(CVector3(0.0f, 1.0f, 0.0f));
	camera.SetUp(CVector3::Up);
	camera.SetViewAngle(CMath::DegToRad(60.0f));
	
	return true;
}
void GameCamera::Update()
{
	Move();
	camera.Update();
}

//////////////////////////////////////////////////////////////////////
// カメラを動かすプログラムが記述されてるよ。
//////////////////////////////////////////////////////////////////////
void GameCamera::Move()
{
	CVector3 playerPos = player->GetPosition();
	//Question 1 視点を動かす。
	CVector3 cameraPos;
	cameraPos.x = playerPos.x;
	cameraPos.y = playerPos.y + 1.5f;
	cameraPos.z = playerPos.z - 4.0f;
	camera.SetPosition(cameraPos);
	//Question 2 注視点を動かす。
	CVector3 cameraTarget;
	cameraTarget.x = playerPos.x;
	cameraTarget.y = playerPos.y + 1.0f;
	cameraTarget.z = playerPos.z;
	camera.SetTarget(cameraTarget);
}