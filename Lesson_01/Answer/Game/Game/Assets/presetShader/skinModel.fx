/*!
 * @brief	�X�L�����f���V�F�[�_�[�B(4�{�[���X�L�j���O)
 */
#include "Common.h" 
#include "LightingFunction.h"
#include "ShadowFunction.h"


//�X�L���s��B
#define MAX_MATRICES  26
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    g_mViewProj : VIEWPROJECTION;
float		g_numBone;			//���̐��B
float4x4	g_worldMatrix;			//!<���[���h�s��B
float4x4	g_mViewProjLastFrame;	//!<1�t���[���O�̃r���[�v���W�F�N�V�����s��B
float4		g_fogParam;				//�t�H�O�̃p�����[�^�Bx�Ƀt�H�O���|����n�߂�[�x�By�Ƀt�H�O�����S�ɂ�����[�x�Bz�̓t�H�O���v�Z���邩�ǂ����̃t���O�B


int4 g_flags;				//x�ɖ@���}�b�v�Ay�̓V���h�E���V�[�o�[�Az�̓������C�g�Aw�̓X�y�L�����}�b�v�B

texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

//�@���}�b�v
texture g_normalTexture;		//�@���}�b�v�B
sampler g_normalMapSampler = 
sampler_state
{
	Texture = <g_normalTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

//��p�̃L���[�u�}�b�v�B
texture g_skyCubeMap;
samplerCUBE g_skyCubeMapSampler = 
sampler_state
{
    Texture = <g_skyCubeMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

//�V�[���e�N�X�`��
texture g_sceneTexture;		//�V�[���e�N�X�`���B
sampler g_sceneTextureSampler = 
sampler_state
{
	Texture = <g_sceneTexture>;
	MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

/*!
 * @brief	���͒��_
 */
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3	Tangent			: TANGENT;		//�ڃx�N�g��
    float3  Tex0            : TEXCOORD0;
};

/*!
 * @brief	�C���X�^���V���O�`��p�̓��͒��_�B
 */
struct VS_INPUT_INSTANCING
{
	VS_INPUT	base;
	float4 mWorld1	: TEXCOORD1;		//���[���h�s���1�s��
	float4 mWorld2	: TEXCOORD2;		//���[���h�s���2�s��
	float4 mWorld3	: TEXCOORD3;		//���[���h�s���3�s��
	float4 mWorld4	: TEXCOORD4;		//���[���h�s���4�s��
};

/*!
 * @brief	�o�͒��_�B
 */
struct VS_OUTPUT
{
	float4  Pos     		: POSITION;
    float3  Normal			: NORMAL;
    float2  Tex0   			: TEXCOORD0;
    float3	Tangent			: TEXCOORD1;	//�ڃx�N�g��
    float4  worldPos_depth	: TEXCOORD2;	//xyz�Ƀ��[���h���W�Bw�ɂ͎ˉe��Ԃł�depth���i�[�����B
    float4  velocity		: TEXCOORD3;	//���x�B
    float4  screenPos		: TEXCOORD4;
    float4	mieColor		: TEXCOORD5;	//�~�[�����F�B
    float4	rayColor		: TEXCOORD6;	//���C���[�����F�B
    float3  posToCameraDir	: TEXCOORD7;
};

/*!
 * @brief	�V���h�E�}�b�v�`�����ݎ��̏o�͒��_�B
 */
struct VS_OUTPUT_RENDER_SHADOW_MAP
{
	float4  Pos     : POSITION;
	float4	depth	: TEXCOORD;
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[����̏o�́B
 */
struct PSOutput{
	float4	color		: COLOR0;		//�����_�����O�^�[�Q�b�g0�ɏ������݁B
	float4	depth		: COLOR1;		//�����_�����O�^�[�Q�b�g1�ɏ������݁B
	float4  velocity 	: COLOR2;		//�����_�����O�^�[�Q�b�g2�ɏ������݁B
};




// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}
// Returns the near intersection point of a line and a sphere
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
}
// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0;
	return 0.75 + 0.75*fCos2;
}

/*!
 * @brief	��C�����������������̃~�[�����ƃ��C���[�������v�Z�B
 */
void CalcMieAndRayleighColorsSkyFromAtomosphere( out float4 mieColor, out float4 rayColor, out float3 posToCameraDir, float3 worldPos )
{
//	worldPos.y += g_atmosParam.fInnerRadius;
	float3 cameraPos = g_cameraPos.xyz;
	cameraPos *= 0.001f;
	cameraPos.y += g_atmosParam.fInnerRadius;
	mieColor = 0.0f;
	rayColor = 0.0f;
	worldPos *= 0.001f;
	worldPos.y += g_atmosParam.fInnerRadius;
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3Ray = worldPos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
//	float fNear = getNearIntersection(g_cameraPos.xyz, v3Ray, g_atmosParam.fCameraHeight2, g_atmosParam.fOuterRadius2);
	
	float3 v3Start = cameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(g_atmosParam.fScaleOverScaleDepth * (g_atmosParam.fInnerRadius - g_atmosParam.fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);
	
	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * g_atmosParam.fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	
	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(g_atmosParam.fScaleOverScaleDepth * (g_atmosParam.fInnerRadius - fHeight));
		float fLightAngle = dot(g_atmosParam.v3LightDirection, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (g_atmosParam.v3InvWavelength * g_atmosParam.fKr4PI + g_atmosParam.fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}
	
	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader

	mieColor.rgb =  v3FrontColor * g_atmosParam.fKmESun;
	rayColor.rgb = v3FrontColor * (g_atmosParam.v3InvWavelength * g_atmosParam.fKrESun);
	posToCameraDir = cameraPos - worldPos;
}
/*!
 * @brief	��C��������I�u�W�F�N�g���������̃~�[�����ƃ��C���[�������v�Z�B
 */
void CalcMieAndRayleighColorsObjectFromAtomosphere( out float4 mieColor, out float4 rayColor, out float3 posToCameraDir, float3 worldPos )
{
//	worldPos.y += g_atmosParam.fInnerRadius;
	float3 cameraPos = g_cameraPos.xyz;
	cameraPos *= 0.001f; //�P�ʂ�m����km�ɕύX�B
	cameraPos.y += g_atmosParam.fInnerRadius;
	mieColor = 0.0f;
	rayColor = 0.0f;
	worldPos *= 0.001f; //�P�ʂ�m����km�ɕύX�B
	worldPos.y += g_atmosParam.fInnerRadius;
	worldPos.y = min(worldPos.y, cameraPos.y);
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3Ray = worldPos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
//	float fNear = getNearIntersection(g_cameraPos.xyz, v3Ray, g_atmosParam.fCameraHeight2, g_atmosParam.fOuterRadius2);
	
	float3 v3Start = cameraPos;
	float fDepth = exp((g_atmosParam.fInnerRadius - g_atmosParam.fCameraHeight) / fScaleDepth);
	float fCameraAngle = dot(-v3Ray, worldPos) / length(worldPos);
	float fLightAngle = dot(g_atmosParam.v3LightDirection, worldPos) / length(worldPos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * g_atmosParam.fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	
	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	float3 v3Attenuate;
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(g_atmosParam.fScaleOverScaleDepth * (g_atmosParam.fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (g_atmosParam.v3InvWavelength * g_atmosParam.fKr4PI + g_atmosParam.fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}
	
	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader

	mieColor.rgb =  v3Attenuate;
	rayColor.rgb = v3FrontColor * (g_atmosParam.v3InvWavelength * g_atmosParam.fKrESun + g_atmosParam.fKmESun);
	posToCameraDir = cameraPos - worldPos;
}
/*!
 *@brief	��C�����V�~�����[�V�����B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
 */

void CalcMieAndRayleighColors(out float4 mieColor, out float4 rayColor, out float3 posToCameraDir, float3 worldPos)
{
	if(g_flags2.y == AtomosphereFuncObjectFromAtomosphere){
		CalcMieAndRayleighColorsObjectFromAtomosphere(mieColor, rayColor, posToCameraDir, worldPos);
	}else if(g_flags2.y == AtomosphereFuncSkyFromAtomosphere){
		CalcMieAndRayleighColorsSkyFromAtomosphere(mieColor, rayColor, posToCameraDir, worldPos);
	}else{
		mieColor = 0.0f;
		rayColor = 0.0f;
	}
}

/*!
 *@brief	���[���h���W�ƃ��[���h�@�����X�L���s�񂩂�v�Z����B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
 */
void CalcWorldPosAndNormalFromSkinMatrix( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent, uniform bool calcNormal )
{
	Pos = 0.0f;
	Normal = 0.0f;
	Tangent = 0.0f;
	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);
	
	//�u�����h���[�g�B
	float BlendWeightsArray[4] = (float[4])In.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(In.Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        if(calcNormal){
	        Normal += mul(In.Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    	    Tangent += mul(In.Tangent, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    	}
    }
    LastWeight = 1.0f - LastWeight; 
    
	Pos += (mul(In.Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
	if(calcNormal){
	    Normal += (mul(In.Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    	Tangent += (mul(In.Tangent, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    }
}
/*!
 *@brief	���[���h���W�ƃ��[���h�@�����v�Z�B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
 */
void CalcWorldPosAndNormal( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent, uniform bool calcNormal )
{
	Normal = 0.0f;
	Tangent = 0.0f;
	Pos = mul(In.Pos, g_worldMatrix );
	if(calcNormal){
		Normal = mul(In.Normal, g_worldMatrix );
		Tangent = mul(In.Tangent, g_worldMatrix );
	}
}
/*!
 *@brief	���_�V�F�[�_�[�B
 *@param[in]	In			���͒��_�B
 *@param[in]	hasSkin		�X�L������H
 *@param[in]	doStealth	�X�e���X�H
 */
VS_OUTPUT VSMain( VS_INPUT In, uniform bool hasSkin, uniform bool doStealth )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent, true );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent, true );
	}
	o.worldPos_depth.xyz = Pos.xyz;

    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.worldPos_depth.w = o.Pos.w;
    o.Tex0 = In.Tex0;
    o.velocity = mul(float4(Pos.xyz, 1.0f), g_mViewProjLastFrame);
    o.screenPos = o.Pos;
    if(doStealth == false){
		//������ւ�̓X�e���X���ʂ̂Ƃ��͌v�Z���Ȃ��Ă����̂ŃJ�b�g�B
    	o.Normal = normalize(Normal);
	    o.Tangent = normalize(Tangent);
	    CalcMieAndRayleighColors( o.mieColor, o.rayColor, o.posToCameraDir, o.worldPos_depth.xyz );
	}
	return o;
}
/*!
 *@brief	���_�V�F�[�_�[�B
 *@param[in]	In			���͒��_�B
 *@param[in]	hasSkin		�X�L������H
 */
VS_OUTPUT VSMainInstancing( VS_INPUT_INSTANCING In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal, Tangent, true );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In.base, Pos, Normal, Tangent, true );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//���[���h�s���������B

	o.worldPos_depth.xyz = Pos.xyz;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.worldPos_depth.w = o.Pos.w;
    o.Normal = mul(normalize(Normal), worldMat);
    o.Tex0 = In.base.Tex0;
   	o.velocity = mul(float4(Pos.xyz, 1.0f), g_mViewProjLastFrame);
   	o.screenPos = o.Pos;
   	CalcMieAndRayleighColors( o.mieColor, o.rayColor, o.posToCameraDir, o.worldPos_depth.xyz );
   	
	return o;
}

/*!
 * @brief	�s�N�Z���V�F�[�_�[�B
 */
PSOutput PSMain( VS_OUTPUT In )
{
	float4 color = 0.0f;
	float4 diffuseColor = tex2D(g_diffuseTextureSampler, In.Tex0);
	color = diffuseColor;
	float3 normal = normalize(In.Normal);
	if(g_flags.x){
		//�@���}�b�v����B
		float3 tangent = normalize(In.Tangent);
		float3 binSpaceNormal = tex2D( g_normalMapSampler, In.Tex0);
		float4x4 tangentSpaceMatrix;
		float3 biNormal = normalize( cross( tangent, normal) );
		tangentSpaceMatrix[0] = float4( tangent, 0.0f);
		tangentSpaceMatrix[1] = float4( biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4( normal, 0.0f);
		tangentSpaceMatrix[3] = float4( 0.0f, 0.0f, 0.0f, 1.0f );
		//-1.0�`1.0�͈̔͂Ƀ}�b�s���O����B
		binSpaceNormal = (binSpaceNormal * 2.0f)- 1.0f;
		normal = tangentSpaceMatrix[0] * binSpaceNormal.x + tangentSpaceMatrix[1] * binSpaceNormal.y + tangentSpaceMatrix[2] * binSpaceNormal.z; 
		
	}
	
	float4 lig = DiffuseLight(normal);
	if(g_flags.z){
		//�������C�g�B
		lig.xyz += CalcLimLight(normal, g_light.limLightDir, g_light.limLightColor.xyz);
	}
	if(g_flags.w){
		//�X�y�L�������C�g�B
		lig.xyz += SpecLight(normal, In.worldPos_depth.xyz, In.Tex0);
	}
	
	if(g_flags.y){
		//�e
		lig *= CalcShadow(In.worldPos_depth.xyz);
	
	}
	//���Ȕ����F
	lig.xyz += g_light.emission;
	
	color *= lig;

	//��C����
	if(g_flags2.y == AtomosphereFuncObjectFromAtomosphere)
	{
		color = In.rayColor + color * In.mieColor;
	}
	
	//�|�C���g���C�g�B
	color.xyz += diffuseColor.xyz * PointLight(normal, In.worldPos_depth.xyz, g_flags.z);
	
	//�A���r�G���g���C�g�����Z�B
	color.xyz += diffuseColor.xyz * g_light.ambient.xyz;	
	
	if(g_fogParam.z > 1.9f){
		//�����t�H�O
		float h = max(In.worldPos_depth.y - g_fogParam.y, 0.0f);
		float t = min(h / g_fogParam.x, 1.0f);
		color.xyz = lerp(float3(0.75f, 0.75f, 0.95f), color.xyz, t);
	}else if(g_fogParam.z > 0.0f){
		//�����t�H�O
		float z = length(In.worldPos_depth.xyz - g_cameraPos);
		z = max(z - g_fogParam.x, 0.0f);
		float t = min( z / g_fogParam.y, 1.0f);
		color.xyz = lerp(color.xyz, float3(0.75f, 0.75f, 0.95f), t);
	}
	
	PSOutput psOut = (PSOutput)0;
	psOut.color = color;
	psOut.depth = In.worldPos_depth.w;
	if(g_flags2.x){
		psOut.velocity.xy = In.velocity.xy / In.velocity.w-In.screenPos.xy / In.screenPos.w;
		psOut.velocity.xy *= 0.5f;
		psOut.velocity.xy += 0.5f;
		psOut.velocity.zw = 0.0f;
	}else{
		//���x�Ȃ��B
		psOut.velocity = 0.5f;
	}
	return psOut;
}
/*!
 * @brief	�X�e���X���ʗp�̃V�F�[�_�[�B
 */
PSOutput PSStealthMain(VS_OUTPUT In)
{
	float4 color = 0.0f;
	float4 diffuseColor = tex2D(g_diffuseTextureSampler, In.Tex0);
	color = diffuseColor;
	
	PSOutput psOut = (PSOutput)0;
	psOut.color = color;
	psOut.depth = In.worldPos_depth.w;
	if(g_flags2.x){
		psOut.velocity.xy = In.velocity.xy / In.velocity.w-In.screenPos.xy / In.screenPos.w;
		psOut.velocity.xy *= 0.5f;
		psOut.velocity.xy += 0.5f;
		psOut.velocity.zw = 0.0f;
	}else{
		//���x�Ȃ��B
		psOut.velocity = 0.5f;
	}
	
	return psOut;
}
/*!
 * @brief	��`��p�̒��_�V�F�[�_�[�B
 */
VS_OUTPUT VSSkyMain(VS_INPUT In)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	//��ɃX�L���͂Ȃ��B
	//�@���̌v�Z������Ȃ��B
	CalcWorldPosAndNormal( In, Pos, Normal, Tangent, false);
	o.worldPos_depth.xyz = Pos.xyz;
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.worldPos_depth.w = o.Pos.w;
    o.Tex0 = In.Tex0;
    o.Normal = In.Normal;
    o.velocity = mul(float4(Pos.xyz, 1.0f), g_mViewProjLastFrame);
    o.screenPos = o.Pos;
    CalcMieAndRayleighColors( o.mieColor, o.rayColor, o.posToCameraDir, o.worldPos_depth.xyz );
	return o;
}
/*!
 * @brief	��`��p�̃s�N�Z���V�F�[�_�[�B
 */
PSOutput PSSkyMain(VS_OUTPUT In){
	float4 diffuseColor = texCUBE(g_skyCubeMapSampler, In.Normal * -1.0f);
	float4 color = 0;
	//��̃e�N�X�`���𔒍���
	float3 monochrome = float3(0.29900f, 0.58700f, 0.11400f );
	float Y  =  dot(monochrome, diffuseColor);
	//�����������e�N�X�`����n�悵�Ĕ��ɋ߂��������������o���B
	float cloudRate = pow(Y, 3.0f );
	color = In.rayColor + 0.25f * In.mieColor;
	//��C�̐F�����m�N����
	float colorY = max( 0.0f, dot(monochrome, color) );
	//����t�͖�ɂȂ��1.0f�ɋ߂Â��Ă���B
//	float nightRate = pow( 1.0f - min(1.0f, colorY), 1.0f );
	float nightRate = max( 0.0f, dot(float3(0.0f, 1.0f, 0.0f ), g_atmosParam.v3LightDirection ));
	//���̐F
//	color.xyz += float3(0.0f, 0.0f, 0.1f ) * nightRate ;
	//�_�̐F�B���Ԃ�1.0f�Ŗ�Ԃ�0.3f
	float cloudColor = lerp(3.0f, 0.1f,pow( 1.0f - nightRate, 3.0f));
	//��̐F�Ɖ_�̐F�Ƃ̊Ԃ��_���Ő��`�⊮�B
	color.xyz = lerp( color.xyz, cloudColor, cloudRate ) ;
	PSOutput psOut = (PSOutput)0;
	psOut.color = color * 1.1f;
	psOut.depth = In.worldPos_depth.w;
	if(g_flags2.x){
		psOut.velocity.xy = In.velocity.xy / In.velocity.w-In.screenPos.xy / In.screenPos.w;
		psOut.velocity.xy *= 0.5f;
		psOut.velocity.xy += 0.5f;
		psOut.velocity.zw = 0.0f;
	}else{
		//���x�Ȃ��B
		psOut.velocity = 0.5f;
	}
	return psOut;
}
/*!
 * @brief	�V���h�E�}�b�v�`�����ݎ��̒��_�V�F�[�_�[�B
 */
VS_OUTPUT_RENDER_SHADOW_MAP VSMainRenderShadowMap( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT_RENDER_SHADOW_MAP o = (VS_OUTPUT_RENDER_SHADOW_MAP)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent, false );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent, false );
	}
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
	o.depth = o.Pos;
	return o;
}
/*!
 * @brief	�V���h�E�}�b�v�������ݎ��̒��_�V�F�[�_�[(�C���X�^���V���O��)
 */
VS_OUTPUT_RENDER_SHADOW_MAP VSMainInstancingRenderShadowMap(VS_INPUT_INSTANCING In, uniform bool hasSkin)
{
	VS_OUTPUT_RENDER_SHADOW_MAP o = (VS_OUTPUT_RENDER_SHADOW_MAP)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In.base, Pos, Normal, Tangent, false );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In.base, Pos, Normal, Tangent, false );
	}
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;
	Pos = mul(float4(Pos.xyz, 1.0f), worldMat );	//���[���h�s���������B
	o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
	o.depth = o.Pos;
	return o;
}
/*!
 * @brief	�V���h�E�}�b�v�`�����ݎ��̃s�N�Z���V�F�[�_�[�B
 */
float4 PSMainRenderShadowMap( VS_OUTPUT_RENDER_SHADOW_MAP In ) : COLOR
{

	float z = In.depth.z/In.depth.w;
	float dx = ddx(z);
	float dy = ddy(z);
	return float4(z, z*z+0.25f*(dx*dx+dy*dy), 0.0f, 1.0f);
}

texture g_splatMap;			//Splatmap
sampler g_splatMapSampler = 
sampler_state
{
	Texture = <g_splatMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
texture g_terrainTex0;
texture g_terrainTex1;
texture g_terrainTex2;
texture g_terrainTex3;
float4 g_terrainRect;	//!<�n�`��XZ���ʂŌ�����`�B

sampler g_terrainTexSampler[4] = {
	sampler_state
	{
		Texture = <g_terrainTex0>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
	sampler_state
	{
		Texture = <g_terrainTex1>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
	sampler_state
	{
		Texture = <g_terrainTex2>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
	sampler_state
	{
		Texture = <g_terrainTex3>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
};

texture g_terrainNormalMap0;
texture g_terrainNormalMap1;
texture g_terrainNormalMap2;
texture g_terrainNormalMap3;
sampler g_terrainNormalMapSampler[4] = {
	sampler_state
	{
		Texture = <g_terrainNormalMap0>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
	sampler_state
	{
		Texture = <g_terrainNormalMap1>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
	sampler_state
	{
		Texture = <g_terrainNormalMap2>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
	sampler_state
	{
		Texture = <g_terrainNormalMap3>;
		MipFilter = LINEAR;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		AddressU = Wrap;
		AddressV = Wrap;
	},
};
/*!
 *@brief	�n�`�p�s�N�Z���V�F�[�_�[�B
 */
PSOutput PSTerrain(VS_OUTPUT In) : COLOR
{
	//�X�v���b�g�}�b�v��UV���W�����߂�B
	float2 splatMapUV;
	splatMapUV.x = (In.worldPos_depth.x - g_terrainRect.x) / (g_terrainRect.y-g_terrainRect.x);
	splatMapUV.y = (In.worldPos_depth.z - g_terrainRect.z) / (g_terrainRect.w-g_terrainRect.z);
	float4 splatMap = tex2D(g_splatMapSampler, splatMapUV);
	float t = splatMap.r + splatMap.g + splatMap.b /*+ splatMap.w*/;
	float4 weights = float4(splatMap.r/t, splatMap.g/t, splatMap.b/t, splatMap.w/t);
	
	float4 diffuseColor = tex2D(g_terrainTexSampler[0], In.Tex0) * weights.x;
	diffuseColor += tex2D(g_terrainTexSampler[1], In.Tex0) * weights.y;
	diffuseColor += tex2D(g_terrainTexSampler[2], In.Tex0) * weights.z;
	//diffuseColor += tex2D(g_terrainTexSampler[3], In.Tex0) * weights.w;
	float4 color = diffuseColor;
	
	float3 normal = normalize(In.Normal);
	//�f�Bwwwww�t���[�Y���C�g
	float4 lig = DiffuseLight(normal);
	//�e
	lig *= CalcShadow(In.worldPos_depth.xyz);
	color *= lig;
	
	//��C�����B
	color = In.rayColor + color * In.mieColor;
	
	//�|�C���g���C�g�B
	color.xyz += diffuseColor.xyz * PointLight(normal, In.worldPos_depth.xyz, 0);
	//�A���r�G���g���C�g�����Z�B
	color.xyz += diffuseColor.xyz * g_light.ambient.xyz;
	
	PSOutput psOut = (PSOutput)0;
	psOut.velocity.xy = In.velocity.xy / In.velocity.w-In.screenPos.xy / In.screenPos.w;
	psOut.velocity.xy *= 0.5f;
	psOut.velocity.xy += 0.5f;
	psOut.velocity.zw = 0.0f;
	
	psOut.color = color;
	psOut.depth = In.worldPos_depth.w;
	
	return psOut;
}

/*!
 *@brief	�X�L�����胂�f���p�̃e�N�j�b�N�B
 */
technique SkinModel
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMain(true, false);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	�X�L���Ȃ����f���p�̃e�N�j�b�N�B
 */
technique NoSkinModel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain(false, false);
		PixelShader = compile ps_3_0 PSMain();
	}
}
/*!
 *@brief	�X�L�����胂�f���̃C���X�^���V���O�`��p�̃e�N�j�b�N�B
 */
technique SkinModelInstancing
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMainInstancing(true);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	�X�L���Ȃ����f���̃C���X�^���V���O�`��p�̃e�N�j�b�N�B
 */
technique NoSkinModelInstancing
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 PSMain();
	}
}

/*!
 * @brief	�X�L�����胂�f���p�̃V���h�E�}�b�v�`�����݃e�N�j�b�N�B
 */
technique SkinModelRenderShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainRenderShadowMap(true);
		PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}

/*!
 * @brief	�X�L���Ȃ����f���p�̃V���h�E�}�b�v�`�����݃e�N�j�b�N�B
 */
technique NoSkinModelRenderShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainRenderShadowMap(false);
		PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}
technique SkinModelInstancingRenderToShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancingRenderShadowMap(true);
        PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}
technique NoSkinModelInstancingRenderToShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancingRenderShadowMap(false);
        PixelShader = compile ps_3_0 PSMainRenderShadowMap();
	}
}
/*!
 * @brief	��`��p�̃e�N�j�b�N�B
 */
technique Sky{
	pass p0
	{
		VertexShader =  compile vs_3_0 VSSkyMain();
		PixelShader = compile ps_3_0 PSSkyMain();
	}
}
/*!
 * @brief	�X�L������X�e���X�B
 */
technique StealthSkin{
	pass p0
	{
		VertexShader =  compile vs_3_0 VSMain(true, true);
		PixelShader = compile ps_3_0 PSStealthMain();
	}
}
/*!
 *@brief	�n�`
 */
technique Terrain{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain(false, false);
		PixelShader = compile ps_3_0 PSTerrain();
	}
}