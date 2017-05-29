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
	//���_�𓮂����B
	CVector3 cameraPos;
	cameraPos.x = player->position.x;
	cameraPos.y = player->position.y + 1.5f;
	cameraPos.z = player->position.z - 4.0f;
	camera.SetPosition(cameraPos);
	//�����_�𓮂����B
	CVector3 cameraTarget;
	cameraTarget.x = player->position.x;
	cameraTarget.y = player->position.y + 1.0f;
	cameraTarget.z = player->position.z;
	camera.SetTarget(cameraTarget);
}