//================================================
//
//3D�Q�[��(��)[light.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//���C�g�N���X�̒�`
//***************************
class CLight
{/* ��{�N���X */
private: /* �萔�̒�` */
	static const int MAX_LIGHT = 3;	//���C�g�̍ő吔

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CLight();
	~CLight();

public: /* �����o�֐� */
	void Init();	//������
	void Uninit();	//�I��
	void Update();	//�X�V
private:
	/* �ݒ� */
	void Set(const int &nNum, const D3DXCOLOR &Diffuse, const D3DXVECTOR3 &vecDirection);

private: /* �����o�ϐ� */
	D3DLIGHT9 m_aLight[MAX_LIGHT];	//���C�g���
};

#endif