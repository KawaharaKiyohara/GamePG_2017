/*!
 *@brief	�v���C���[�B
 */
#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"


Player* player = nullptr;

Player::Player()
{
}
Player::~Player()
{
}
bool Player::Start()
{
	//�v���C���[���������B
	Init("Assets/modelData/Player.X", gameCamera->GetCamera());
	//�L�����N�^�[�R���g���[���[���������B
	characterController.Init(0.6f, 0.3f, position);

	//�}�e���A���������B
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Thethief_D.tga");
	if (mat != nullptr) {
		//���������B
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Thethief_N.tga"));
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SpecularMap, *TextureResources().Load("Assets/modelData/Thethief_S.tga"));
	}
	mat = skinModelData.GetBody()->FindMaterial("Thethief_wuqi_D.tga");
	if (mat != nullptr) {
		//���������B
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Thethief_wuqi_N.tga"));
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SpecularMap, *TextureResources().Load("Assets/modelData/Thethief_wuqi_S.tga"));
	}
	//�@���}�b�v�A�X�y�L�����}�b�v�A�t���l������ݒ肷��B
	skinModel.SetHasNormalMap(true);
	skinModel.SetHasSpeculerMap(true);
	skinModel.SetFresnelFlag(true);
	return true;
}
void Player::Update()
{
	//���N���X��Update���Ăяo���B
	Object3D::Update();
	//�v���C���[�𓮂����B
	Move();

	CVector3 lightPos = position;
	lightPos.y += 2.0f;
	lightPos.z += 1.0f;
	ShadowMap().SetLightPosition(lightPos);
	CVector3 lightTarget = position;
	ShadowMap().SetLightTarget(lightTarget);
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[�𓮂����v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::Move()
{
	//Question 1 �L���������E�ɓ������Ă݂悤�B
	moveSpeed.x = 0.2f;
	//Question 2 �L�������㉺�ɓ������Ă݂悤�B
	//Question 3 �L�������W�����v�����Ă݂悤�B
	//Question 4 �d�͂������Ă݂悤�B
	//�ړ��B
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
}