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
//インスタンスを生成して一度だけ呼ばれる処理。
///////////////////////////////////////////////
bool GameCamera::Start()
{
	m_camera.SetPosition({ 0.0f, 60.0f, 200.0f });
	m_camera.SetTarget({0.0f, 50.0f, 0.0f});
	return true;
}
///////////////////////////////////////////////
//毎フレーム呼ばれる更新処理。
//座標の移動とかはここに書くといいよ。
///////////////////////////////////////////////
void GameCamera::Update()
{
	m_camera.Update();
}
///////////////////////////////////////////////
//毎フレーム呼ばれる描画処理。
//モデルの描画処理。
///////////////////////////////////////////////
void GameCamera::Render(CRenderContext& rc)
{
}
