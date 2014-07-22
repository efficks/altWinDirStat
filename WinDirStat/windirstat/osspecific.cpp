// osspecific.cpp	- Implementation of the platform-specific classes
//
// WinDirStat - Directory Statistics
// Copyright (C) 2003-2004 Bernhard Seifert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: bseifert@users.sourceforge.net, bseifert@daccord.net
//
// Last modified: $Date$

#include "stdafx.h"
#include "osspecific.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define TSPEC "W"
#else
#define TSPEC "A"
#endif

#define GETPROC(name) m_##name = ( m_dll != 0 ? (Type##name)GetProcAddress(m_dll, #name) : NULL )
#define TGETPROC(name) m_##name = ( m_dll != 0 ? (Type##name)GetProcAddress(m_dll, #name TSPEC) : NULL )

#define CHECK(name) if (m_##name == 0) return false

/////////////////////////////////////////////////////////////////////////////

CVolumeApi::CVolumeApi( ) : m_UnloadDll( false ) {
	m_dll = GetModuleHandle( _T( "kernel32.dll" ) );
	if ( !m_dll ) {
		m_dll = LoadLibrary( _T( "kernel32.dll" ) );
		m_UnloadDll = ( m_dll != NULL );
		}

	TGETPROC( GetVolumeNameForVolumeMountPoint );
	TGETPROC( FindFirstVolume );
	TGETPROC( FindNextVolume );
	GETPROC( FindVolumeClose );
	TGETPROC( FindFirstVolumeMountPoint );
	TGETPROC( FindNextVolumeMountPoint );
	GETPROC( FindVolumeMountPointClose );
	}

CVolumeApi::~CVolumeApi( ) {
	if ( m_UnloadDll ) {
		FreeLibrary( m_dll );
		}
	// "It is not safe to call FreeLibrary from DllMain."
	// Therefore, don't use global variables of type CVolumeApi in a DLL.
	}

bool CVolumeApi::IsSupported( ) const {
	CHECK( GetVolumeNameForVolumeMountPoint );
	CHECK( FindFirstVolume );
	CHECK( FindNextVolume );
	CHECK( FindVolumeClose );
	CHECK( FindFirstVolumeMountPoint );
	CHECK( FindNextVolumeMountPoint );
	CHECK( FindVolumeMountPointClose );

	return true;
	}


BOOL CVolumeApi::GetVolumeNameForVolumeMountPoint( _Inout_ LPCTSTR lpszVolumeMountPoint, _Inout_ LPTSTR lpszVolumeName, _In_ DWORD cchBufferLength ) {
	ASSERT( IsSupported( ) );
	return ( *m_GetVolumeNameForVolumeMountPoint )( lpszVolumeMountPoint, lpszVolumeName, cchBufferLength );
	}

HANDLE CVolumeApi::FindFirstVolume( _In_ LPTSTR lpszVolumeName, _In_ DWORD cchBufferLength ) {
	ASSERT( IsSupported( ) );
	return ( *m_FindFirstVolume )( lpszVolumeName, cchBufferLength );
	}

BOOL CVolumeApi::FindNextVolume( _Inout_ HANDLE hFindVolume, _Inout_ LPTSTR lpszVolumeName, _In_ DWORD cchBufferLength ) {
	ASSERT( IsSupported( ) );
	return ( *m_FindNextVolume )( hFindVolume, lpszVolumeName, cchBufferLength );
	}

BOOL CVolumeApi::FindVolumeClose( _Inout_ HANDLE hFindVolume ) {
	ASSERT( IsSupported( ) );
	return ( *m_FindVolumeClose )( hFindVolume );
	}


HANDLE CVolumeApi::FindFirstVolumeMountPoint( _In_ LPCTSTR lpszRootPathName, _In_ LPTSTR lpszVolumeMountPoint, _In_ DWORD cchBufferLength ) {
	ASSERT( IsSupported( ) );
	return ( *m_FindFirstVolumeMountPoint )( lpszRootPathName, lpszVolumeMountPoint, cchBufferLength );
	}

BOOL CVolumeApi::FindNextVolumeMountPoint( _Inout_ HANDLE hFindVolumeMountPoint, _Inout_ LPTSTR lpszVolumeMountPoint, _In_ DWORD cchBufferLength ) {
	ASSERT( IsSupported( ) );
	return ( *m_FindNextVolumeMountPoint )( hFindVolumeMountPoint, lpszVolumeMountPoint, cchBufferLength );
	}

BOOL CVolumeApi::FindVolumeMountPointClose( _Inout_ HANDLE hFindVolumeMountPoint ) {
	ASSERT( IsSupported( ) );
	return ( *m_FindVolumeMountPointClose )( hFindVolumeMountPoint );
	}


