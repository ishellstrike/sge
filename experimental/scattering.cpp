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
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;
#include "scattering.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "data/shaders/scattering/scatter_params.h"
#include "prefecences.h"
#include "random.h"

Scattering::Scattering()
{

}

string* loadFile(const string &fileName)
{
    string* result = new string();
    ifstream file(fileName.c_str());
    if (!file) {
        LOG(error) << "Cannot open file " << fileName;
        throw exception();
    }
    string line;
    while (getline(file, line)) {
        *result += line;
        *result += '\n';
    }
    file.close();
    return result;
}

void printShaderLog(int shaderId)
{
    int logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char *log = new char[logLength];
        glGetShaderInfoLog(shaderId, logLength, &logLength, log);
        LOG(verbose) << string(log);
        delete[] log;
    }
}

unsigned int loadProgram(const vector<string> &files)
{
    unsigned int programId = glCreateProgram();
    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    int n = files.size();
    string **strs = new string*[n];
    const char** lines = new const char*[n + 1];
    LOG(verbose) << "loading program " << files[n - 1] << "...";
    bool geo = false;
    for (int i = 0; i < n; ++i) {
        string* s = loadFile(files[i]);
        strs[i] = s;
        lines[i + 1] = s->c_str();
        if (strstr(lines[i + 1], "_GEOMETRY_") != NULL) {
            geo = true;
        }
    }

    lines[0] = "#version 330 core\n#define _VERTEX_\n";
    glShaderSource(vertexShaderId, n + 1, lines, NULL);
    glCompileShader(vertexShaderId);
    printShaderLog(vertexShaderId);

    if (geo) {
        unsigned geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT);
        glAttachShader(programId, geometryShaderId);
        lines[0] = "#version 330 core\n#define _GEOMETRY_\n";
        glShaderSource(geometryShaderId, n + 1, lines, NULL);
        glCompileShader(geometryShaderId);
        printShaderLog(geometryShaderId);
        glProgramParameteriEXT(programId, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
        glProgramParameteriEXT(programId, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
        glProgramParameteriEXT(programId, GL_GEOMETRY_VERTICES_OUT_EXT, 3);
    }

    lines[0] = "#version 330 core\n#define _FRAGMENT_\n";
    glShaderSource(fragmentShaderId, n + 1, lines, NULL);
    glCompileShader(fragmentShaderId);
    printShaderLog(fragmentShaderId);

    glLinkProgram(programId);

    for (int i = 0; i < n; ++i) {
        delete strs[i];
    }
    delete[] strs;
    delete[] lines;

    return programId;
}

void drawQuad(const vec4 &view = vec4(-1,-1,1,1))
{
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(view.x, view.y);
    glVertex2f(view.z, view.y);
    glVertex2f(view.x, view.w);
    glVertex2f(view.z, view.w);
    glEnd();
}

void Scattering::setLayer(unsigned int prog, int layer)
{
    double r = layer / (RES_R - 1.0);
    r = r * r;
    r = sqrt(Rg * Rg + r * (Rt * Rt - Rg * Rg)) + (layer == 0 ? 0.01 : (layer == RES_R - 1 ? -0.001 : 0.0));
    double dmin = Rt - r;
    double dmax = sqrt(r * r - Rg * Rg) + sqrt(Rt * Rt - Rg * Rg);
    double dminp = r - Rg;
    double dmaxp = sqrt(r * r - Rg * Rg);
    glUniform1f(glGetUniformLocation(prog, "r"), float(r));
    glUniform4f(glGetUniformLocation(prog, "dhdH"), float(dmin), float(dmax), float(dminp), float(dmaxp));
    glUniform1i(glGetUniformLocation(prog, "layer"), layer);
}

void Scattering::Precompute()
{
    glActiveTexture(GL_TEXTURE0 + transmittanceUnit);
    glGenTextures(1, &transmittanceTexture);
    glBindTexture(GL_TEXTURE_2D, transmittanceTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, TRANSMITTANCE_W, TRANSMITTANCE_H, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + irradianceUnit);
    glGenTextures(1, &irradianceTexture);
    glBindTexture(GL_TEXTURE_2D, irradianceTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, SKY_W, SKY_H, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + inscatterUnit);
    glGenTextures(1, &inscatterTexture);
    glBindTexture(GL_TEXTURE_3D, inscatterTexture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F_ARB, RES_MU_S * RES_NU, RES_MU, RES_R, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + deltaEUnit);
    glGenTextures(1, &deltaETexture);
    glBindTexture(GL_TEXTURE_2D, deltaETexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, SKY_W, SKY_H, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + deltaSRUnit);
    glGenTextures(1, &deltaSRTexture);
    glBindTexture(GL_TEXTURE_3D, deltaSRTexture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F_ARB, RES_MU_S * RES_NU, RES_MU, RES_R, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + deltaSMUnit);
    glGenTextures(1, &deltaSMTexture);
    glBindTexture(GL_TEXTURE_3D, deltaSMTexture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F_ARB, RES_MU_S * RES_NU, RES_MU, RES_R, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + deltaJUnit);
    glGenTextures(1, &deltaJTexture);
    glBindTexture(GL_TEXTURE_3D, deltaJTexture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F_ARB, RES_MU_S * RES_NU, RES_MU, RES_R, 0, GL_RGB, GL_FLOAT, NULL);

    vector<string> files;
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/transmittance.glsl");
    transmittanceProg = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/irradiance1.glsl");
    irradiance1Prog = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/inscatter1.glsl");
    inscatter1Prog = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/copyIrradiance.glsl");
    copyIrradianceProg = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/copyInscatter1.glsl");
    copyInscatter1Prog = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/inscatterS.glsl");
    jProg = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/irradianceN.glsl");
    irradianceNProg = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/inscatterN.glsl");
    inscatterNProg = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/copyInscatterN.glsl");
    copyInscatterNProg = loadProgram(files);

    files.clear();
    files.push_back("data/shaders/scattering/scatter_params.h");
    files.push_back("data/shaders/scattering/common.glsl");
    files.push_back("data/shaders/scattering/earth.glsl");
    drawProg = loadProgram(files);
    glUseProgram(drawProg);
    glUniform1i(glGetUniformLocation(drawProg, "reflectanceSampler"), reflectanceUnit);
    glUniform1i(glGetUniformLocation(drawProg, "transmittanceSampler"), transmittanceUnit);
    glUniform1i(glGetUniformLocation(drawProg, "irradianceSampler"), irradianceUnit);
    glUniform1i(glGetUniformLocation(drawProg, "inscatterSampler"), inscatterUnit);

    LOG(verbose) << glGetUniformLocation(drawProg, "reflectanceSampler") << "; " <<
                    glGetUniformLocation(drawProg, "transmittanceSampler") << "; " <<
                    glGetUniformLocation(drawProg, "irradianceSampler") << "; " <<
                    glGetUniformLocation(drawProg, "inscatterSampler") << "; ";

    LOG(verbose) << "precomputations...";

    glGenFramebuffers(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

    // computes transmittance texture T (line 1 in algorithm 4.1)
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, transmittanceTexture, 0);
    glViewport(0, 0, TRANSMITTANCE_W, TRANSMITTANCE_H);
    glUseProgram(transmittanceProg);
    drawQuad();

    // computes irradiance texture deltaE (line 2 in algorithm 4.1)
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, deltaETexture, 0);
    glViewport(0, 0, SKY_W, SKY_H);
    glUseProgram(irradiance1Prog);
    glUniform1i(glGetUniformLocation(irradiance1Prog, "transmittanceSampler"), transmittanceUnit);
    drawQuad();

    // computes single scattering texture deltaS (line 3 in algorithm 4.1)
    // Rayleigh and Mie separated in deltaSR + deltaSM
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, deltaSRTexture, 0);
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, deltaSMTexture, 0);
    unsigned int bufs[2] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
    glDrawBuffers(2, bufs);
    glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
    glUseProgram(inscatter1Prog);
    glUniform1i(glGetUniformLocation(inscatter1Prog, "transmittanceSampler"), transmittanceUnit);
    for (int layer = 0; layer < RES_R; ++layer) {
        setLayer(inscatter1Prog, layer);
        drawQuad();
    }
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, 0, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

    // copies deltaE into irradiance texture E (line 4 in algorithm 4.1)
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, irradianceTexture, 0);
    glViewport(0, 0, SKY_W, SKY_H);
    glUseProgram(copyIrradianceProg);
    glUniform1f(glGetUniformLocation(copyIrradianceProg, "k"), 0.0);
    glUniform1i(glGetUniformLocation(copyIrradianceProg, "deltaESampler"), deltaEUnit);
    drawQuad();

    // copies deltaS into inscatter texture S (line 5 in algorithm 4.1)
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, inscatterTexture, 0);
    glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
    glUseProgram(copyInscatter1Prog);
    glUniform1i(glGetUniformLocation(copyInscatter1Prog, "deltaSRSampler"), deltaSRUnit);
    glUniform1i(glGetUniformLocation(copyInscatter1Prog, "deltaSMSampler"), deltaSMUnit);
    for (int layer = 0; layer < RES_R; ++layer) {
        setLayer(copyInscatter1Prog, layer);
        drawQuad();
    }

    // loop for each scattering order (line 6 in algorithm 4.1)
    for (int order = 2; order <= 4; ++order) {

        // computes deltaJ (line 7 in algorithm 4.1)
        glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, deltaJTexture, 0);
        glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
        glUseProgram(jProg);
        glUniform1f(glGetUniformLocation(jProg, "first"), order == 2 ? 1.0 : 0.0);
        glUniform1i(glGetUniformLocation(jProg, "transmittanceSampler"), transmittanceUnit);
        glUniform1i(glGetUniformLocation(jProg, "deltaESampler"), deltaEUnit);
        glUniform1i(glGetUniformLocation(jProg, "deltaSRSampler"), deltaSRUnit);
        glUniform1i(glGetUniformLocation(jProg, "deltaSMSampler"), deltaSMUnit);
        for (int layer = 0; layer < RES_R; ++layer) {
            setLayer(jProg, layer);
            drawQuad();
        }

        // computes deltaE (line 8 in algorithm 4.1)
        glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, deltaETexture, 0);
        glViewport(0, 0, SKY_W, SKY_H);
        glUseProgram(irradianceNProg);
        glUniform1f(glGetUniformLocation(irradianceNProg, "first"), order == 2 ? 1.0 : 0.0);
        glUniform1i(glGetUniformLocation(irradianceNProg, "transmittanceSampler"), transmittanceUnit);
        glUniform1i(glGetUniformLocation(irradianceNProg, "deltaSRSampler"), deltaSRUnit);
        glUniform1i(glGetUniformLocation(irradianceNProg, "deltaSMSampler"), deltaSMUnit);
        drawQuad();

        // computes deltaS (line 9 in algorithm 4.1)
        glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, deltaSRTexture, 0);
        glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
        glUseProgram(inscatterNProg);
        glUniform1f(glGetUniformLocation(inscatterNProg, "first"), order == 2 ? 1.0 : 0.0);
        glUniform1i(glGetUniformLocation(inscatterNProg, "transmittanceSampler"), transmittanceUnit);
        glUniform1i(glGetUniformLocation(inscatterNProg, "deltaJSampler"), deltaJUnit);
        for (int layer = 0; layer < RES_R; ++layer) {
            setLayer(inscatterNProg, layer);
            drawQuad();
        }

        glEnable(GL_BLEND);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

        // adds deltaE into irradiance texture E (line 10 in algorithm 4.1)
        glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, irradianceTexture, 0);
        glViewport(0, 0, SKY_W, SKY_H);
        glUseProgram(copyIrradianceProg);
        glUniform1f(glGetUniformLocation(copyIrradianceProg, "k"), 1.0);
        glUniform1i(glGetUniformLocation(copyIrradianceProg, "deltaESampler"), deltaEUnit);
        drawQuad();

        // adds deltaS into inscatter texture S (line 11 in algorithm 4.1)
        glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, inscatterTexture, 0);
        glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
        glUseProgram(copyInscatterNProg);
        glUniform1i(glGetUniformLocation(copyInscatterNProg, "deltaSSampler"), deltaSRUnit);
        for (int layer = 0; layer < RES_R; ++layer) {
            setLayer(copyInscatterNProg, layer);
            drawQuad();
        }

        glDisable(GL_BLEND);
    }

    glFinish();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glViewport(0, 0, RESX, RESY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    LOG(verbose) << "ready.";
}

glm::vec3 s(0.0, -1, 0.0);
float exposure = 0.4f;

void Scattering::redisplayFunc(const Camera & cam)
{
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glClearColor(0,0,0,1);

    glm::mat4 iproj = glm::inverse(cam.Projection());
    glm::mat4 iview = glm::inverse(cam.View());

    //glm::vec4 c = iview * glm::vec4(0.f, 0.f, 0.f, 1.f); //same
    glm::vec4 c = glm::vec4(cam.Position(), 1);

    s = glm::vec3(1,0,0);

    glUseProgram(drawProg);
    glUniform3f(glGetUniformLocation(drawProg, "c"), c.x, c.y, c.z);
    glUniform3f(glGetUniformLocation(drawProg, "s"), s.x, s.y, s.z);
    glUniformMatrix4fv(glGetUniformLocation(drawProg, "projInverse"), 1, GL_FALSE, &iproj[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(drawProg, "viewInverse"), 1, GL_FALSE, &iview[0][0]);
    glUniform1f(glGetUniformLocation(drawProg, "exposure"), exposure);
    drawQuad();
    glFinish();
    glFlush();
}
