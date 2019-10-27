//*****************************************************************************
// Thread�N���X
// �������J�I�v�V�����Ɂu-pthread�v��ǉ����邱��
//*****************************************************************************
#include "CThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <unistd.h>


//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CThread::CThread(const char* pszId)
{
	int			iRet = 0;


	m_strId = "";
	m_bInitFlag = false;
	m_iError = 0;
	m_hThread = 0;
	
	// �N���X����ێ�
	if (pszId != NULL)
	{
		m_strId = pszId;
	}

	// �~���[�e�b�N�����̏�����
	pthread_mutexattr_init(&m_tMutexAttr);
	iRet = pthread_mutexattr_settype(&m_tMutexAttr,PTHREAD_PROCESS_PRIVATE);		// �X���b�h�Ԃ̔r��
	if (iRet != 0)
	{
		return;
	}

	// �X���b�h�I���p�C�x���g������
	CEVENT_RET_ENUM eEventRet = m_cThreadEndEvent.Init();
	if (eEventRet != CEVENT_RET_SUCCESS)
	{
		return;
	}

	m_bInitFlag = true;
}


//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CThread::~CThread()
{
	// �X���b�h����~���Ă��Ȃ����Ƃ��l��
	this->Stop();

	// �~���[�e�b�N�X�����I�u�W�F�N�g��j��
	pthread_mutexattr_destroy(&m_tMutexAttr);
}


//-----------------------------------------------------------------------------
// �X���b�h�J�n
//-----------------------------------------------------------------------------
CTHREAD_RET_ENUM CThread::Start()
{
	int			iRet = 0;


	// �����������Ŏ��s���Ă���ꍇ
	if (m_bInitFlag == false)
	{
		return CTHREAD_RET_ERROR_INIT;
	}

	// ���ɓ��삵�Ă���ꍇ
	if (m_hThread != 0)
	{
		return CTHREAD_RET_ERROR_AlREADY_STARTED;
	}

	// �X���b�h�J�n
	iRet = pthread_create(&m_hThread, NULL,ThreadLauncher, this);
	if (iRet != 0)
	{
		m_iError = errno;
		printf("pthread_create Error. [errno�F%d]\n", m_iError);
		printf("-> %s\n", strerror(m_iError));
		return CTHREAD_RET_ERROR_START;
	}

	return CTHREAD_RET_SUCCESS;
}


//-----------------------------------------------------------------------------
// �X���b�h��~
//-----------------------------------------------------------------------------
CTHREAD_RET_ENUM CThread::Stop()
{
	CEVENT_RET_ENUM			eRet = CEVENT_RET_SUCCESS;


	// �����������Ŏ��s���Ă���ꍇ
	if (m_bInitFlag == false)
	{
		return CTHREAD_RET_ERROR_INIT;
	}

	// ���ɒ�~���Ă���ꍇ
	if (m_hThread == 0)
	{
		return CTHREAD_RET_SUCCESS;
	}

	// �X���b�h���~������
	eRet = m_cThreadEndEvent.SetEvent();
	if (eRet != CEVENT_RET_SUCCESS)
	{
		// �X���b�h��~�Ɏ��s�����ꍇ�́A�����I�ɏI��������
		pthread_cancel(m_hThread);
	}
	pthread_join(m_hThread, NULL);
	m_hThread = 0;

	return CTHREAD_RET_SUCCESS;
}


//-----------------------------------------------------------------------------
// �X���b�h�Ăяo��
//-----------------------------------------------------------------------------
void* CThread::ThreadLauncher(void* pUserData)
{
	// �X���b�h�����Ăяo��
	reinterpret_cast<CThread*>(pUserData)->ThreadProc();

	return (void *)NULL;
}



//-----------------------------------------------------------------------------
// �X���b�h�����i���T���v�����j
//-----------------------------------------------------------------------------
void CThread::ThreadProc()
{
	CEVENT_RET_ENUM eRet = CEVENT_RET_SUCCESS;
	DWORD			dwCount = 1;
	bool			bLoop = true;
	DWORD			dwTimeout = 0;


	printf("-- Thread %s Start --\n", m_strId.c_str());


	// pthread_testcancel���Ă΂��܂ŏ����𑱂���
	while (bLoop)
	{
		// Sleep���Ԃ𐶐�
		dwTimeout = ((rand() % 30) + 1) * 100;

		// �I���C�x���g���ʒm��҂�
		eRet = m_cThreadEndEvent.Wait(dwTimeout);
		switch (eRet) {
		case CEVENT_RET_WAIT_TIMEOUT:
//			pthread_mutex_lock(&(this->m_hMutex));
			printf("[%s] Count : %lu \n", this->m_strId.c_str(), dwCount++);
//			pthread_mutex_unlock(&(this->m_hMutex));
			break;

		case CEVENT_RET_RECIVE_EVENT:
			m_cThreadEndEvent.ResetEvent();
			bLoop = false;
			break;

		default:
			printf("CEvent::Wait error. \n");
			bLoop = false;
			break;
		}
	}

	printf("-- Thread %s End --\n", m_strId.c_str());
}










