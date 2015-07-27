#ifndef SCATTERING_H
#define SCATTERING_H
/**
 * Precomputed Atmospheric Scattering
 * Copyright (c) 2008 INRIA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <GL/glew.h>
#include "logger.h"
#include "camera.h"

class Scattering
{
public:
    Scattering();

    const int reflectanceUnit = 0;
    const int transmittanceUnit = 1;
    const int irradianceUnit = 2;
    const int inscatterUnit = 3;
    const int deltaEUnit = 4;
    const int deltaSRUnit = 5;
    const int deltaSMUnit = 6;
    const int deltaJUnit = 7;

    unsigned int reflectanceTexture;//unit 0, ground reflectance texture
    unsigned int transmittanceTexture;//unit 1, T table
    unsigned int irradianceTexture;//unit 2, E table
    unsigned int inscatterTexture;//unit 3, S table
    unsigned int deltaETexture;//unit 4, deltaE table
    unsigned int deltaSRTexture;//unit 5, deltaS table (Rayleigh part)
    unsigned int deltaSMTexture;//unit 6, deltaS table (Mie part)
    unsigned int deltaJTexture;//unit 7, deltaJ table

    unsigned int transmittanceProg;
    unsigned int irradiance1Prog;
    unsigned int inscatter1Prog;
    unsigned int copyIrradianceProg;
    unsigned int copyInscatter1Prog;
    unsigned int jProg;
    unsigned int irradianceNProg;
    unsigned int inscatterNProg;
    unsigned int copyInscatterNProg;

    unsigned int fbo;

    unsigned int drawProg;

    void Precompute();
    void setLayer(unsigned int prog, int layer);
    void redisplayFunc(const Camera &cam);
};

#endif // SCATTERING_H
