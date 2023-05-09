//================================================
//
//3D�Q�[��(��)[score.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "score.h"
#include "number.h"

#include "application.h"
#include "input.h"

#include <math.h>
#include <assert.h>

//================================================
//����
//================================================
CScore* CScore::Create()
{
	CScore* pScore = nullptr;	//�|�C���^

	if (pScore != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pScore = new CScore;	//�������̓��I�m��

	pScore->Init();	//������

	return pScore;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CScore::CScore():CObject::CObject(CObject::PRIORITY::PRIO_POLYGON_2D),
	m_pNumber(nullptr),
	m_nScore(0)
{
	//�^�C�v�̐ݒ�
	CObject::SetObjType(CObject::OBJ_TYPE::SCORE);
}

//================================================
//�f�X�g���N�^
//================================================
CScore::~CScore()
{
}

//================================================
//������
//================================================
HRESULT CScore::Init()
{
	if (m_pNumber != nullptr)
	{//NULL�`�F�b�N
		m_pNumber = nullptr;
	}

	/* nullptr�̏ꍇ */

	m_pNumber = CNumber::Create();	//�������̓��I�m��

	//�ʒu�̐ݒ�
	m_pNumber->SetPos(D3DXVECTOR3(CNumber::POS_X, CNumber::POS_Y, 0.0f));

	//�T�C�Y�̐ݒ�
	m_pNumber->SetSize(D3DXVECTOR2(CNumber::NUMBER_WIDTH, CNumber::NUMBER_HEIGHT));

	//�e�N�X�`���̐ݒ�
	m_pNumber->SetTexture(CTexture::TEXTURE::Number00);

	//�e�N�X�`�����W�̐ݒ�
	m_pNumber->SetTexUV(CNumber::DIVIDE_TEX_U, 0);

	return S_OK;
}

//================================================
//�I��
//================================================
void CScore::Uninit()
{
}

//================================================
//�X�V
//================================================
void CScore::Update()
{
	//�e�N�X�`�����W�̐ݒ�
	m_pNumber->SetTexUV(CNumber::DIVIDE_TEX_U, m_nScore);
}

//================================================
//�`��
//================================================
void CScore::Draw()
{
}

//================================================
//�X�R�A�̃Z�b�g
//================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//================================================
//�X�R�A���Z
//================================================
void CScore::AddScore(const int nValue)
{
	m_nScore += nValue;
}

//================================================
//���݂̃X�R�A�̎擾
//================================================
int CScore::GetCurrentScore()
{
	return m_nScore;
}
