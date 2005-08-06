//
// This file is part of the aMule Project.
//
// Copyright (c) 2003-2005 aMule Team ( admin@amule.org / http://www.amule.org )
// Copyright (c) 2002 Merkur ( devs@emule-project.net / http://www.emule-project.net )
//
// Any parts of this program derived from the xMule, lMule or eMule project,
// or contributed by third-party developers are copyrighted by their
// respective authors.
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA, 02111-1307, USA
//

#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "OtherFunctions.h"
#endif

#include <wx/defs.h>		// Needed before any other wx/*.h
#include <wx/string.h>		// Needed for wxString
#include <wx/utils.h>		// Needed for wxGetHomeDir()
#include <wx/filename.h>	// Needed for wxFileName::GetPathSeparator()
#include <wx/intl.h>		// Needed for wxLANGUAGE_ constants
#include <wx/thread.h>

#ifdef __WXBASE__
	#include <time.h>
	#include <errno.h>
#else
	#include <wx/utils.h>
#endif	

#include "Types.h"		// Needed for uint16, uint32 and uint64
#include "ArchSpecific.h" // Needed for Peek/Poke functions

namespace otherfunctions {
	
/**
 * Helper function.
 *
 * @param ArgA The base value.
 * @param ArgB The value to compare ArgA against.
 * @return See below.
 *
 * Use this function to safely compare two arguments of a type that supports 
 * the "<" operator. It works like strcmp and returns a negative value if ArgA
 * is less than ArgB, zero if ArgA is equal to ArgB and a positive value if 
 * ArgA is greater than ArgB.
 */
template <class TYPE>
int CmpAny(const TYPE& ArgA, const TYPE& ArgB)
{
	if ( ArgA < ArgB ) {
		return -1;
	} else if ( ArgB < ArgA ) {
		return  1;
	} else {
		return  0;
	}
}

//! Overloaded version of CmpAny for use with wxStrings.
inline int CmpAny(const wxString& ArgA, const wxString& ArgB)
{
	return ArgA.CmpNoCase( ArgB );
}

//! Overloaded version of CmpAny for use with C-Strings (Unicoded).
inline int CmpAny(const wxChar* ArgA, const wxChar* ArgB)
{
	return wxString( ArgA ).CmpNoCase( ArgB );
}


/**
 * Removes the first instance of a value from a STL-like list: list, vector or deque.
 *
 * @param list The list to manipulate.
 * @param item The value to search for and remove.
 * @return The number of instances removed.
 */
template <typename LIST, typename ITEM>
unsigned int EraseFirstValue( LIST& list, const ITEM& item )
{
	typename LIST::iterator it = list.begin();

	for (; it != list.end(); ++it) {
		if (*it == item) {
			list.erase(it);
			
			return true;
		}
	}

	return false;
}


/**
 * Removes all instances of a value from a STL-like list: list, vector or deque.
 *
 * @param list The list to manipulate.
 * @param item The value to search for and remove.
 * @return The number of instances removed.
 */
template <typename LIST, typename ITEM>
unsigned int EraseValue( LIST& list, const ITEM& item )
{
	typename LIST::iterator it = list.begin();
	unsigned int count = 0;

	for ( ; it != list.end(); ) {
		if ( *it == item ) {
			it = list.erase( it );
			count++;
		} else {
			++it;
		}
	}

	return count;
}


/**
 * Returns a description of the version of aMule being used.
 *
 * @return A detailed description of the aMule version, including wx information.
 *
 * Use this rather than just using the VERSION or CURRENT_VERSION_LONG
 * constants, when displaying information to the user. The purpose is to
 * help with debugging.
 */
wxString GetMuleVersion();

/**
 * This functions is like the GetMuleVersion function above, with the exception
 * that it also includes the name of the application. This can be one of the
 * following:
 *
 *  - aMule
 *  - aMuled
 *  - Remote aMule-GUI
 */
wxString GetFullMuleVersion();


/**
 * Helperfunction for accessing a child of the calling widget.
 *
 * @param IdOrName The ID or the Name of the widget to find.
 * @param type The widget-type to cast the found widget to.
 *
 * Use this function as a replacement for the following constructs:
 *  - wxStaticCast( FindWindow( <IdOrName> ), <type> )
 *  - (<type>*)FindWindow( <IdOrName> )
 *
 * It has the advantage of validating the cast in debug builds and being much
 * shorter than than manually typing wxStaticCast + FindWindow. This mean that
 * we will be alerted in case of widget changing type, instead of getting just 
 * getting bad mojo due to casting a pointer to the wrong type.
 */
#define CastChild( IdOrName, type )			dynamic_cast<type*>( FindWindow( IdOrName ) )


/**
 * Helperfunction for accessing the child of a any widget by ID.
 *
 * @param ID The ID of the widget to find.
 * @param parent The parent of the widget to find, or NULL to search from the top.
 * @param type The type to cast the widget to.
 *
 * @see CastChild()
 */
#define CastByID( ID, parent, type )		dynamic_cast<type*>( wxWindow::FindWindowById( (ID), (parent) ) )


/**
 * Helperfunction for accessing the child of a any widget by Name.
 *
 * @param Name The Name of the widget to find.
 * @param parent The parent of the widget to find, or NULL to search from the top.
 * @param type The type to cast the widget to.
 *
 * @see CastChild()
 */
#define CastByName( Name, parent, type )	dynamic_cast<type*>( wxWindow::FindWindowByName( (Name), (parent) ) )


// From Gnucleus project [found by Tarod]
// Base16/Base32/Base64 Encode/Decode functions
wxString EncodeBase16(const unsigned char* buffer, unsigned int bufLen);
unsigned int DecodeBase16(const wxString &base16Buffer, unsigned int base16BufLen, unsigned char *buffer);
wxString EncodeBase32(const unsigned char* buffer, unsigned int bufLen);
unsigned int DecodeBase32(const wxString &base32Buffer, unsigned int base32BufLen, unsigned char *buffer);
wxString EncodeBase64(const char* buffer, unsigned int bufLen);
unsigned int DecodeBase64(const wxString &base64Buffer, unsigned int base64BufLen, unsigned char *buffer);

// Converts the number of bytes to human readable form.
wxString CastItoXBytes(uint64 count);
// Converts the number to human readable form, abbreviating when nessecary.
wxString CastItoIShort(uint64 number);
// Converts a number of bytes to a human readable speed value.
wxString CastItoSpeed(uint32 bytes);
// Converts an amount of seconds to human readable time.
wxString CastSecondsToHM(sint32 seconds);
// Returns the amount of Bytes the provided size-type represents
uint32 GetTypeSize(uint8 type);
// Returns the string associated with a file-rating value.
wxString GetRateString(uint16 rate);


// The following functions are used to identify and/or name the type of a file
enum FileType { ftAny, ftVideo, ftAudio, ftArchive, ftCDImage, ftPicture, ftText, ftProgram };
// Examins a filename and returns the enumerated value assosiated with it, or ftAny if unknown extension
FileType GetFiletype(const wxString& filename);
// Returns the description of a filetype: Movies, Audio, Pictures and so on...
wxString GetFiletypeDesc(FileType type, bool translated = true);
// Shorthand for GetFiletypeDesc(GetFiletype(filename))
wxString GetFiletypeByName(const wxString& filename, bool translated = true);
// Reports if the file has contents or not (no need for the file to exist)
bool IsEmptyFile(const wxString& filename);


// Returns the max number of connections the current OS can handle.
// Currently anything but windows will return the default value (-1);
int GetMaxConnections();
// Returns the name assosiated with a category value.
wxString GetCatTitle(int catid);

/* Other */

// Compares first and second. For uint16 arrays sorting.
int wxCMPFUNC_CONV Uint16CompareValues(uint16* first, uint16* second);


#define ARRSIZE(x) (int) (sizeof(x)/sizeof(x[0]))
#define itemsof(x) (sizeof(x)/sizeof(x[0]))
#define ELEMENT_COUNT(X) (sizeof(X) / sizeof(X[0]))


///////////////////////////////////////////////////////////////////////////////
// ED2K File Type
//

enum EED2KFileType
{
	ED2KFT_ANY,
	ED2KFT_AUDIO,
	ED2KFT_VIDEO,
	ED2KFT_IMAGE,
	ED2KFT_PROGRAM,
	ED2KFT_DOCUMENT,
	ED2KFT_ARCHIVE,
	ED2KFT_CDIMAGE
};

class EED2KFileTypeClass
{
public:
	EED2KFileTypeClass()
	{
		s_t = ED2KFT_ANY;
	}
	EED2KFileTypeClass(EED2KFileType t)
	{
		s_t = t;
	}
	EED2KFileType GetType() const
	{
		return s_t;
	}
	
private:
	EED2KFileType s_t;
};

EED2KFileType GetED2KFileTypeID(const wxString &strFileName);
wxString GetED2KFileTypeSearchTerm(EED2KFileType iFileID);
wxString GetFileTypeByName(const wxString &strFileName);
///////////////////////////////////////////////////////////////////////////////

// md4cmp -- replacement for memcmp(hash1,hash2,16)
// Like 'memcmp' this function returns 0, if hash1==hash2, and !0, if hash1!=hash2.
// NOTE: Do *NOT* use that function for determining if hash1<hash2 or hash1>hash2.
inline int md4cmp(const void* hash1, const void* hash2)
{
	const char* hashA = (const char*)hash1;
	const char* hashB = (const char*)hash2;
	
	return !(
		RawPeekUInt64( hashA		) == RawPeekUInt64( hashB		) &&
		RawPeekUInt64( hashA + 8	) == RawPeekUInt64( hashB + 8	)
	);
}


// md4clr -- replacement for memset(hash,0,16)
inline void md4clr(void* hash)
{
	char* pDst = (char*)hash;

	RawPokeUInt64( pDst,		0 );
	RawPokeUInt64( pDst + 8,	0 );
}


// md4cpy -- replacement for memcpy(dst,src,16)
inline void md4cpy(const void* dst, const void* src)
{
	char* pDst = (char*)dst;
	const char* pSrc = (const char*)src;
	
	RawPokeUInt64( pDst, 		RawPeekUInt64( pSrc		) );
	RawPokeUInt64( pDst + 8,	RawPeekUInt64( pSrc + 8	) );
}


// DumpMem ... Dumps mem ;)
wxString DumpMemToStr(const void *buff, int n, const wxString& msg = wxEmptyString, bool ok = true);
void DumpMem(const void *buff, int n, const wxString& msg = wxEmptyString, bool ok = true);
void DumpMem_DW(const uint32 *ptr, int count);

// Returns special source ID for GUI.
// It's actually IP<<16+Port
#define GUI_ID(x,y) (uint64)((((uint64)x)<<16) + (uint64)y)
// And so...
#define PORT_FROM_GUI_ID(x) (x & 0xFFFF)
#define IP_FROM_GUI_ID(x) (x >> 16)


inline void MilliSleep(uint32 msecs) {
	#ifdef __WXBASE__
		#ifdef __WXMSW__
			if (msecs) {
				Sleep(msecs);
			}
		#else
			struct timespec waittime;
				waittime.tv_sec = 0;
				waittime.tv_nsec = msecs * 1000 /*micro*/* 1000 /*nano*/;
			struct timespec remtime;
			while ((nanosleep(&waittime,&remtime)==-1) && (errno == EINTR)) {
				memcpy(&waittime,&remtime,sizeof(struct timespec));
			}
		#endif
	#else
		#if wxCHECK_VERSION(2, 5, 3)
			wxMilliSleep(msecs);
		#else
			wxUsleep(msecs);
		#endif
	#endif
}

inline const long int make_full_ed2k_version(int a, int b, int c){
	return ((a << 17) | (b << 10) | (c << 7));
}

inline wxString GetConfigDir(void) {
	return wxGetHomeDir() + wxFileName::GetPathSeparator() + wxT(".aMule") + wxFileName::GetPathSeparator();
}

#define  wxLANGUAGE_CUSTOM 		wxLANGUAGE_USER_DEFINED+1
#define  wxLANGUAGE_ITALIAN_NAPOLITAN 	wxLANGUAGE_USER_DEFINED+2

/**
 * Adds aMule's custom languages to db.
 */
void InitCustomLanguages();

/**
 * Initializes locale
 */
void InitLocale(wxLocale& locale, int language);

/**
 * Converts a string locale definition to a wxLANGUAGE id.
 */
inline int StrLang2wx(const wxString& language)
{
	// get rid of possible encoding and modifier
	wxString lang(language.BeforeFirst('.').BeforeFirst('@'));

	if (!lang.IsEmpty()) {
#if wxCHECK_VERSION(2,5,4)
		const wxLanguageInfo *lng = wxLocale::FindLanguageInfo(lang);
		if (lng) {
			return lng->Language;
		} else {
			return wxLANGUAGE_DEFAULT;
		}
#else
/*----------------------------------------------------------------------------*\
 * Replacement implementation for wxLocale::FindLanguageInfo().
 * Provides the same functionality, but a little slower, and does not have
 * support for custom languages.
\*----------------------------------------------------------------------------*/
		int RetVal = wxLANGUAGE_DEFAULT;

		// Languages are an enum from wxLANGUAGE_DEFAULT to wxLANGUAGE_USER_DEFINED
		for ( int i = wxLANGUAGE_DEFAULT; i < wxLANGUAGE_USER_DEFINED; i++ ) {

			if ((i == wxLANGUAGE_DEFAULT) || (i == wxLANGUAGE_UNKNOWN)) {
				continue;
			}

			const wxLanguageInfo *info = wxLocale::GetLanguageInfo(i);

			if (!info) {
				continue;
			}

			if ( wxStricmp(lang, info->CanonicalName) == 0 || wxStricmp(lang, info->Description) == 0 ) {
				// exact match, stop searching
				RetVal = i;
				break;
			}

			if ( wxStricmp(lang, info->CanonicalName.BeforeFirst(wxT('_'))) == 0 ) {
				// a match -- but maybe we'll find an exact one later, so continue
				// looking
				//
				// OTOH, maybe we had already found a language match and in this
				// case don't overwrite it becauce the entry for the default
				// country always appears first in ms_languagesDB
				if ( RetVal == wxLANGUAGE_DEFAULT ) {
					RetVal = i;
				}
			}
		}

		return RetVal;
//------------------------------------------------------------------------------
#endif
	} else {
		return wxLANGUAGE_DEFAULT;
	}
}

/**
 * Converts a wxLANGUAGE id to a string locale name.
 */
inline wxString wxLang2Str(const int lang)
{
	if (lang != wxLANGUAGE_DEFAULT) {
		const wxLanguageInfo *lng = wxLocale::GetLanguageInfo(lang);
		if (lng) {
			return lng->CanonicalName;
		} else {
			return wxEmptyString;
		}
	} else {
		return wxEmptyString;
	}
}


/**
 * Automatically unlocks a mutex on construction and locks it on destruction.
 *
 * This class is the complement of wxMutexLocker.  It is intended to be used
 * when a mutex, which is locked for a period of time, needs to be
 * temporarily unlocked for a bit.  For example:
 *
 *	wxMutexLocker lock(mutex);
 *
 *	// ... do stuff that requires that the mutex is locked ...
 *
 *	{
 *		CMutexUnlocker unlocker(mutex);
 *		// ... do stuff that requires that the mutex is unlocked ...
 *	}
 *
 *	// ... do more stuff that requires that the mutex is locked ...
 *
 */
class CMutexUnlocker
{
public:
    // unlock the mutex in the ctor
    CMutexUnlocker(wxMutex& mutex)
        : m_isOk(false), m_mutex(mutex)
        { m_isOk = ( m_mutex.Unlock() == wxMUTEX_NO_ERROR ); }

    // returns true if mutex was successfully unlocked in ctor
    bool IsOk() const
        { return m_isOk; }

    // lock the mutex in dtor
    ~CMutexUnlocker()
        { if ( IsOk() ) m_mutex.Lock(); }

private:
    // no assignment operator nor copy ctor
    CMutexUnlocker(const CMutexUnlocker&);
    CMutexUnlocker& operator=(const CMutexUnlocker&);

    bool     m_isOk;
    wxMutex& m_mutex;
};


} // End namespace

#endif // OTHERFUNCTIONS_H
