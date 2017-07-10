#include "stdafx.h"
#include "Ground.h"
#include "GameCamera.h"

Ground::Ground()
{
}
Ground::~Ground()
{
}
bool Ground::Start()
{
	m_skinModelData.LoadModelData("Assets/modelData/ground.X", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_skinModel.SetLight(g_defaultLight);
	return true;
}
void Ground::Update()
{

	m_skinModel.Update({0.0f, -40.0f, 0.0f}, CQuaternion::Identity, CVector3::One);
}
void Ground::Render(CRenderContext& rc)
{
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}