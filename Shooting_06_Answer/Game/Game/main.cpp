#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Ground.h"
#include "Sky.h"
#include "Enemy.h"
#include "tkEngine/sound/tkSoundSource.h"

CLight g_defaultLight;
/*!
 * @brief	tkEngineの初期化。
 */
void InitTkEngine( HINSTANCE hInst )
{
	SInitParam initParam;
	memset(&initParam, 0, sizeof(initParam));
	//コマンドバッファのサイズのテーブル。
	int commandBufferSizeTbl[] = {
		10 * 1024 * 1024,		//10MB
	};
	initParam.hInstance = hInst;
	initParam.gameObjectPrioMax = 255;
	initParam.numRenderContext = 1;	//レンダリングコンテキストは一本
	initParam.commandBufferSizeTbl = commandBufferSizeTbl;
	initParam.screenHeight = 720;
	initParam.screenWidth = 1280;
	initParam.frameBufferHeight = 720;
	initParam.frameBufferWidth = 1280;
	//Bloom
	initParam.graphicsConfig.bloomConfig.isEnable = true;
	initParam.graphicsConfig.edgeRenderConfig.isEnable = false;
	initParam.graphicsConfig.edgeRenderConfig.idMapWidth = initParam.frameBufferWidth;
	initParam.graphicsConfig.edgeRenderConfig.idMapHeight = initParam.frameBufferHeight;
	//Shadow
	initParam.graphicsConfig.shadowRenderConfig.Init();
	initParam.graphicsConfig.shadowRenderConfig.isEnable = true;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapWidth = 1024;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapHeight = 1024;
	initParam.graphicsConfig.shadowRenderConfig.numShadowMap = 3;
	
	//reflection
	initParam.graphicsConfig.reflectionMapConfig.isEnable = false;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapWidth = 512;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapHeight = 512;
	//DOF
	initParam.graphicsConfig.dofConfig.isEnable = false;
	//AA
	initParam.graphicsConfig.aaConfig.isEnable = true;
	//Tonemap
	initParam.graphicsConfig.tonemapConfig.isEnable = false;
	Engine().Init(initParam);	//初期化。
	
	ShadowMap().SetNear(2.0f);
	ShadowMap().SetFar(40.0f);

	g_defaultLight.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	g_defaultLight.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	g_defaultLight.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, 0.707f));
	g_defaultLight.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, 0.0f));


	g_defaultLight.SetDiffuseLightColor(0, CVector4(0.3f, 0.3f, 0.3f, 1.0f));
	g_defaultLight.SetDiffuseLightColor(1, CVector4(0.3f, 0.3f, 0.3f, 1.0f));
	g_defaultLight.SetDiffuseLightColor(2, CVector4(0.3f, 0.3f, 0.3f, 1.0f));
	g_defaultLight.SetDiffuseLightColor(3, CVector4(0.3f, 0.3f, 0.3f, 1.0f));

	g_defaultLight.SetAmbinetLight({ 0.2f, 0.2f, 0.2f });
	g_defaultLight.SetLimLightColor(CVector4(0.0f, 0.0f, 0.0f, 1.0f));
	g_defaultLight.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	
}


int WINAPI wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow
)
{
	//tkEngineの初期化。
	InitTkEngine(hInst);
	
	NewGO<::Sky>(0, "Sky");		//空を追加。
	NewGO<Player>(0, "Player");	//プレイヤーを追加。
	NewGO<Ground>(0);			//地面を追加。

	//敵１体目。
	Enemy* enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 0.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//敵２体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 80.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//敵３体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 160.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//敵４体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -80.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//敵５体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -160.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//敵６体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 0.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//敵７体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 80.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//敵８体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 160.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//敵９体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -80.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//敵１０体目。
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -160.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//ゲームカメラを追加。
	GameCamera* cam = NewGO<GameCamera>(0, "GameCamera");
	ShadowMap().SetCamera(cam->m_camera);
	//音源クラスを作成。
	CSoundSource* soundSource = NewGO<CSoundSource>(0);
	//BGMをロード。
	soundSource->Init("Assets/sound/bgm.wav");
	//ループフラグを立てて再生。
	soundSource->Play(true);

	Engine().RunGameLoop();		//ゲームループを実行。

	return 0;
}