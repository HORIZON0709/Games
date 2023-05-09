//================================================
//
//3D�Q�[��(��)[gimmick.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

//***************************
//�C���N���[�h
//***************************
#include "objectX.h"

//***************************
//�O���錾
//***************************
class CLine;

//***************************
//�v���C���[�N���X�̒�`
//***************************
class CGimmick : public CObjectX
{/* CObjectX�̔h���N���X */
private: /* �萔�̒�` */
	static const float MOVE_SPEED;		//�ړ����x
	static const float ROT_SMOOTHNESS;	//��]�̊��炩��
	static const float ROTATION_SPEED;	//��]���x

	static const int MAX_LINE = 12;	//���C���̍ő吔

public: /* �ÓI�����o�֐� */
	/*
		����
		char* pFileName ---> �t�@�C����
	*/
	static CGimmick* Create(char* pFileName);

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CGimmick();
	~CGimmick() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

private: /* �����o�֐� */
	void SetLines();	//���C���̐ݒ�܂Ƃ�

private: /* �����o�ϐ� */
	CLine* m_apLine[MAX_LINE];	//���C���̃|�C���^

	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X

	D3DXVECTOR3 m_vec;		//�����x�N�g��
	D3DXVECTOR3 m_rotDest;	//�ړI�̌���

	D3DXQUATERNION m_quaternion;	//�N�H�[�^�j�I��
};

#endif