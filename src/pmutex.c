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

#define __PMUTEX_C__VER "20130626"


#ifdef _WIN32
#define _WIN32_WINNT 0x0400
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pmutex.h"
#if defined(unix) || defined(__APPLE__)
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

void pmutex_sleep(int s)
{
#if defined(unix) || defined(__APPLE__)
	sleep(s);
#endif
#ifdef _WIN32
	Sleep(s*1000);
#endif
}

pmutex_t pmutex_create(void)
{
#if defined(unix) || defined(__APPLE__)
	pthread_mutex_t* m;
	
	m=malloc(sizeof(pthread_mutex_t));
	if(m==NULL)return NULL;

	//*m=PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_init(m,NULL);
	return (void*)m;
#endif
#ifdef _WIN32
	LPCRITICAL_SECTION m;
	
	m=malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(m);
	return (void*)m;	
#endif
	return NULL;	
}
int pmutex_destroy(pmutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if(pm==NULL)return 0;
	pthread_mutex_destroy(pm);
	free(pm);
	return 1;
#endif
#ifdef _WIN32
	if(pm==NULL)return 0;
	DeleteCriticalSection(pm);
	free(pm);
	return 1;
#endif
	return 0;
}
int pmutex_lock(pmutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if(pm==NULL)return 0;
	pthread_mutex_lock(pm);
	return 1;
#endif
#ifdef _WIN32	
	if(pm==NULL)return 0;
	EnterCriticalSection(pm);
	return 1;
#endif
	return 0;
}
int pmutex_unlock(pmutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
	if(pm==NULL)return 0;
	pthread_mutex_unlock(pm);
	return 1;
#endif
#ifdef _WIN32	
	if(pm==NULL)return 0;
	LeaveCriticalSection(pm);
	return 1;
#endif
	return 0;
}
int pmutex_trylock(pmutex_t pm)
{
#if defined(unix) || defined(__APPLE__)
    int res;
    res=pthread_mutex_trylock(pm);
    return !res;
#endif
#ifdef _WIN32	
    int res;
	res=TryEnterCriticalSection(pm);
	return res;
#endif
	return 0;
}

#if 0

#include "thread.h"

static pmutex_t pm;

void f(void* vp)
{
	printf("f()\n");
	pmutex_lock(pm);
	printf("f() locked\n");
	pmutex_sleep(5);
	printf("f() unlocked\n");
	pmutex_unlock(pm);	
	printf("f() exit\n");
}
main()
{
	thread_t* th;
	pm=pmutex_create();
	pmutex_lock(pm);
	printf("locked\n");
	th=thread_create(f,NULL);
	pmutex_sleep(5);
	printf("unlocked\n");
	pmutex_unlock(pm);
	thread_join(th);
	pmutex_destroy(pm);
	printf("exit\n");
}

#endif

#if 0


#include "thread.h"

static pmutex_t pm;

void f(void* vp)
{
	int res;
	printf("f()\n");
	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	printf("f() exit\n");
}
main()
{
	thread_t* th;
	pm=pmutex_create();
	pmutex_lock(pm);
	printf("locked\n");
	th=thread_create(f,NULL);
	pmutex_sleep(5);
	printf("unlocked\n");
	pmutex_unlock(pm);
	thread_join(th);
	pmutex_destroy(pm);
	printf("exit\n");
}

#endif

