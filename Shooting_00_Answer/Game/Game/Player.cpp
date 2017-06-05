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
//インスタンスを生成して一度だけ呼ばれる処理。
///////////////////////////////////////////////
bool Player::Start()
{
	//3Dモデルデータをロードして初期化。
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
	//モデルのワールド行列を更新。
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
}
///////////////////////////////////////////////
//毎フレーム呼ばれる描画処理。
//モデルの描画処理。
///////////////////////////////////////////////
void Player::Render(CRenderContext& rc)
{
	//GameCameraを検索。
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}