//void MemoryUsage::UpdateInfo( ) {
//	auto pmc = zeroInitPROCESS_MEMORY_COUNTERS( );
//	if ( GetProcessMemoryInfo( GetCurrentProcess( ), &pmc, sizeof( pmc ) ) ) {
//		TRACE( _T( "%lu\r\n" ), pmc.WorkingSetSize );
//		m_workingSet = pmc.WorkingSetSize;
//		}
//	}


CPsapi::CPsapi( ) {
	//m_dll = LoadLibrary( _T( "psapi.dll" ) );

	GETPROC( GetProcessMemoryInfo );
	}

CPsapi::~CPsapi( ) {
	//if ( m_dll != NULL ) {
	//	FreeLibrary( m_dll );
	//	}
	}

bool CPsapi::IsSupported( ) const {
	CHECK( GetProcessMemoryInfo );
	return true;
	}

BOOL CPsapi::GetProcessMemoryInfo( _In_ HANDLE Process, _Inout_ PPROCESS_MEMORY_COUNTERS ppsmemCounters, _In_ DWORD cb ) {
	ASSERT( IsSupported( ) );
	return ( *m_GetProcessMemoryInfo )( Process, ppsmemCounters, cb );
	}



CQueryDosDeviceApi::CQueryDosDeviceApi( ) : m_UnloadDll( false ) {
	//m_dll = GetModuleHandle( _T( "kernel32.dll" ) );
	//if ( !m_dll ) {
	//	m_dll = LoadLibrary( _T( "kernel32.dll" ) );
	//	m_UnloadDll = ( m_dll != NULL );
	//	}

	TGETPROC( QueryDosDevice );
	}

CQueryDosDeviceApi::~CQueryDosDeviceApi( ) {
	if ( m_UnloadDll ) {
		FreeLibrary( m_dll );
		}
	}

bool CQueryDosDeviceApi::IsSupported( ) const {
	CHECK( QueryDosDevice );
	return true;
	}

DWORD CQueryDosDeviceApi::QueryDosDevice( _Inout_ LPCTSTR lpDeviceName, _Inout_ LPTSTR lpTargetPath, _In_ DWORD ucchMax ) {
	ASSERT( IsSupported( ) );
	return ( *m_QueryDosDevice )( lpDeviceName, lpTargetPath, ucchMax );
	}

/////////////////////////////////////////////////////////////////////////////
//
//CGetCompressedFileSizeApi::CGetCompressedFileSizeApi( ) : m_UnloadDll( false ) {
//	ASSERT( false );
//	m_dll = GetModuleHandle( _T( "kernel32.dll" ) );
//	if ( !m_dll ) {
//		m_dll = LoadLibrary( _T( "kernel32.dll" ) );
//		m_UnloadDll = ( m_dll != NULL );
//		}
//
//	TGETPROC( GetCompressedFileSize );
//	}
//
//CGetCompressedFileSizeApi::~CGetCompressedFileSizeApi( ) {
//	if ( m_UnloadDll ) {
//		FreeLibrary( m_dll );
//		}
//	}
//
//bool CGetCompressedFileSizeApi::IsSupported( ) const {
//	CHECK( GetCompressedFileSize );
//	return true;
//	}
//
//DWORD CGetCompressedFileSizeApi::GetCompressedFileSize( _In_ LPCTSTR lpFileName, _Inout_ LPDWORD lpFileSizeHigh ) {
//	ASSERT( IsSupported( ) );
//	return ( *m_GetCompressedFileSize )( lpFileName, lpFileSizeHigh );
//	}
//
//ULONGLONG CGetCompressedFileSizeApi::GetCompressedFileSize( _In_ LPCTSTR lpFileName ) {
//	ASSERT( IsSupported( ) );
//	ULARGE_INTEGER ret;
//	ret.LowPart = ( *m_GetCompressedFileSize )( lpFileName, &ret.HighPart );
//	return ret.QuadPart;
//	}

// $Log$
// Revision 1.7  2005/04/17 12:27:21  assarbad
// - For details see changelog of 2005-04-17
//
// Revision 1.6  2004/11/28 14:40:06  assarbad
// - Extended CFileFindWDS to replace a global function
// - Now packing/unpacking the file attributes. This even spares a call to find encrypted/compressed files.
//
// Revision 1.5  2004/11/07 20:14:31  assarbad
// - Added wrapper for GetCompressedFileSize() so that by default the compressed file size will be shown.
//
// Revision 1.4  2004/11/05 16:53:07  assarbad
// Added Date and History tag where appropriate.
//
