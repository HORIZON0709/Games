//================================================
//
//3D�Q�[��(��)[panel.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "panel.h"
#include "application.h"
#include "renderer.h"
#include "objectX.h"
#include "object2D.h"
#include "input.h"
#include "game.h"
#include "sound.h"

#include "debug_proc.h"
#include "utility.h"

#include <assert.h>

//***************************
//�萔�̒�`
//***************************
const float CPanel::PANEL_SIZE = 100.0f;	//�p�l���̃T�C�Y

const D3DXVECTOR3 CPanel::PANELSTAND_POS = D3DXVECTOR3(200.0f, 0.0f, 200.0f);	//�p�l���X�^���h�̈ʒu

//***************************
//�ÓI�����o�ϐ�
//***************************
CPanel::PANEL_INFO CPanel::m_aPanelInfo[GRID_Y][GRID_X] = {};	//�p�l�����

//================================================
//����
//================================================
CPanel* CPanel::Create()
{
	CPanel* pPanel = nullptr;	//�|�C���^

	if (pPanel != nullptr)
	{//NULL�`�F�b�N
		assert(false);
	}

	/* nullptr�̏ꍇ */

	pPanel = new CPanel;	//�������̓��I�m��

	pPanel->Init();	//������

	return pPanel;	//���I�m�ۂ������̂�Ԃ�
}

//================================================
//�R���X�g���N�^
//================================================
CPanel::CPanel() :
	m_pPanelStand(nullptr),
	m_pBg(nullptr),
	m_pSelect(nullptr),
	m_nPosX(0),
	m_nPosY(0),
	m_bCollPlayer(false),
	m_bIsPanel(false),
	m_bIsSelect(false)
{
	//�����o�ϐ��̃N���A
	memset(&m_aPos, 0, sizeof(m_aPos));
}

//================================================
//�f�X�g���N�^
//================================================
CPanel::~CPanel()
{
}

//================================================
//������
//================================================
HRESULT CPanel::Init()
{
	//�����o�ϐ��̏�����
	m_nPosX = 0;
	m_nPosY = 0;
	m_bCollPlayer = false;
	m_bIsPanel = false;
	m_bIsSelect = false;

	float fWidth = (float)CRenderer::SCREEN_WIDTH;		//��ʂ̉���
	float fHeight = (float)CRenderer::SCREEN_HEIGHT;	//��ʂ̏c��

	{//�p�l���̈ʒu��ݒ肷��
		D3DXVECTOR3 aPos[MAX_PANEL] =
		{//�p�l���̈ʒu(�Œ�)
			D3DXVECTOR3(fWidth * 0.4f, fHeight * 0.3f, 0.0f),	//1
			D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.3f, 0.0f),	//2
			D3DXVECTOR3(fWidth * 0.6f, fHeight * 0.3f, 0.0f),	//3

			D3DXVECTOR3(fWidth * 0.4f, fHeight * 0.5f, 0.0f),	//4
			D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f),	//5
			D3DXVECTOR3(fWidth * 0.6f, fHeight * 0.5f, 0.0f),	//6

			D3DXVECTOR3(fWidth * 0.4f, fHeight * 0.7f, 0.0f),	//7
			D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.7f, 0.0f),	//8
			D3DXVECTOR3(fWidth * 0.6f, fHeight * 0.7f, 0.0f)	//9
		};

		for (int Y = 0; Y < GRID_Y; Y++)
		{
			for (int X = 0; X < GRID_X; X++)
			{
				//3�~3�}�X�ł̈ʒu��ۑ�����
				m_aPos[Y][X] = aPos[X + (Y * 3)];
			}
		}
	}

	/* �p�l���X�^���h */

	//����
	m_pPanelStand = CObjectX::Create("data/MODEL/PanelStand.x");

	//�ʒu�̐ݒ�
	m_pPanelStand->SetPos(PANELSTAND_POS);

	/* �w�i */

	//����
	m_pBg = CObject2D::Create();

	//�e���̐ݒ�
	m_pBg->SetPos(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f));
	m_pBg->SetSize(D3DXVECTOR2(fWidth, fHeight));
	m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));

	//�`�悵�Ȃ�
	m_pBg->SetIsDraw(false);

	/* �I��p�p�l�� */

	//����
	m_pSelect = CObject2D::Create();

	//�e���̐ݒ�
	m_pSelect->SetPos(m_aPos[0][0]);
	m_pSelect->SetSize(D3DXVECTOR2(PANEL_SIZE + 20.0f, PANEL_SIZE + 20.0f));
	m_pSelect->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	//�`�悵�Ȃ�
	m_pSelect->SetIsDraw(false);

	int nTex = CTexture::TEXTURE::Number_Single_1;	//�e�N�X�`���ݒ�p
	int nStage = CStage::STAGE::Stage01;			//�X�e�[�W�ݒ�p

	for (int Y = 0; Y < GRID_Y; Y++)
	{
		for (int X = 0; X < GRID_X; X++)
		{
			//����
			m_aPanelInfo[Y][X].m_pPanel = CObject2D::Create();

			//�ʒu�̐ݒ�
			m_aPanelInfo[Y][X].m_pPanel->SetPos(m_aPos[Y][X]);

			//�T�C�Y�̐ݒ�
			m_aPanelInfo[Y][X].m_pPanel->SetSize(D3DXVECTOR2(PANEL_SIZE, PANEL_SIZE));

			//�e�N�X�`���̐ݒ�
			m_aPanelInfo[Y][X].m_pPanel->SetTexture((CTexture::TEXTURE)nTex);

			//���̃e�N�X�`���ɂ���
			nTex++;

			//�`�悵�Ȃ�
			m_aPanelInfo[Y][X].m_pPanel->SetIsDraw(false);

			//�X�e�[�W��ݒ�
			m_aPanelInfo[Y][X].stage = (CStage::STAGE)nStage;

			if (Y == (GRID_Y - 1) && X == (GRID_X - 1))
			{//�󔒕����̓X�e�[�W����
				m_aPanelInfo[Y][X].stage = CStage::STAGE::NONE;
			}

			//���̃X�e�[�W�ɂ���
			nStage++;
		}
	}

	return S_OK;
}

