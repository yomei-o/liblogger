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

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif


#include <stdio.h>
#ifdef WIN32			/* For Windows */
#include <windows.h>
#endif /* WIN32 */
#if defined(unix) || defined(__APPLE__)
#include <dlfcn.h>
#endif /* WIN32 */

#include "dll_client.h"


//#define ERR_PRINT

#ifdef ERR_PRINT
 #define EPRINT(a) fprintf(stderr,a)
 #define EPRINT2(a,b) fprintf(stderr,a,b)
#else
 #define EPRINT(a) do; while(0)
 #define EPRINT2(a,b) do; while(0)
#endif


/*----------------------------------------------------------*/
/*						Load DLL							*/
/*----------------------------------------------------------*/
void *dll_load(char *dllname)
{
	void *pFunclib=NULL;

	if(dllname==NULL) {
		EPRINT("### dllname is NULL\n");
		return NULL;
	}
#ifdef _WIN32
	pFunclib = LoadLibrary(dllname);
#endif

#if defined(unix) || defined(__APPLE__)
	pFunclib = dlopen(dllname, RTLD_LAZY);
#endif
	if(!pFunclib) {
		EPRINT2("### cannot dll_load >%s<\n",dllname);
	}
	return pFunclib;
}

/*----------------------------------------------------------*/
/*						Access DLL							*/
/*----------------------------------------------------------*/
void *dll_access(void *pDllmod, char *funcname)
{
	void *pDll_func=NULL;

	if(pDllmod==NULL) {
		EPRINT("### dll_access handle is NULL\n");
		return NULL;
	}
	if(funcname==NULL) {
		EPRINT("### dll_access funcname is NULL\n");
		return NULL;
	}
#ifdef _WIN32
	pDll_func = GetProcAddress((HMODULE)pDllmod, funcname); 
#endif

#if defined(unix) || defined(__APPLE__)
	pDll_func = dlsym(pDllmod, funcname);
#endif
	if(!pDll_func) {
		EPRINT2("### cannot dll_access >%s<\n",funcname);
	}
	return pDll_func;
}

/*----------------------------------------------------------*/
/*						Close DLL							*/
/*----------------------------------------------------------*/
void dll_close(void *pDllmod)
{
	if(pDllmod==NULL)return;
#ifdef _WIN32
	FreeLibrary(pDllmod);
#endif

#if defined(unix) || defined(__APPLE__)
	dlclose(pDllmod);
#endif
	return;
}


