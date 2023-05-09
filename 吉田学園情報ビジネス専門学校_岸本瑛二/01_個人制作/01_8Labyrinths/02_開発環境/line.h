//================================================
//
//3D�Q�[��(��)[line.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _LINE_H_
#define _LINE_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//�萔�̒�`
//***************************
const DWORD FVF_VERTEX_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);	//���_�t�H�[�}�b�g

//***************************
//���C���N���X�̒�`
//***************************
class CLine
{/* ��{�N���X */
public: /* �\���̂̒�` */
	struct VERTEX_LINE
	{//���_�f�[�^
		D3DXVECTOR3 pos;	//���_���W
		D3DCOLOR	col;	//���_�J���[
	};

public: /* �ÓI�����o�֐� */
	static CLine* Create();	//����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CLine();
	~CLine();

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init();	//������
	void Uninit();	//�I��
	void Update();	//�X�V
	void Draw();	//�`��

public: /* �����o�֐� */
	/*
		�ݒ�
		const D3DXVECTOR3 &pos ---> �ʒu
		const D3DXVECTOR3 &rot ---> ����
		const D3DXVECTOR3 &start ---> �n�_
		const D3DXVECTOR3 &end ---> �I�_
		const D3DXCOLOR &col ---> �F
	*/
	void Set(
		const D3DXVECTOR3 &pos,
		const D3DXVECTOR3 &rot,
		const D3DXVECTOR3 &start,
		const D3DXVECTOR3 &end,
		const D3DXCOLOR &col);

private: /* �����o�ϐ� */
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_start;	//�n�_
	D3DXVECTOR3 m_end;		//�I�_

	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X

	D3DXCOLOR m_col;	//�F
};
#endif