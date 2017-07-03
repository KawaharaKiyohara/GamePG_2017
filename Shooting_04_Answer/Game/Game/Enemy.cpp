#include "stdafx.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Bullet.h"

Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}
bool Enemy::Start() 
{
	//3Dモデルデータをロードして初期化。
	m_skinModelData.LoadModelData("Assets/modelData/enemy.X", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_skinModel.SetLight(g_defaultLight);
	return true;
}
void Enemy::Update()
{
	//モデルのワールド行列を更新。
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
	
	//指定した名前のゲームオブジェクトを検索して、見つかったらラムダ式の処理を実行する。
	FindGO<Bullet>(
		"Bullet",
		//見つかったときの処理。
		[&](Bullet* bullet) {
			CVector3 diff = m_position - bullet->m_position;
			if (diff.Length() < 20.0f) {
				//死亡。
				DeleteGO(this);
			}
		}
	);
}
void Enemy::Render(CRenderContext& rc)
{
	//GameCameraを検索。
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}
