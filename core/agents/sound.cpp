#include "sound.h"
#include "prefecences.h"

void Sound::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(name), NVP(filename), NVP(near), NVP(far));
}

void Sound::onLoad(Object *par, Level *l, const glm::vec3 &pos, const GameTimer& gt)
{
    if(!filename.empty())
    {
        bool suc = snd.Open(Prefecences::Instance()->getSoundsDir() + filename, false, false);
        if(!suc)
        {
            LOG(error) << filename << " loading failed";
            return;
        }
        snd.NearFar(near, far);
    }
}

void Sound::onUpdate(Object *par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    snd.Update();
}

void Sound::Move(const glm::vec3 &p) const
{
    snd.Move(p);
}

void Sound::Pitch(float p) const
{
    snd.Pitch(p);
}

void Sound::Play() const
{
    snd.Play();
}
