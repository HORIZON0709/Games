//================================================
//
//3D�Q�[��(��)[objectMesh.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECTMESH_H_
#define _OBJECTMESH_H_

//***************************
//�C���N���[�h
//***************************
#include "object3D.h"
#include "texture.h"

//***************************
//�I�u�W�F�N�g���b�V���N���X�̒�`
//***************************
class CObjectMesh : public CObject
{/* CObject�̔h���N���X */
private: /* �萔�̒�` */
	static const int NUM_VERTEX;	//1�|���S���̒��_��

	static const int NUM_BLK_X;	//�u���b�N��(X��)
	static const int NUM_BLK_Z;	//�u���b�N��(Z��)

	static const int NUM_VTX_X;	//���_��(X��)
	static const int NUM_VTX_Z;	//���_��(Z��)

	static const float MESH_SIZE;	//���b�V���̃T�C�Y

public: /* �ÓI�����o�֐� */
	static CObjectMesh* Create();	//����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CObjectMesh();
	~CObjectMesh() override;

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

public: /* Rot */
	/*
		������ݒ�
		const D3DXVECTOR3 &pos ---> �C�ӂ̌���
	*/
	void SetRot(const D3DXVECTOR3 &rot);

	//�������擾
	D3DXVECTOR3 GetRot();

public: /* �����o�֐� */
	/*
		�����蔻��
		D3DXVECTOR3* pPos ---> �ʒu�̃|�C���^
	*/
	bool Collision(D3DXVECTOR3* pPos);

	/*
		2D�x�N�g���̓���
		D3DXVECTOR3* vec1 ---> �C�ӂ̃x�N�g��1
		D3DXVECTOR3* vec2 ---> �C�ӂ̃x�N�g��2
	*/
	float Vec2Dot(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

	/*
		2D�x�N�g���̊O��
		D3DXVECTOR3* vec1 ---> �C�ӂ̃x�N�g��1
		D3DXVECTOR3* vec2 ---> �C�ӂ̃x�N�g��2
	*/
	float Vec2Cross(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);
private:
	int AskVtx();	//���_�������߂�
	int AskIdx();	//�C���f�b�N�X�������߂�
	int AskPol();	//�|���S���������߂�

private: /* �����o�ϐ� */
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	CTexture::TEXTURE m_texture;	//�e�N�X�`���̗񋓌^

	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����

	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X

	float m_fWidth;		//��
	float m_fHeight;	//����

	int m_nNumVtx;	//���_��
	int m_nNumIdx;	//�C���f�b�N�X��
	int m_nNumPol;	//�|���S����
};

#endif