//================================================
//
//3D�Q�[��(��)[polygon3D.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

//***************************
//�C���N���[�h
//***************************
#include "object3D.h"

//***************************
//3D�|���S���N���X�̒�`
//***************************
class CPolygon3D : public CObject3D
{/* CObject3D�̔h���N���X */
public: /* �萔�̒�` */
	static const float POLYGON_WIDTH;	//����
	static const float POLYGON_HEIGHT;	//�c��

public: /* �ÓI�����o�֐� */
	static CPolygon3D* Create();	//����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CPolygon3D();
	~CPolygon3D()override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��
};
#endif