//================================================
//
//3D�Q�[��(��)[camera.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//�J�����N���X�̒�`
//***************************
class CCamera
{/* ��{�N���X */
public: /* �萔�̒�` */
	static const float MOVE_SPEED;	//�ړ����x

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CCamera();
	~CCamera();

public: /* �����o�֐� */
	void Init();	//������
	void Uninit();	//�I��
	void Update();	//�X�V
	void Set();		//�ݒ�

	D3DXMATRIX GetMatrixView();	//�r���[�}�g���N�X�̎擾
	D3DXVECTOR3 GetPosV();		//���_�̈ʒu�̎擾
private:
	void Move();	//�ړ�

private: /* �����o�ϐ� */
	D3DXVECTOR3 m_posV;	//���_
	D3DXVECTOR3 m_posR;	//�����_
	D3DXVECTOR3 m_vecU;	//������x�N�g��
	D3DXVECTOR3 m_move;	//�ړ���

	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
};

#endif