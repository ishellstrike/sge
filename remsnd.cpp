#include "remsnd.h"
#include <fstream>
#include <map>
#include <boost/filesystem.hpp>
#include "logger.h"
#include "prefecences.h"

bool RemSnd::Open(const std::string &Filename, bool Looped, bool Streamed)
{
    if(Prefecences::Instance()->no_sound)
        return true;

    std::ifstream a((Filename+".ogg").c_str());
    if (!a.is_open())
        return false;
    a.close();

    mLooped    = Looped;

    alGenSources(1, &mSourceID);
    if (!OpenAL::CheckALError())
        return false;

    alSourcef (mSourceID, AL_PITCH,    1.0f);
    alSourcef (mSourceID, AL_GAIN,    1.0f);
    alSourcefv(mSourceID, AL_POSITION,  &mPos[0]);
    alSourcefv(mSourceID, AL_VELOCITY,  &mVel[0]);
    alSourcei (mSourceID, AL_LOOPING,  mLooped);

    mStreamed = Streamed;
    return LoadOggFile(Filename+".ogg", Streamed);
}

bool RemSnd::IsStreamed()
{
    return mStreamed;
}

void RemSnd::Play() const
{
    if(Prefecences::Instance()->no_sound)
        return;

    ALint state;
    alGetSourcei(mSourceID, AL_SOURCE_STATE, &state);
    if(state != AL_PLAYING)
        alSourcePlay(mSourceID);
}

void RemSnd::Close()
{
    if(Prefecences::Instance()->no_sound)
        return;

    alSourceStop(mSourceID);
    if (alIsSource(mSourceID)) alDeleteSources(1, &mSourceID);
}

void RemSnd::Stop()
{
    if(Prefecences::Instance()->no_sound)
        return;

    alSourceStop(mSourceID);
}

void RemSnd::Pitch(float t) const
{
    if(Prefecences::Instance()->no_sound)
        return;

    alSourcef(mSourceID, AL_PITCH, t);
}

void RemSnd::NearFar(int n, int f)
{
    if(Prefecences::Instance()->no_sound)
        return;

    alSourcei(mSourceID, AL_REFERENCE_DISTANCE, n);
    alSourcei(mSourceID, AL_MAX_DISTANCE, f);
}

void RemSnd::Move(const glm::vec3 &pos)
{
    if(Prefecences::Instance()->no_sound)
        return;

    ALint state;
    alGetSourcei(mSourceID, AL_SOURCE_STATE, &state);
    if(state != AL_PLAYING)
    {
        if(glm::distance(pos, OpenAL::listener) < glm::distance(mPos, OpenAL::listener))
        {
            mPos = pos;
            Stop();
        }
        else
            return;
    }
    else
        mPos = pos;
    alSourcefv(mSourceID, AL_POSITION, &pos[0]);
}

void RemSnd::Update()
{
    if(Prefecences::Instance()->no_sound)
        return;

    alListenerfv(AL_POSITION, &OpenAL::listener[0]);
    if (!mStreamed) return;

    int				Processed = 0;
    ALuint			BufID;

    alGetSourcei(mSourceID, AL_BUFFERS_PROCESSED, &Processed);

    while (Processed--)
    {
        alSourceUnqueueBuffers(mSourceID, 1, &BufID);
        if (!OpenAL::CheckALError()) return;
        if (ReadOggBlock(BufID, DYNBUF_SIZE) != 0)
        {
            alSourceQueueBuffers(mSourceID, 1, &BufID);
            if (!OpenAL::CheckALError()) return;
        }
        else
        {
            ov_pcm_seek(mVF, 0);
            alSourceQueueBuffers(mSourceID, 1, &BufID);
            if (!OpenAL::CheckALError()) return;

            if (!mLooped) Stop();
        }
    }
}

RemSnd::RemSnd()
{

}

RemSnd::~RemSnd()
{
    Close();
}

bool RemSnd::ReadOggBlock(ALuint BufID, size_t Size)
{
    char		eof = 0;
    int			current_section;
    long		TotalRet = 0, ret;
    char		*PCM;

    if (Size < 1) return false;
    PCM = new char[Size];

    while (TotalRet < Size)
    {
        ret = ov_read(mVF, PCM + TotalRet, Size - TotalRet, 0, 2, 1, &current_section);

        if (ret == 0) break;
        else if (ret < 0)
        {

        }
        else
        {
            TotalRet += ret;
        }
    }
    if (TotalRet > 0)
    {
        alBufferData(BufID, OpenAL::Buffers[BufID].Format, (void *)PCM,
                     TotalRet, OpenAL::Buffers[BufID].Rate);
        OpenAL::CheckALError();
    }
    delete [] PCM;
    return (ret > 0);
}

size_t ReadOgg(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    std::istream *File = reinterpret_cast<std::istream*>(datasource);
    File->read((char *)ptr, size * nmemb);
    return File->gcount();
}

