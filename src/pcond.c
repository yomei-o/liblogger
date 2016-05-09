/******* BEGIN LICENCE BLOCK *******

    Sony Digital Photo Frame Application
    Copyright (C) 2008 Sony Corporation.

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License,version 2, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.

    Linking Sony Digital Photo Frame Application statically or dynamically with other modules is making a combined work based on Sony Digital Photo Frame Application. Thus, the terms and conditions of the GNU General Public License cover the whole combination.

    In addition, as a special exception, the copyright holders of Sony Digital Photo Frame Application give you permission to combine Sony Digital Photo Frame Application with free software programs or libraries that are released under the GNU LGPL and with code included in the standard release of Sony OS Abstraction Library (or modified versions of such code, with unchanged license). You may copy and distribute such a system following the terms of the GNU GPL for Sony Digital Photo Frame Application and the licenses of the other code concerned{, provided that you include the source code of that other code when and as the GNU GPL requires distribution of source code}.

    Note that people who make modified versions of Sony Digital Photo Frame Application are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU General Public License gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.

******* END LICENCE BLOCK *******/

#define __PCOND_C__VER "20130627"

#ifdef _WIN32
#define _WIN32_WINNT 0x0400
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(unix) || defined(__APPLE__)
#include <unistd.h>
#include <pthread.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif


#include "pcond.h"


#ifdef _WIN32


typedef struct
{
  int waiters_count_;
  CRITICAL_SECTION waiters_count_lock_;
  int release_count_;  
  //int wait_generation_count_;
  HANDLE event_;
  HANDLE event2_;
} pthread_cond_t;

#endif



void pcond_sleep(int s)
{
#if defined(unix) || defined(__APPLE__)
	sleep(s);
#endif
#ifdef _WIN32
	Sleep(s*1000);
#endif
}



pcond_t pcond_init(void)
{
#if defined(unix) || defined(__APPLE__)
	pthread_cond_t* p;
	
	p=malloc(sizeof(pthread_cond_t));
	if(p==NULL)return NULL;
	
	//*p=PTHREAD_COND_INITIALIZER;
	
	pthread_cond_init(p,NULL);
	
	return (void*)p;	
#endif
#ifdef _WIN32
	pthread_cond_t* cv;
	
	cv=malloc(sizeof(pthread_cond_t));
	if(cv==NULL)return NULL;

	memset(cv,0,sizeof(pthread_cond_t));
	
	cv->waiters_count_ = 0;
	cv->release_count_ = 0;
	//cv->wait_generation_count_=0;
	
	InitializeCriticalSection (&cv->waiters_count_lock_);
	cv->event_ = CreateEvent (NULL,  // no security
		FALSE,  // manual-reset
		FALSE, // non-signaled initially
		NULL); // unnamed
	cv->event2_ = CreateEvent (NULL,  // no security
		FALSE,  // manual-reset
		TRUE, // signaled initially
		NULL); // unnamed
	return (void*)cv;
#endif
	return NULL;
}

int pcond_destroy(pcond_t pc)
{
#if defined(unix) || defined(__APPLE__)
	if(pc==NULL)return 0;
	pthread_cond_destroy((void*)pc);
	free(pc);
	
	return 1;
#endif
#ifdef _WIN32
	pthread_cond_t* cv;
	cv=(pthread_cond_t*)pc;
	if(pc==NULL)return 0;
	DeleteCriticalSection(&cv->waiters_count_lock_);
	CloseHandle(cv->event_);
	free(pc);
	return 1;
#endif
	return 0;
}

int pcond_wait(pcond_t pc,pmutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if(pc==NULL || pm==NULL)return 0;
	pthread_cond_wait((void*)pc,(void*)pm);
	return 1;
#endif
#ifdef _WIN32
	pthread_cond_t* cv;
	cv=(pthread_cond_t*)pc;
	if(pc==NULL || pm==NULL)return 0;


	EnterCriticalSection (&cv->waiters_count_lock_);
	cv->waiters_count_++;
	LeaveCriticalSection (&cv->waiters_count_lock_);
	
	pmutex_unlock(pm);
	WaitForSingleObject (cv->event_, INFINITE);
	SetEvent(cv->event2_);
	pmutex_lock(pm);
	EnterCriticalSection (&cv->waiters_count_lock_);
	cv->waiters_count_--;
	cv->release_count_--;
	LeaveCriticalSection (&cv->waiters_count_lock_);

	
	return 1;
#endif
	return 0;
}

int pcond_signal(pmutex_t pc)
{
#if defined(unix) || defined(__APPLE__)
	if(pc==NULL)return 0;
	pthread_cond_signal((void*)pc);
	return 1;
#endif
#ifdef _WIN32
	pthread_cond_t* cv;
	cv=(pthread_cond_t*)pc;
	if(pc==NULL)return 0;

	EnterCriticalSection (&cv->waiters_count_lock_);
	if (cv->waiters_count_ > cv->release_count_) {
		WaitForSingleObject(cv->event2_,-1);
		SetEvent (cv->event_);
		cv->release_count_++;
	}
	LeaveCriticalSection (&cv->waiters_count_lock_);
#endif
	return 0;
}

int pcond_broadcast(pmutex_t pc)
{
#if defined(unix) || defined(__APPLE__)
	if(pc==NULL)return 0;
	pthread_cond_broadcast((void*)pc);
	return 1;
#endif
#ifdef _WIN32
	pthread_cond_t* cv;
	cv=(pthread_cond_t*)pc;
	if(pc==NULL)return 0;

	EnterCriticalSection (&cv->waiters_count_lock_);
	while (cv->waiters_count_ > cv->release_count_) {  
		WaitForSingleObject(cv->event2_,-1);
		SetEvent (cv->event_);
		cv->release_count_ ++;
	}
	LeaveCriticalSection (&cv->waiters_count_lock_);
	return 1;
#endif
	return 0;
}




#if 0


#include "thread.h"

static pmutex_t pm;
static pcond_t pc;
static int test=0;
void f(void* vp)
{
	while(test==0)
	{
	printf("f()[%d]\n",(int)vp);
	pmutex_lock(pm);
	pcond_wait(pc,pm);
	pmutex_unlock(pm);
	printf("f()[%d]   done\n",(int)vp);
	}
}
main()
{
	thread_t* th[10];
	int i;

	pc=pcond_init();
	pm=pmutex_create();

	for(i=0;i<10;i++)
	{
		th[i]=thread_create(f,(void*)i);
	}
	printf("sleep\n");
	pmutex_sleep(5);
	printf("unlocked\n");

	pmutex_lock(pm);
	pcond_signal(pc);
	pcond_signal(pc);
	pcond_signal(pc);
	pmutex_unlock(pm);

	pmutex_sleep(5);
	printf("unlocked\n");

	pmutex_lock(pm);
	pcond_broadcast(pc);
	pmutex_sleep(5);
	pcond_broadcast(pc);
	pmutex_unlock(pm);

	pmutex_sleep(2);
		
	test=1;
	pcond_broadcast(pc);
	pmutex_unlock(pm);
	
	for(i=0;i<10;i++)
	{
		thread_join(th[i]);
	}
	pmutex_destroy(pm);
	pcond_destroy(pc);
}

#endif

