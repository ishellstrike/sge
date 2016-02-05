#ifndef REMSND_H
#define REMSND_H
#include <windows.h>
#include <al.h>
#include <alc.h>
#include <string>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <fstream>
#include <map>
#include <glm/glm.hpp>

#define NUM_OF_DYNBUF 2
#define DYNBUF_SIZE 65536

class OpenAL
{
public:
    static ALboolean CheckALCError();
    static ALboolean CheckALError();

    static bool InitializeOpenAL();
    static void DestroyOpenAL();

    static ALCdevice *pDevice;
    static ALCcontext *pContext;

    typedef struct
    {
      unsigned int  ID;
      std::string    Filename;
      unsigned int  Rate;
      unsigned int  Format;
    } SndInfo;

    typedef std::map<ALuint, SndInfo> TBuf;
    static TBuf Buffers;
    static glm::vec3 listener;
};

class RemSnd
{
public:
  glm::vec3 mVel;
  glm::vec3 mPos;
  bool  mLooped;

  bool Open(const std::string &filename, bool Looped, bool Streamed);
  bool IsStreamed();
  void Play() const;
  void Close();
  void Update();
  void Move(const glm::vec3 &pos);
  void Stop();
  void Pitch(float t) const;
  void NearFar(int n, int f);

  RemSnd();
  virtual ~RemSnd();

private:
  ALuint      mSourceID;
  OggVorbis_File  *mVF;
  vorbis_comment  *mComment;
  vorbis_info    *mInfo;

  std::ifstream    OggFile;
  bool      mStreamed;

  bool ReadOggBlock(ALuint BufID, size_t Size);
  bool LoadOggFile (const std::string &Filename, bool Streamed);
};

#endif // REMSND_H