int SeekOgg(void *datasource, ogg_int64_t offset, int whence)
{
    std::istream *File = reinterpret_cast<std::istream*>(datasource);
    std::ios_base::seekdir Dir;
    File->clear();
    switch (whence)
    {
    case SEEK_SET: Dir = std::ios::beg;  break;
    case SEEK_CUR: Dir = std::ios::cur;  break;
    case SEEK_END: Dir = std::ios::end;  break;
    default: return -1;
    }
    File->seekg((std::streamoff)offset, Dir);
    return (File->fail() ? -1 : 0);
}

long TellOgg(void *datasource)
{
    std::istream *File = reinterpret_cast<std::istream*>(datasource);
    return File->tellg();
}

int CloseOgg(void *datasource)
{
    return 0;
}

bool RemSnd::LoadOggFile(const std::string &Filename, bool Streamed)
{
    if(Prefecences::Instance()->no_sound)
        return true;

    int        i, DynBuffs = 1, BlockSize;
    OpenAL::SndInfo      buffer;
    ALuint      BufID = 0;
    ov_callbacks  cb;

    cb.close_func  = CloseOgg;
    cb.read_func  = ReadOgg;
    cb.seek_func  = SeekOgg;
    cb.tell_func  = TellOgg;

    mVF = new OggVorbis_File;

    OggFile.open(Filename.c_str(), std::ios_base::in | std::ios_base::binary);

    if (ov_open_callbacks(&OggFile, mVF, NULL, -1, cb) < 0)
    {
        return false;
    }

    if (!Streamed)
    {
        for (OpenAL::TBuf::iterator i = OpenAL::Buffers.begin(); i != OpenAL::Buffers.end(); i++)
        {
            if (i->second.Filename == Filename) BufID = i->first;
        }
        BlockSize = ov_pcm_total(mVF, -1) * 4;
    }
    else
    {
        BlockSize  = DYNBUF_SIZE;
        DynBuffs  = NUM_OF_DYNBUF;
        alSourcei(mSourceID, AL_LOOPING, AL_FALSE);
    }

    mComment    = ov_comment(mVF, -1);
    mInfo      = ov_info(mVF, -1);

    buffer.Rate    = mInfo->rate;
    buffer.Filename  = Filename;
    buffer.Format = (mInfo->channels == 1) ? AL_FORMAT_MONO16
                                           : AL_FORMAT_STEREO16;

    if (Streamed || !BufID)
    {
        for (i = 0; i < DynBuffs; i++)
        {
            alGenBuffers(1, &buffer.ID);
            if (!OpenAL::CheckALError())
                return false;
            OpenAL::Buffers[buffer.ID] = buffer;
            ReadOggBlock(buffer.ID, BlockSize);
            if (!OpenAL::CheckALError())
                return false;

            if (Streamed)
            {
                alSourceQueueBuffers(mSourceID, 1, &buffer.ID);
                if (!OpenAL::CheckALError())
                    return false;
            }
            else
                alSourcei(mSourceID, AL_BUFFER, buffer.ID);
        }
    }
    else
    {
        alSourcei(mSourceID, AL_BUFFER, OpenAL::Buffers[BufID].ID);
    }
    LOG(trace) << Filename << " loaded";

    return true;
}

ALboolean OpenAL::CheckALCError()
{
    ALenum ErrCode;
    std::string Err = "ALC error: ";
    if ((ErrCode = alcGetError(pDevice)) != ALC_NO_ERROR)
    {
        Err += (char *)alcGetString(pDevice, ErrCode);
        LOG(error) << Err.data();
        return AL_FALSE;
    }
    return AL_TRUE;
}

ALboolean OpenAL::CheckALError()
{
    ALenum ErrCode;
    std::string Err = "OpenAL error: ";
    if ((ErrCode = alGetError()) != AL_NO_ERROR)
    {
        Err += (char *)alGetString(ErrCode);
        LOG(error) << Err.data();
        return AL_FALSE;
    }
    return AL_TRUE;
}

bool OpenAL::InitializeOpenAL()
{
    if(Prefecences::Instance()->no_sound)
    {
        LOG(info) << "-nosound. OpenAL disabled.";
        return true;
    }

    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

    pDevice = alcOpenDevice(nullptr);

    if (!pDevice)
    {
        LOG(error) << "Default sound device not present";
        return false;
    }
    pContext = alcCreateContext(pDevice, nullptr);
    if (!CheckALCError()) return false;

    alcMakeContextCurrent(pContext);

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
    LOG(trace) << "OpenAL init complete";
    return true;
}

void OpenAL::DestroyOpenAL()
{
    if(Prefecences::Instance()->no_sound)
        return;

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);
    LOG(trace) << "OpenAL destroyed";
}

ALCdevice *OpenAL::pDevice;
ALCcontext *OpenAL::pContext;
OpenAL::TBuf OpenAL::Buffers;
glm::vec3 OpenAL::listener;
