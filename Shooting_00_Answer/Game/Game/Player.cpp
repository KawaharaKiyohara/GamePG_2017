#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

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
	//3D���f���f�[�^�����[�h���ď������B
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
	//���f���̃��[���h�s����X�V�B
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
}
///////////////////////////////////////////////
//���t���[���Ă΂��`�揈���B
//���f���̕`�揈���B
///////////////////////////////////////////////
void Player::Render(CRenderContext& rc)
{
	//GameCamera�������B
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}
