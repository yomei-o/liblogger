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

#define __MYTHREAD_C__VER "20130626"


#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif


#include <stdio.h>
#include <stdlib.h>
#include "mythread.h"



#ifdef HAVE_PTHREAD_H
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



/* yomei fixed: translate __stdcall to __cdecl  */
#ifdef _WIN32
static unsigned __stdcall internal_start_thread( void * vp)
{
	mythread_t *t;
	if(vp==NULL)return 0;

	t=(mythread_t *)vp;
	return (unsigned)(((void * (*)(void *))(t->func))(t->arg));
}
#endif


mythread_t*
mythread_create(void* func, void* arg)
{
	mythread_t *t;
	
	t = (mythread_t *)malloc(sizeof(mythread_t));
	if (!t)
	{
		goto fail0;
	}

	t->func = func;
	t->arg = arg;

#ifdef HAVE_PTHREAD_H
	if (PTHREAD_CREATE(&t->hthread, 0, func, arg))
	{
		goto fail1;
	}
#else
#ifdef _WIN32
	{
		int id;
		HANDLE hnd;
		/* yomei fixed: this thread function must be a standard call.  */
		hnd = BEGINTHREAD(0, 0, internal_start_thread, t, 0, &id);
		if (!hnd)
		{
			goto fail1;
		}
		t->hthread = hnd;
	}
#endif
#endif
	return t;

fail1:
	if (t)
	{
		free(t);
		t = NULL;
	}
fail0:
	return t;
}



void
mythread_join(mythread_t* t)
{
	if (!t)
	{
		goto fail;
	}
#ifdef HAVE_PTHREAD_H
	pthread_join(t->hthread, NULL);
	free(t);
#else
#ifdef _WIN32
	WaitForSingleObject(t->hthread, INFINITE);
	CloseHandle(t->hthread);
	free(t);
#endif
#endif

fail:
	return;
}




