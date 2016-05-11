/******* BEGIN LICENCE BLOCK *******

    Sony Digital Photo Frame Application
    Copyright (C) 2013 Sony Corporation.

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License,version 2, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.

    Linking Sony Digital Photo Frame Application statically or dynamically with other modules is making a combined work based on Sony Digital Photo Frame Application. Thus, the terms and conditions of the GNU General Public License cover the whole combination.

    In addition, as a special exception, the copyright holders of Sony Digital Photo Frame Application give you permission to combine Sony Digital Photo Frame Application with free software programs or libraries that are released under the GNU LGPL and with code included in the standard release of Sony OS Abstraction Library (or modified versions of such code, with unchanged license). You may copy and distribute such a system following the terms of the GNU GPL for Sony Digital Photo Frame Application and the licenses of the other code concerned{, provided that you include the source code of that other code when and as the GNU GPL requires distribution of source code}.

    Note that people who make modified versions of Sony Digital Photo Frame Application are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU General Public License gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.

******* END LICENCE BLOCK *******/



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

