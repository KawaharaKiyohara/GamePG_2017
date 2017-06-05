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
	m_skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void Ground::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, g_gameCamera->m_camera.GetViewMatrix(), g_gameCamera->m_camera.GetProjectionMatrix());
}