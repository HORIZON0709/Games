//================================================
//
//3D�Q�[��(��)[mode.h]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "mode.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include <assert.h>

//--------------------------------------------------
// ����
//--------------------------------------------------
CMode* CMode::Create(const MODE &mode)
{
	CMode* pMode = nullptr;	//�|�C���^

	switch (mode)
	{
	case MODE::TITLE: /* �^�C�g�� */
		pMode = new CTitle;	//�������̓��I�m��
		break;

	case MODE::GAME: /* �Q�[�� */
		pMode = new CGame;	//�������̓��I�m��
		break;

	case MODE::RESULT: /* ���U���g */
		pMode = new CResult;	//�������̓��I�m��
		break;

	case MODE::MAX:
	default: /* ����ȊO */
		assert(false);
		break;
	}

	if (pMode == nullptr)
	{// NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�ł͂Ȃ��ꍇ */

	pMode->Init();	//������

	return pMode;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CMode::CMode(MODE mode) :
	m_modeNext(MODE::NONE)
{
	m_mode = mode;
}

//================================================
//�f�X�g���N�^
//================================================
CMode::~CMode()
{
}

//================================================
//���[�h�̐ݒ�
//================================================
CMode* CMode::Set()
{
	if (m_modeNext == MODE::NONE)
	{//���̃��[�h�����܂��Ă��Ȃ�
		return this;
	}

	//���݂̃��[�h���I��
	Uninit();

	m_mode = m_modeNext;		//���[�h��ύX
	m_modeNext = MODE::NONE;	//�����Ȃ���Ԃɂ���
	
	return Create(m_mode);	//�ύX��̃��[�h�𐶐����ĕԂ�
}

//================================================
//���[�h�̎擾
//================================================
CMode::MODE CMode::Get()
{
	return m_mode;
}

//================================================
//���[�h�̕ύX
//================================================
void CMode::Change(const MODE &mode)
{
	assert(mode > MODE::NONE && mode < MODE::MAX);

	m_modeNext = mode;
}