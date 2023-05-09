//================================================
//
//3D�Q�[��(��)[objectX.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//***************************
//�C���N���[�h
//***************************
#include "object.h"

#include <stdio.h>

//***************************
//�I�u�W�F�N�g�N���X�̒�`
//***************************
class CObjectX : public CObject
{/* CObject�̔h���N���X */
public: /* �ÓI�����o�֐� */
	/*
		����
		char* pFileName ---> �t�@�C����
	*/
	static CObjectX* Create(char* pFileName);

	/*
		�t�@�C�����̐ݒ�
		char* pFileName ---> �t�@�C����
	*/
	static void SetFileName(char* pFileName);

private: /* �ÓI�����o�ϐ� */
	static char* m_pFileName;	//�t�@�C����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CObjectX();
	~CObjectX() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

public: /* Pos */
	/*
		�ʒu��ݒ�
		const D3DXVECTOR3 &pos ---> �C�ӂ̈ʒu
	*/
	void SetPos(const D3DXVECTOR3 &pos);

	//�ʒu���擾
	D3DXVECTOR3 GetPos();

public: /* Move */
	/*
		�ړ��ʂ�ݒ�
		const D3DXVECTOR3 &move ---> �C�ӂ̈ړ���
	*/
	void SetMove(const D3DXVECTOR3 &move);

	//�ړ��ʂ��擾
	D3DXVECTOR3 GetMove();

public: /* Rot */
	/*
		������ݒ�
		const D3DXVECTOR3 &rot ---> �C�ӂ̌���
	*/
	void SetRot(const D3DXVECTOR3 &rot);

	//�������擾
	D3DXVECTOR3 GetRot();

public: /* vtxMax,vtxMin */
	D3DXVECTOR3 GetVtxMax();	//���_�̍ő�l�̎擾
	D3DXVECTOR3 GetVtxMin();	//���_�̍ŏ��l�̎擾

private: /* �����o�ϐ� */
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_vtxMax;	//���_�̍ő�l
	D3DXVECTOR3 m_vtxMin;	//���_�̍ŏ��l
	
	LPD3DXMESH m_pMesh;			//���b�V�����̃|�C���^
	LPD3DXBUFFER m_pBuffMat;	//�}�e���A�����̃|�C���^
	DWORD m_numMat;				//�}�e���A�����̐�

	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
};

#endif