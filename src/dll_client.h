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


 
#ifndef _DLL_CLIENT_H_
#define _DLL_CLIENT_H_

#define _DLL_CLIENT_H_VER "20130701"


#define LIB_OK                0

#define LIB_ERR_INVALID_REQUEST     (-1)
#define LIB_ERR_INVALID_PARAMETER   (-2)
#define LIB_ERR_NOSERVICE           (-3)
#define LIB_ERR_NOREQUEST           (-4)


#if defined(_WIN32) && !defined(__GNUC__)
#define LIBEXT ".dll"
#else
#define LIBEXT ".so"
#endif

#define DLL_NAME(a) a LIBEXT


#ifdef __cplusplus
extern "C"
{
#endif


void *dll_load(char *dllname);
void *dll_access(void *pDllmod, char *funcname);
void dll_close(void *pDllmod);


#ifdef __cplusplus
}
#endif



#endif


