/*
 * Default entry point for a 16-bit exe
 *
 * Copyright 2009 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>
#include "windef.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/winbase16.h"
#define DECLSPEC_HIDDEN
#include "../krnl386/kernel16_private.h"

#ifdef __i386__
extern WORD WINAPI WinMain16( HINSTANCE16 inst, HINSTANCE16 prev, LPSTR cmdline, WORD show );

void WINAPI DECLSPEC_HIDDEN __wine_spec_exe16_entry( CONTEXT *context )
{
	HMODULE krnl386 = LoadLibraryA("krnl386.exe16");
    PDB16 *psp;
    WORD len;
    LPSTR cmdline;

    /*InitTask16*/((void(WINAPI*)(CONTEXT*))GetProcAddress(krnl386, "InitTask16"))( context );

    psp = /*MapSL*/((LPVOID(WINAPI*)(DWORD))GetProcAddress(krnl386, "MapSL"))( WOWGlobalLock16( context->SegEs ));
    len = psp->cmdLine[0];
    cmdline = HeapAlloc( GetProcessHeap(), 0, len + 1 );
    memcpy( cmdline, psp->cmdLine + 1, len );
    cmdline[len] = 0;

    SetEvent(kernel_get_thread_data()->idle_event);
    ExitThread( WinMain16( context->Edi, context->Esi, cmdline, context->Edx ));
}

#endif  /* __i386__ */
