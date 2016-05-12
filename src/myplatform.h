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



#ifndef __MYPLATHOME_H__
#define __MYPLATHOME_H__




#ifdef __cplusplus
extern "C"
{
#endif



//
// Thread
//

	struct _thread
{
	void* hthread;
	void* func;
	void* arg;
};

typedef struct _thread* mp_thread_t;
	
	
mp_thread_t mp_thread_create(void(*func)(void*), void *arg);
void mp_thread_join(mp_thread_t t);

//
// Sleep
//

void mp_sleep(unsigned int s);
void mp_msleep(unsigned int ms);


//
// mutex
//

typedef void* mp_mutex_t;

mp_mutex_t m_mutex_create(void);
int mp_mutex_destroy(mp_mutex_t pm);
int mp_mutex_lock(mp_mutex_t pm);
int m_pmutex_unlock(mp_mutex_t pm);
int m_pmutex_trylock(mp_mutex_t pm);


//
// cond
//

typedef void* mp_cond_t;

mp_cond_t mp_cond_init(void);
int mp_cond_destroy(mp_cond_t pc);

int mp_cond_wait(mp_cond_t pc, mp_mutex_t pm);
int mp_cond_signal(mp_cond_t pc);
int mp_cond_broadcast(mp_cond_t pc);



#ifdef __cplusplus
}
#endif

#endif /* __MYTHREAD_H__ */