//================================================
//�I��
//================================================
void CPanel::Uninit()
{
	/* �T�E���h */

	CApplication::GetSound()->Stop();

	for (int Y = 0; Y < GRID_Y; Y++)
	{
		for (int X = 0; X < GRID_X; X++)
		{
			if (m_aPanelInfo[Y][X].m_pPanel != nullptr)
			{//NULL�`�F�b�N
				m_aPanelInfo[Y][X].m_pPanel->SetDeathFlag();
				m_aPanelInfo[Y][X].m_pPanel = nullptr;
			}
		}
	}

	if (m_pSelect != nullptr)
	{//NULL�`�F�b�N
		m_pSelect->SetDeathFlag();
		m_pSelect = nullptr;
	}

	if (m_pBg != nullptr)
	{//NULL�`�F�b�N
		m_pBg->SetDeathFlag();
		m_pBg = nullptr;
	}

	if (m_pPanelStand != nullptr)
	{//NULL�`�F�b�N
		m_pPanelStand->SetDeathFlag();
		m_pPanelStand = nullptr;
	}
}

//================================================
//�X�V
//================================================
void CPanel::Update()
{
	if (m_bCollPlayer &&
		CApplication::GetInputKeyboard()->GetTrigger(DIK_E))
	{//�u�v���C���[���������Ă���v���uE�L�[���������v�ꍇ
		//���쒆�F�񑀍�̐؂�ւ�
		m_bIsPanel = m_bIsPanel ? false : true;

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Enter);
	}

	/* �`�悷�� */

	//�w�i
	m_pBg->SetIsDraw(m_bIsPanel);

	//�I��p�p�l��
	m_pSelect->SetIsDraw(m_bIsPanel);

	for (int Y = 0; Y < GRID_Y; Y++)
	{
		for (int X = 0; X < GRID_X; X++)
		{
			if (m_aPanelInfo[Y][X].m_pPanel == nullptr)
			{//NULL�`�F�b�N
				continue;
			}

			/* nullptr�ł͂Ȃ��ꍇ */

			//�p�l��
			m_aPanelInfo[Y][X].m_pPanel->SetIsDraw(m_bIsPanel);

			//���݂̃X�e�[�W���擾
			CStage::STAGE stage = CGame::GetStage()->Get();

			if (m_aPanelInfo[Y][X].stage == stage)
			{//���݂̃X�e�[�W�ɊY������p�l����T��
				//�F���Â�����
				m_aPanelInfo[Y][X].m_pPanel->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				continue;
			}

			/* ����ȊO�̃p�l�� */

			//�F�����ɖ߂�
			m_aPanelInfo[Y][X].m_pPanel->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if (!m_bIsPanel)
	{//�p�l����������Ă��Ȃ��ꍇ
		return;
	}

	/* �p�l�����쒆�̏ꍇ */

	//�p�l���̑I��
	SelectPanel();
}

//================================================
//�`��
//================================================
void CPanel::Draw()
{
}

//================================================
//�p�l�����쒆���ǂ������擾
//================================================
bool CPanel::GetIsPanel()
{
	return m_bIsPanel;
}

//================================================
//�p�l�����̎擾
//================================================
CPanel::PANEL_INFO CPanel::GetPanelInfo(int Y, int X)
{
	return m_aPanelInfo[Y][X];
}

//================================================
//�p�l���X�^���h�̎擾
//================================================
CObjectX* CPanel::GetPanelStand()
{
	return m_pPanelStand;
}

//================================================
//�v���C���[���������Ă��邩�ǂ�����ݒ�
//================================================
void CPanel::SetCollPlayer(bool bCollPlayer)
{
	m_bCollPlayer = bCollPlayer;
}

//================================================
//�p�l���̑I��
//================================================
void CPanel::SelectPanel()
{
	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{//Enter�L�[
		//�I���F��I���̐؂�ւ�
		m_bIsSelect = m_bIsSelect ? false : true;

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Enter);
	}

	//�I��p�p�l���̈ړ�
	MoveSelect();

	//�I��p�p�l���̐F�̐ݒ�
	SetSelectColor();

	//�p�l���̈ړ�
	MovePanel();
}

