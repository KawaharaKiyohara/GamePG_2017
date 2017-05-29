#pragma once
#include "Lib/Object3D.h"

#include "tkEngine/Physics/tkMeshCollider.h"
#include "tkEngine/Physics/tkRigidBody.h"

class Ground :
	public Object3D
{
public:
	Ground();
	~Ground();
	bool Start() override;
	CMeshCollider			meshCollider;		//���b�V���R���C�_�[�B
	CRigidBody				rigidBody;			//���́B
};

