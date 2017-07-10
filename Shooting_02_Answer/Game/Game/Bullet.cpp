#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
///////////////////////////////////////////////
//インスタンスを生成して一度だけ呼ばれる処理。
///////////////////////////////////////////////
bool Bullet::Start()
{
	//3Dモデルデータをロードして初期化。
	m_skinModelData.LoadModelData("Assets/modelData/bullet.x", NULL);
	m_skinModel.Init(m_skinModelData.GetBody());
	//ライトを設定。
	m_skinModel.SetLight(g_defaultLight);
	//毎フレームFindGOするのは重いので、初期化の時にゲームカメラを検索しておく。
	m_gameCamera = FindGO<GameCamera>("GameCamera");
	return true;
}
///////////////////////////////////////////////
//毎フレーム呼ばれる更新処理。
//座標の移動とかはここに書くといいよ。
///////////////////////////////////////////////
void Bullet::Update()
{
	m_position += m_moveSpeed;
	//弾のモデルが小さすぎるので拡大率に30倍を設定する。
	CVector3 scale;
	scale.x = 30.0f;
	scale.y = 30.0f;
	scale.z = 30.0f;
	m_skinModel.Update(m_position, CQuaternion::Identity, scale);
}
///////////////////////////////////////////////
//毎フレーム呼ばれる描画処理。
//モデルの描画処理。
///////////////////////////////////////////////
void Bullet::Render(CRenderContext& rc)
{
	m_skinModel.Draw(
		rc, 
		m_gameCamera->m_camera.GetViewMatrix(), 
		m_gameCamera->m_camera.GetProjectionMatrix()
	);
}
