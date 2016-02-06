/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_sound.h"
#include "prefecences.h"

#ifdef CLIENT
#define C_BAKUP
#undef CLIENT
#endif

void SgeSound::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(name), NVP(filename), NVP(near), NVP(far));
}

#ifdef CLIENT
void SgeSound::onDbLoad(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer& gt)
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

void SgeSound::onUpdate(std::shared_ptr<Object> &par, Level *l, const glm::vec3 &pos, const GameTimer &gt)
{
    snd.Update();
}
#endif

void SgeSound::Move(const glm::vec3 &p) const
{
#ifdef CLIENT
    snd.Move(p);
#endif
}

void SgeSound::Pitch(float p) const
{
#ifdef CLIENT
    snd.Pitch(p);
#endif
}

void SgeSound::Play() const
{
#ifdef CLIENT
    snd.Play();
#endif
}

#ifdef C_BAKUP
#define CLIENT
#endif