//================================================
//�I��p�p�l���̈ړ�
//================================================
void CPanel::MoveSelect()
{
	if (m_bIsSelect)
	{//�I�𒆂̏ꍇ
		return;
	}

	/* �I�𒆂ł͂Ȃ��ꍇ */

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_W))
	{//W�L�[����
		m_nPosY--;	//-1����

		if (m_nPosY < 0)
		{//0����(-1�ȉ�)�ɂȂ����ꍇ
			m_nPosY = 0;	//0�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}
	else if (CApplication::GetInputKeyboard()->GetTrigger(DIK_S))
	{//S�L�[����
		m_nPosY++;	//+1����

		if (m_nPosY >= GRID_Y)
		{//2���傫��(3�ȏ�)�ɂȂ����ꍇ
			m_nPosY = GRID_Y - 1;	//2�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_A))
	{//A�L�[����
		m_nPosX--;	//-1����

		if (m_nPosX < 0)
		{//0����(-1�ȉ�)�ɂȂ����ꍇ
			m_nPosX = 0;	//0�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}
	else if (CApplication::GetInputKeyboard()->GetTrigger(DIK_D))
	{//D�L�[����
		m_nPosX++;	//+1����

		if (m_nPosX >= GRID_X)
		{//2���傫��(3�ȏ�)�ɂȂ����ꍇ
			m_nPosX = GRID_X - 1;	//2�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}

	//�I��p�p�l���̈ʒu�̐ݒ�
	m_pSelect->SetPos(m_aPos[m_nPosY][m_nPosX]);
}

//================================================
//�I��p�p�l���̐F�̐ݒ�
//================================================
void CPanel::SetSelectColor()
{
	if (m_aPanelInfo[m_nPosY][m_nPosX].stage == CGame::GetStage()->Get() ||
		m_aPanelInfo[m_nPosY][m_nPosX].stage == CStage::STAGE::NONE)
	{//�u���݋���X�e�[�W�Ɠ����p�l���v�������́u�X�e�[�W��񂪖����p�l���v�̏ꍇ
		m_bIsSelect = false;
		return;
	}

	/* �u���݋���X�e�[�W�ł͂Ȃ��p�l���v���u�X�e�[�W��񂪂���p�l���v�̏ꍇ */
	
	//�F�ݒ�p
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

	if (m_bIsSelect)
	{//�I�𒆂̏ꍇ
		//�F��ς���
		col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
	}

	//�F�̐ݒ�
	m_pSelect->SetCol(col);
}

//================================================
//�p�l���̈ړ�
//================================================
void CPanel::MovePanel()
{
	if (!m_bIsSelect)
	{//�I�𒆂ł͂Ȃ��ꍇ
		return;
	}

	/* �I�𒆂̏ꍇ */

	//���݈ʒu�̔ԍ���ۑ�
	int nPosX = m_nPosX;	//X
	int nPosY = m_nPosY;	//Y

	if (m_aPanelInfo[nPosX][nPosY].stage == CStage::STAGE::NONE)
	{//�I�������p�l���ɁA�Y������X�e�[�W�������ꍇ
		return;
	}

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_W))
	{//W�L�[����
		m_nPosY--;	//-1����

		if (m_nPosY < 0)
		{//0����(-1�ȉ�)�ɂȂ����ꍇ
			m_nPosY = 0;	//0�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}
	else if (CApplication::GetInputKeyboard()->GetTrigger(DIK_S))
	{//S�L�[����
		m_nPosY++;	//+1����

		if (m_nPosY >= GRID_Y)
		{//2���傫��(3�ȏ�)�ɂȂ����ꍇ
			m_nPosY = GRID_Y - 1;	//2�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_A))
	{//A�L�[����
		m_nPosX--;	//-1����

		if (m_nPosX < 0)
		{//0����(-1�ȉ�)�ɂȂ����ꍇ
			m_nPosX = 0;	//0�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}
	else if (CApplication::GetInputKeyboard()->GetTrigger(DIK_D))
	{//D�L�[����
		m_nPosX++;	//+1����

		if (m_nPosX >= GRID_X)
		{//2���傫��(3�ȏ�)�ɂȂ����ꍇ
			m_nPosX = GRID_X - 1;	//2�ɌŒ�
		}

		//SE��炷
		CApplication::GetSound()->Play(CSound::LABEL_SE_Select);
	}

	bool bCanMove = false;	//�ړ��ł��邩�ǂ���

	if (m_aPanelInfo[m_nPosY][m_nPosX].stage == CStage::STAGE::NONE)
	{//�ړ���ɃX�e�[�W�������ꍇ
		//�ړ��ł���
		bCanMove = true;
	}

	if (bCanMove)
	{//�ړ��ł���ꍇ
		//�p�l�����̃|�C���^
		PANEL_INFO* now = &m_aPanelInfo[nPosY][nPosX];		//���ݑI�����Ă���p�l��
		PANEL_INFO* dest = &m_aPanelInfo[m_nPosY][m_nPosX];	//�ړ���̃p�l��(�o�c)

		//�X�e�[�W����ۑ�
		CStage::STAGE stageNow = now->stage;
		CStage::STAGE stageDest = dest->stage;

		//�X�e�[�W�������ւ���
		dest->stage = stageNow;
		now->stage = stageDest;

		//�e�N�X�`������ۑ�
		CTexture::TEXTURE texNow = now->m_pPanel->GetTexture();		//���ݑI�����Ă���p�l��
		CTexture::TEXTURE texDest = dest->m_pPanel->GetTexture();	//�ړ���̃p�l��(�o�c)

		//�e�N�X�`���������ւ���
		dest->m_pPanel->SetTexture(texNow);
		now->m_pPanel->SetTexture(texDest);

		//�I������Ă��Ȃ���Ԃɂ���
		m_bIsSelect = false;

		//�ړ�����
		bCanMove = false;
	}
}