/****************************************************
* OggPlayer.cpp
****************************************************/
#include "dxstdafx.h"
#include "dsclass.h"

OggPlayer::OggPlayer()
{
	pDS             = NULL;
	bInitialized    = false;
	bReleaseDS      = false;

	for(int j=0;j<NUMSOUNDS;j++)
	{
		sound[j].bFileOpened     = false;
		sound[j].pDSB            = NULL;
		sound[j].bLoop           = false;
	}
}

OggPlayer::~OggPlayer()
{
	if (bReleaseDS && pDS)
		pDS->Release();
}

bool OggPlayer::InitDirectSound( HWND hWnd )
{
	HRESULT hr;

	if (FAILED(hr = DirectSoundCreate8(NULL, &pDS, NULL)))
		return bInitialized = false;

	pDS->Initialize(NULL);
	pDS->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );

	bReleaseDS = true;

	return bInitialized = true;
}

bool OggPlayer::OpenOgg( char *filename )
{
	if (!bInitialized)
		return false;

	int currBuf;

	currBuf=-1;
	for(int j=0;j<NUMSOUNDS; j++)
	{
		if(!sound[j].bFileOpened)
		{
			currBuf=j;
			j=NUMSOUNDS;
		}
	}
	if(currBuf<0)
		return false;

	LPDIRECTSOUNDBUFFER  pTemp;           // the temp buffer

	FILE    *f;

	f = fopen(filename, "rb");
	if (!f) return false;

	ov_open(f, &vf, NULL, 0);

	// ok now the tricky part

	// the vorbis_info struct keeps the most of the interesting format info
	vorbis_info *vi = ov_info(&vf,-1);

	// set the wave format
	WAVEFORMATEX        wfm;

	memset(&wfm, 0, sizeof(wfm));

	wfm.cbSize          = sizeof(wfm);
	wfm.nChannels       = vi->channels;
	wfm.wBitsPerSample  = 16;                    // ogg vorbis is always 16 bit
	wfm.nSamplesPerSec  = vi->rate;
	wfm.nAvgBytesPerSec = wfm.nSamplesPerSec*wfm.nChannels*2;
	wfm.nBlockAlign     = 2*wfm.nChannels;
	wfm.wFormatTag      = 1;


	// set up the buffer
	DSBUFFERDESC desc;

	desc.dwSize         = sizeof(desc);
	desc.dwFlags        = 0;
	desc.lpwfxFormat    = &wfm;
	desc.dwReserved     = 0;
	desc.dwBufferBytes  = BUFSIZE;


	//CREATE TEMPORARY BUFFER
	pDS->CreateSoundBuffer(&desc, &pTemp, NULL );

	// fill the buffer

	DWORD   size = BUFSIZE;
	DWORD   pos = 0;
	int     sec = 0;
	int     ret = 1;

	char    *buf;
	char    *buf2;

	pTemp->Lock(0, size, (LPVOID*)&buf, &size, NULL, NULL, DSBLOCK_ENTIREBUFFER);

	// now read in the bits
	while(ret && pos<size)
	{
		ret = ov_read(&vf, buf+pos, size-pos, 0, 2, 1, &sec);
		pos += ret;
	}

	pTemp->Unlock( buf, size, NULL, NULL );

	//CREATE BUFFER TO MATCH ACTUAL SIZE OF FILE
	desc.dwBufferBytes  = pos;


	pDS->CreateSoundBuffer(&desc, &sound[currBuf].pDSB, NULL );
	sound[currBuf].pDSB->Lock(0, pos, (LPVOID*)&buf2, &pos, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	CopyMemory(buf2,buf,pos);
	sound[currBuf].pDSB->Unlock( buf2, size, NULL, NULL );

	//DUMP THE TEMP BUFFER
	pTemp->Release();

	sound[currBuf].nCurSection			=
		sound[currBuf].nLastSection			= 0;
	sound[currBuf].actualSize			= pos;

	ov_clear(&vf);
	fclose(f);

	buf=NULL;

	return sound[currBuf].bFileOpened = true;
}

void OggPlayer::Close()
{

	for(int j=0;j<NUMSOUNDS;j++)
		Close(j);
}

void OggPlayer::Close(int i)
{

	sound[i].bFileOpened = false;

	if (sound[i].pDSB)
		sound[i].pDSB->Release();
}


void OggPlayer::Play(int i, bool loop)
{
	
	if (!bInitialized)
		return;

	if (!sound[i].bFileOpened)
		return;

	if (!sound[i].pDSB)
		return;

	// looping is handled in the "isplaying" routine
	sound[i].bLoop = loop;
	sound[i].pDSB->Play(0,0,NULL);    

	
}

bool OggPlayer::IsPlaying(int i) 
{ 
	if (i>= NUMSOUNDS || i<0) 
		return false; 

	if (!sound[i].pDSB)
		return false;

	DWORD dwStatus;
	DWORD pos;

	sound[i].pDSB->GetStatus(&dwStatus);
	if(dwStatus & DSBSTATUS_PLAYING)
	{
		sound[i].pDSB->GetCurrentPosition(&pos,NULL);
		if(pos>=sound[i].actualSize)
		{
			if(!sound[i].bLoop)
			{
				Stop(i);
				return false;
			}
			else
			{
				sound[i].pDSB->SetCurrentPosition(0);
				return true;
			}
		}
		return true;
	}
	else if(sound[i].bLoop)
	{
		Play(i,true);
		return true;
	}

	return false;
}

void OggPlayer::Stop(int i)
{

	if (!bInitialized)
		return;

	if (!sound[i].bFileOpened)
		return;

	if (!sound[i].pDSB)
		return;

	sound[i].pDSB->Stop();
	sound[i].pDSB->SetCurrentPosition(0);
	sound[i].bLoop=false;
}
