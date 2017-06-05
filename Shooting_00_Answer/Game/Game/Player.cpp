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
//インスタンスを生成して一度だけ呼ばれる処理。
///////////////////////////////////////////////
bool Player::Start()
{
	m_skinModelData.LoadModelData("Assets/modelData/plane.X", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_skinModel.SetLight(g_defaultLight);
	return true;
}
///////////////////////////////////////////////
//毎フレーム呼ばれる更新処理。
//座標の移動とかはここに書くといいよ。
///////////////////////////////////////////////
void Player::Update()
{
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
}
///////////////////////////////////////////////
//毎フレーム呼ばれる描画処理。
//モデルの描画処理。
///////////////////////////////////////////////
void Player::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, g_gameCamera->m_camera.GetViewMatrix(), g_gameCamera->m_camera.GetProjectionMatrix());
}
