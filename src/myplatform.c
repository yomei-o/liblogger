/*
Copyright (c) 2016, Yomei Otani <yomei.otani@gmai.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#include <stdio.h>
#include <stdlib.h>

#if defined(unix) || defined(__APPLE__) || defined(ANDROID)
#include <pthread.h>
#include <string.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif

#include "myplatform.h"


#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif




//
// thread
//

#if defined(unix) || defined(__APPLE__) || defined(ANDROID)
#define PTHREAD_CREATE(thread, attr, func, arg)                          \
	(int)pthread_create((pthread_t *)(thread), (pthread_attr_t *)(attr), \
    (void * (*)(void *))(func), (void *)(arg))
#else
#ifdef _WIN32
#define BEGINTHREAD(src, stack, func, arg, flag, id)         \
	(HANDLE)_beginthreadex((void *)(src), (unsigned)(stack), \
	(unsigned (_stdcall *)(void *))(func), (void *)(arg),    \
	(unsigned)(flag), (unsigned *)(id))
#endif
#endif


#ifdef _WIN32
static unsigned __stdcall internal_start_thread(void * vp)
{
	mp_thread_t t;
	if(vp==NULL)return 0;

	t=(mp_thread_t )vp;
	return (unsigned)(((void * (*)(void *))(t->func))(t->arg));
}
#endif


mp_thread_t mp_thread_create(void (*func)(void*), void* arg)
{
	mp_thread_t t;
	
	t = (mp_thread_t)malloc(sizeof(struct _thread));
	if (t == NULL)return NULL;
	memset(t,0,sizeof(struct _thread));

	t->func = func;
	t->arg = arg;

#if defined(unix) || defined(__APPLE__) || defined(ANDROID)
	if (PTHREAD_CREATE(&t->hthread, 0, func, arg))
	{
		free(t);
		return NULL;
	}
#endif
#ifdef _WIN32
	{
		int id;
		HANDLE hnd;
		/* yomei fixed: this thread function must be a standard call.  */
		hnd = BEGINTHREAD(0, 0, internal_start_thread, t, 0, &id);
		if (!hnd)
		{
			free(t);
			return NULL;
		}
		t->hthread = hnd;
	}
#endif
	return t;

}



void mp_thread_join(mp_thread_t t)
{
	if (t == NULL)return;

#if defined(unix) || defined(__APPLE__) || defined(ANDROID)
	pthread_join(t->hthread, NULL);
	free(t);
#endif
#ifdef _WIN32
	WaitForSingleObject(t->hthread, INFINITE);
	CloseHandle(t->hthread);
	free(t);
#endif
}


//
// sleep
//

void mp_sleep(unsigned int s)
{
#if defined(unix) || defined(__APPLE__) || defined(ANDROID)
	sleep(s);
#endif
#ifdef _WIN32
	Sleep(s * 1000);
#endif
}


void mp_msleep(unsigned int ms)
{
#if defined(unix) || defined(__APPLE__) || defined(ANDROID)
	sleep(ms / 1000);
	usleep((ms % 1000) * 1000);
#endif
#ifdef _WIN32
	Sleep(ms);
#endif
}


//
// mutex
//


mp_mutex_t mp_mutex_create(void)
{
#if defined(unix) || defined(__APPLE__)
	pthread_mutex_t* m;

	m = malloc(sizeof(pthread_mutex_t));
	if (m == NULL)return NULL;

	//*m=PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_init(m, NULL);
	return (void*)m;
#endif
#ifdef _WIN32
	LPCRITICAL_SECTION m;

	m = malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(m);
	return (void*)m;
#endif
	return NULL;
}
int mp_mutex_destroy(mp_mutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if (pm == NULL)return 0;
	pthread_mutex_destroy(pm);
	free(pm);
	return 1;
#endif
#ifdef _WIN32
	if (pm == NULL)return 0;
	DeleteCriticalSection(pm);
	free(pm);
	return 1;
#endif
	return 0;
}
int mp_mutex_lock(mp_mutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if (pm == NULL)return 0;
	pthread_mutex_lock(pm);
	return 1;
#endif
#ifdef _WIN32	
	if (pm == NULL)return 0;
	EnterCriticalSection(pm);
	return 1;
#endif
	return 0;
}
int mp_mutex_unlock(mp_mutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if (pm == NULL)return 0;
	pthread_mutex_unlock(pm);
	return 1;
#endif
#ifdef _WIN32	
	if (pm == NULL)return 0;
	LeaveCriticalSection(pm);
	return 1;
#endif
	return 0;
}
int mp_mutex_trylock(mp_mutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	int res;
	res = pthread_mutex_trylock(pm);
	return !res;
#endif
#ifdef _WIN32	
	int res;
	res = TryEnterCriticalSection(pm);
	return res;
#endif
	return 0;
}



//
// Condition
//

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


mp_cond_t mp_cond_init(void)
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

int mp_cond_destroy(mp_cond_t pc)
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

int mp_cond_wait(mp_cond_t pc,mp_mutex_t pm)
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
	
	mp_mutex_unlock(pm);
	WaitForSingleObject (cv->event_, INFINITE);
	SetEvent(cv->event2_);
	mp_mutex_lock(pm);
	EnterCriticalSection (&cv->waiters_count_lock_);
	cv->waiters_count_--;
	cv->release_count_--;
	LeaveCriticalSection (&cv->waiters_count_lock_);

	
	return 1;
#endif
	return 0;
}

int mp_cond_signal(mp_cond_t pc)
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

int mp_cond_broadcast(mp_cond_t pc)
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









//
// test
//


#if 0


static mp_mutex_t pm;
static mp_cond_t pc;
static int test = 0;

void f(void* vp)
{
	while (test == 0)
	{
		printf("f()[%d]\n", (int)vp);
		mp_mutex_lock(pm);
		mp_cond_wait(pc, pm);
		mp_mutex_unlock(pm);
		printf("f()[%d]   done\n", (int)vp);
	}
}
main()
{
	mp_thread_t th[10];
	int i;

	pc = mp_cond_init();
	pm = mp_mutex_create();

	for (i = 0; i<10; i++)
	{
		th[i] = mp_thread_create(f, (void*)i);
	}
	printf("sleep\n");
	mp_sleep(5);

	printf("signal 3 times\n");
	mp_mutex_lock(pm);
	mp_cond_signal(pc);
	mp_cond_signal(pc);
	mp_cond_signal(pc);
	mp_mutex_unlock(pm);
	mp_sleep(5);

	printf("broadcast\n");
	mp_mutex_lock(pm);
	mp_cond_broadcast(pc);
	mp_sleep(5);
	printf("broadcast locking\n");
	mp_cond_broadcast(pc);
	mp_mutex_unlock(pm);
	mp_sleep(2);

	test = 1;
	printf("broadcast non-lock\n");
	mp_cond_broadcast(pc);
	mp_mutex_unlock(pm);

	for (i = 0; i<10; i++)
	{
		mp_thread_join(th[i]);
	}
	mp_mutex_destroy(pm);
	mp_cond_destroy(pc);
}


#endif

