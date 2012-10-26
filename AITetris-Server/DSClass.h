//based on the source found at
//http://www.freelists.org/archives/directmusic/03-2003/fullthread1.html
//changes: 
//allows more than one sound buffer
//variable size buffers
//needed files: 
//http://www.vorbis.com/download_win.psp  <- Ogg Vorbis SDK
//
//Link the following files to your project: vorbisfile_static.lib,
//vorbis_static.lib, ogg_static, dsound.lib. Also be sure your include
//paths contain the SDK.
/**************************************************************
* OggPlayer.h
***************************************************************/
#pragma once

#include    <windows.h>                 // from your fave include dir ;)
#include    <mmreg.h>

#include    <dsound.h>                  // from the directx 8 sdk

#include    "vorbis/codec.h"            // from the vorbis sdk
#include    "vorbis/vorbisfile.h"       // also :)

#define     BUFSIZE    1048576*60         // buffer length
#define		NUMSOUNDS  32

class OggPlayer
{
protected:

    bool        bInitialized;           // initialized?

    bool        bReleaseDS;             // release ds by ourselves?

    LPDIRECTSOUND8 pDS;                    // the directsound 8 object

	struct soundFile
	{
		LPDIRECTSOUNDBUFFER  pDSB;           // the buffer
	    bool        bFileOpened;            // have we opened an ogg yet?
	    bool        bLoop;                  // loop?
		int         nLastSection,           // which half of the buffer are/were
	                nCurSection;            // we playing?
		unsigned int actualSize;			// actual size of buffer
	};
	soundFile	sound[NUMSOUNDS];

    OggVorbis_File vf;                     // for the vorbisfile interface
											//tf is temporary for checking file size

public:

    OggPlayer();
    ~OggPlayer();

    bool InitDirectSound( HWND hWnd );// initialize dsound ..
    inline void UseDirectSound( LPDIRECTSOUND8 _pDS ) { pDS = _pDS; }// .. or use already initialized
    bool OpenOgg( char *filename ); // this opens an oggvorbis for playing
    void Close();// close out all files
	void Close(int i); //close specific file
    void Play(int i, bool loop = false); // play it again sam
    void Stop(int i); // stop it
    bool IsPlaying(int i);//
};

