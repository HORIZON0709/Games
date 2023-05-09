//================================================
//
//3D�Q�[��(��)[number.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//***************************
//�C���N���[�h
//***************************
#include "object2D.h"

//***************************
//�i���o�[�N���X�̒�`
//***************************
class CNumber : public CObject2D
{/* CObject2D�̔h���N���X */
public: /* �萔�̒�` */
	static const int DIVIDE_TEX_U = 10;	//�e�N�X�`���̕�����( U���� )

	static const float POS_X;			//�ʒu( X )
	static const float POS_Y;			//�ʒu( Y )
	static const float NUMBER_WIDTH;	//����
	static const float NUMBER_HEIGHT;	//�c��

public: /* �ÓI�����o�֐� */
	static CNumber* Create();	//����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CNumber();
	~CNumber()override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��
};
#endif