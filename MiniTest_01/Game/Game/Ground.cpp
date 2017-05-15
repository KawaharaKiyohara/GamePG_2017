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
	Init("Assets/modelData/ground.x", gameCamera->GetCamera());
	//地面のコリジョンと剛体を初期化。
	meshCollider.CreateFromSkinModel(&skinModel, nullptr);
	RigidBodyInfo rbInfo;
	rbInfo.collider = &meshCollider;
	rbInfo.mass = 0.0f;
	rigidBody.Create(rbInfo);
	PhysicsWorld().AddRigidBody(&rigidBody);
	Sky().SetEnable(&gameCamera->GetCamera(), &defaultLight);
	Sky().SetSceneLight(defaultLight);
	Sky().SetLuminance({ 5.5f, 5.5f, 5.5f });
	Sky().SetNightAmbientLight({ 0.2f, 0.2f, 0.2f });
	Sky().SetDayAmbientLight({ 0.7f, 0.7f, 0.7f });
	Tonemap().Reset();
	return true;
}
