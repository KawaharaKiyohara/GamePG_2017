#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Ground.h"
#include "Sky.h"
#include "Enemy.h"
#include "tkEngine/sound/tkSoundSource.h"

CLight g_defaultLight;
/*!
 * @brief	tkEngine�̏������B
 */
void InitTkEngine( HINSTANCE hInst )
{
	SInitParam initParam;
	memset(&initParam, 0, sizeof(initParam));
	//�R�}���h�o�b�t�@�̃T�C�Y�̃e�[�u���B
	int commandBufferSizeTbl[] = {
		10 * 1024 * 1024,		//10MB
	};
	initParam.hInstance = hInst;
	initParam.gameObjectPrioMax = 255;
	initParam.numRenderContext = 1;	//�����_�����O�R���e�L�X�g�͈�{
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
	Engine().Init(initParam);	//�������B
	
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
	//tkEngine�̏������B
	InitTkEngine(hInst);
	
	NewGO<::Sky>(0, "Sky");		//���ǉ��B
	NewGO<Player>(0, "Player");	//�v���C���[��ǉ��B
	NewGO<Ground>(0);			//�n�ʂ�ǉ��B

	//�G�P�̖ځB
	Enemy* enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 0.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//�G�Q�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 80.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//�G�R�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 160.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//�G�S�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -80.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//�G�T�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -160.0f;
	enemy->m_position.y = 0.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//�G�U�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 0.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//�G�V�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 80.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	//�G�W�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = 160.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//�G�X�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -80.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//�G�P�O�̖ځB
	enemy = NewGO<Enemy>(0, "Enemy");
	enemy->m_position.x = -160.0f;
	enemy->m_position.y = 50.0f;
	enemy->m_position.z = -100.0f;
	enemy->m_rotation.SetRotationDeg(CVector3::AxisY, 180.0f);

	//�Q�[���J������ǉ��B
	GameCamera* cam = NewGO<GameCamera>(0, "GameCamera");
	ShadowMap().SetCamera(cam->m_camera);
	//�����N���X���쐬�B
	CSoundSource* soundSource = NewGO<CSoundSource>(0);
	//BGM�����[�h�B
	soundSource->Init("Assets/sound/bgm.wav");
	//���[�v�t���O�𗧂ĂčĐ��B
	soundSource->Play(true);

	Engine().RunGameLoop();		//�Q�[�����[�v�����s�B

	return 0;
}