#include "stdafx.h"
#include "GameCamera.h"


GameCamera* g_gameCamera = NULL;

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}
///////////////////////////////////////////////
//�C���X�^���X�𐶐����Ĉ�x�����Ă΂�鏈���B
///////////////////////////////////////////////
bool GameCamera::Start()
{
	m_camera.SetPosition({ 0.0f, 60.0f, 200.0f });
	m_camera.SetTarget({0.0f, 50.0f, 0.0f});
	return true;
}
///////////////////////////////////////////////
//���t���[���Ă΂��X�V�����B
//���W�̈ړ��Ƃ��͂����ɏ����Ƃ�����B
///////////////////////////////////////////////
void GameCamera::Update()
{
	m_camera.Update();
}
///////////////////////////////////////////////
//���t���[���Ă΂��`�揈���B
//���f���̕`�揈���B
///////////////////////////////////////////////
void GameCamera::Render(CRenderContext& rc)
{
}
