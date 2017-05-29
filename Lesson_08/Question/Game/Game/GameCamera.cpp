/*!
 *@brief	�Q�[���J�����B
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
	//�J�����̏������B
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
// �J�����𓮂����v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void GameCamera::Move()
{
	CVector3 playerPos = player->GetPosition();
	//Question 1 ���_�𓮂����B
	CVector3 cameraPos;
	cameraPos.x = playerPos.x;
	cameraPos.y = playerPos.y + 1.5f;
	cameraPos.z = playerPos.z - 4.0f;
	camera.SetPosition(cameraPos);
	//Question 2 �����_�𓮂����B
	CVector3 cameraTarget;
	cameraTarget.x = playerPos.x;
	cameraTarget.y = playerPos.y + 1.0f;
	cameraTarget.z = playerPos.z;
	camera.SetTarget(cameraTarget);
}