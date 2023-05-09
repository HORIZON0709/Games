//================================================
//
//3D�Q�[��(��)[result.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//***************************
//�C���N���[�h
//***************************
#include "mode.h"

#include <d3dx9.h>

//***************************
//�O���錾
//***************************
class CBg;
class CObject2D;

//***************************
//���U���g�N���X�̒�`
//***************************
class CResult : public CMode
{/* CMode�̔h���N���X */
private: /* �萔�̒�` */
	static const float GAMECLEAR_WIDTH;		//�Q�[���N���A�̕�
	static const float GAMECLEAR_HEIGHT;	//�Q�[���N���A�̍���

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CResult();
	~CResult() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

private: /* �ÓI�����o�ϐ� */
	static CBg* m_pBg;				//�w�i
	static CObject2D* m_pGameClear;	//�Q�[���N���A

private: /* �����o�ϐ� */
	bool m_bFadeOut;	//�Ó]�������ǂ���
};
#endif