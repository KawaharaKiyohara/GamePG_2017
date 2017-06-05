#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

Player* g_player = NULL;
Player::Player()
{
}


Player::~Player()
{
}
///////////////////////////////////////////////
//�C���X�^���X�𐶐����Ĉ�x�����Ă΂�鏈���B
///////////////////////////////////////////////
bool Player::Start()
{
	m_skinModelData.LoadModelData("Assets/modelData/plane.X", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_skinModel.SetLight(g_defaultLight);
	return true;
}
///////////////////////////////////////////////
//���t���[���Ă΂��X�V�����B
//���W�̈ړ��Ƃ��͂����ɏ����Ƃ�����B
///////////////////////////////////////////////
void Player::Update()
{
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
}
///////////////////////////////////////////////
//���t���[���Ă΂��`�揈���B
//���f���̕`�揈���B
///////////////////////////////////////////////
void Player::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, g_gameCamera->m_camera.GetViewMatrix(), g_gameCamera->m_camera.GetProjectionMatrix());
}
