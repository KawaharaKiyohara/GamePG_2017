#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Bullet.h"

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
	//移動処理を呼び出す。
	Move();
	//モデルのワールド行列を更新。
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
	//弾丸の発射処理を記述。
	if (m_interval == 0) {
		//発射間隔が経過した。
		if (Pad(0).IsPress(enButtonA)) {
			Bullet* bullet = NewGO<Bullet>(0, "Bullet");
			bullet->m_position = m_position;
			bullet->m_moveSpeed.x = 0.0f;
			bullet->m_moveSpeed.y = 0.0f;
			bullet->m_moveSpeed.z = 10.0f;
			m_interval = 5;		//5フレーム間隔をあける。
		}
	}
	else {
		m_interval--;
	}

	// 指定した名前のゲームオブジェクトを検索して、見つかったらラムダ式の処理を実行する。
		FindGO<Bullet>(
			"EnemyBullet",
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
///////////////////////////////////////////////
//移動処理。
///////////////////////////////////////////////
void Player::Move()
{
	if (Pad(0).IsPress(enButtonLeft)) {
		m_position.x += 1.5f;
		//Z軸周りに-10.0°旋回する。
		m_rotation.SetRotationDeg(CVector3::AxisZ, -10.0f);
	}
	if (Pad(0).IsPress(enButtonRight)) {
		m_position.x -= 1.5f;
		//Z軸周りに10.0°旋回する。
		m_rotation.SetRotationDeg(CVector3::AxisZ, 10.0f);
	}

	if (Pad(0).IsPress(enButtonUp)) {
		m_position.y += 1.5f;
		//Z軸周りに-10.0°旋回する。
		m_rotation.SetRotationDeg(CVector3::AxisX, 40.0f);
	}
	if (Pad(0).IsPress(enButtonDown)) {
		m_position.y -= 1.5f;
		m_rotation.SetRotationDeg(CVector3::AxisX, -40.0f);
	}
	//もしもm_position.xが150よりも大きくなったら。
	if (m_position.x > 150.0f) {
		//150を代入する。
		m_position.x = 150.0f;
	}
	//もしもm_position.xが150よりも小さくなったら。
	if (m_position.x < -150.0f) {
		//-150を代入する。
		m_position.x = -150.0f;
	}
	//もしもm_position.yが120よりも大きくなったら。
	if (m_position.y > 120.0f) {
		//120を代入する。
		m_position.y = 120.0f;
	}
	//もしもm_position.yが0よりも小さくなったら。
	if (m_position.y < 0.0f) {
		//0を代入する。
		m_position.y = 0.0f;
	}
}
