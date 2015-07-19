#include "mesh.h"
#include <vector>
#include <math.h>
#include "../spritebatch.h"
#include "../helper.h"
#include "../logger.h"
#include <fstream>
#include <sstream>

Mesh::Mesh(void)
{
}

Mesh::Mesh(const Mesh &m) :
    World(m.World),
    shader(m.shader),
    material(m.material)
{
    Vertices = m.Vertices;
    Indices = m.Indices;
}


Mesh::~Mesh(void)
{
    if(m_vbo) {
        glDeleteBuffers(2, m_vbo);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, m_vao);

        delete[] m_vbo;
        m_vbo = nullptr;
        delete[] m_vao;
        m_vao = nullptr;
        OPENGL_CHECK_ERRORS();
    }
}

/*!
 * \brief Mesh::Unindex
 *
 * Create unique vertex for each index
 */
void Mesh::Unindex()
{
    auto temp = std::move(Vertices);
    Vertices.resize(Indices.size()); 
    for(int i=0; i<Indices.size();i++){
        Vertices[i] = temp[Indices[i]];
        Indices[i] = i;
    }
} 


/*!
 * \brief CalcTangentBasis
 * \param p1
 * \param p2
 * \param p3
 * \param t1
 * \param t2
 * \param t3
 * \param tangent out value
 * \param binormal out value
 *
 * Build tangent and binormal
 */
void CalcTangentBasis(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3,
                      const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3,
                      glm::vec3 &tangent, glm::vec3 &binormal )
{
  glm::vec3 e1  = p2 - p1;
  glm::vec3 e2  = p3 - p1;
  glm::vec2 et1 = t2 - t1;
  glm::vec2 et2 = t3 - t1;
  tangent   = glm::vec3(0.0f);
  binormal  = glm::vec3(0.0f);
  float tmp = 0.0;
  if (glm::abs(et1.x*et2.y - et1.y*et2.x)<0.0001f)
    tmp = 1.0f;
  else
    tmp = 1.0 / (et1.x*et2.y - et1.y*et2.x);
  tangent  = (e1 * et2.y - e2 * et1.y) * tmp;
  binormal = (e2 * et1.x - e1 * et2.x) * tmp;
  tangent = glm::normalize(tangent);
  binormal = glm::normalize(binormal);
}

/*!
 * \brief Mesh::CalcTB
 *
 * Calculate tangents and binormals for mesh
 * needs normals
 */
void Mesh::CalcTB()
{
  for (unsigned int i=0; i < Vertices.size(); i++)
  {
    Vertices[i].tangent = glm::vec3(0.0f);
    Vertices[i].binormal = glm::vec3(0.0f);
  }
  for (unsigned int i=0; i < (Vertices.size() / 3);i++)
  {
    unsigned int a = Indices[i * 3 + 0];
    unsigned int b = Indices[i * 3 + 1];
    unsigned int c = Indices[i * 3 + 2];

    glm::vec3 bin, tan;
    CalcTangentBasis ( Vertices[a].position,
                       Vertices[b].position,
                       Vertices[c].position,
                       Vertices[a].uv,
                       Vertices[b].uv,
                       Vertices[c].uv,
                       tan, bin);

    Vertices[a].tangent  += tan;
    Vertices[b].tangent  += tan;
    Vertices[c].tangent  += tan;

    Vertices[a].binormal += bin;
    Vertices[b].binormal += bin;
    Vertices[c].binormal += bin;
  }

  for (unsigned int i=0; i < Vertices.size();i++)
  {
    Vertices[i].tangent = glm::normalize(Vertices[i].tangent);
    Vertices[i].binormal = glm::normalize(Vertices[i].binormal);

    glm::vec3 tmpT = Vertices[i].tangent;
    glm::vec3 tmpB = Vertices[i].binormal;
    glm::vec3 tmpN = Vertices[i].normal;

    glm::vec3 newT = tmpT - ((glm::cross(tmpN, tmpT)) * tmpN);
    glm::vec3 newB = tmpB - ((glm::cross(tmpN, tmpB)) * tmpN) - ((glm::cross(newT, tmpB))*newT);
    newT = glm::normalize(newT);
    newB = glm::normalize(newB);
    Vertices[i].tangent  = newT;
    Vertices[i].binormal = newB;

    float lenT = newT.length ();
    float lenB = newB.length ();

    if (lenT <= 0.0001 || lenB <= 0.0001)
    {
      if (lenT > 0.5)
        Vertices[i].binormal = glm::cross(tmpN, Vertices[i].tangent);
      else if (lenB > 0.5)
        Vertices[i].tangent  = glm::cross(Vertices[i].binormal, tmpN);
      else
      {
        glm::vec3 xAxis (1.0, 0.0, 0.0);
        glm::vec3 yAxis (0.0, 1.0, 0.0);
        glm::vec3 startAxis;
        if ((glm::dot(xAxis, tmpN)) < (glm::dot(yAxis, tmpN)))
          startAxis = xAxis;
        else
          startAxis = yAxis;
        Vertices[i].tangent  = glm::cross(tmpN, startAxis);
        Vertices[i].binormal = glm::cross(tmpN, Vertices[i].tangent);
      }
    }
    else if ((glm::dot(Vertices[i].binormal, Vertices[i].tangent))> 0.9999)
      Vertices[i].binormal = glm::cross(tmpN, Vertices[i].tangent);
  }
}


/*!
 * \brief Mesh::computeNormal
 *
 * needs unindexed model
 */
void Mesh::computeNormal()
{
    for(size_t i=0; i<Vertices.size();i+=3){
        glm::vec3 const & a = Vertices[i].position;
        glm::vec3 const & b = Vertices[i+1].position;
        glm::vec3 const & c = Vertices[i+2].position;
        auto t = glm::normalize(glm::cross(c - a, b - a));
        Vertices[i].normal = Vertices[i+1].normal = Vertices[i+2].normal = t;
    }
} 

/*!
 * \brief Mesh::MergeVerteces
 *
 * Merge same vertices and combine their normals
 */
void Mesh::MergeVerteces()
{
    for (size_t i=0;i<Vertices.size();i++)
    {
        auto n = Vertices[i].normal;
        float nn = 1.0f;
        std::vector<int> same;
        for (size_t j=i;j<Vertices.size();j++)
        {
            if(Vertices[i].position == Vertices[j].position){
                n += Vertices[j].normal;
                nn++;
                same.push_back(j);
            }
        }
        n /= nn;
        Vertices[i].normal = n;
        if(same.size() > 0)
            for(size_t j=0; j<same.size(); j++){
                Vertices[same[j]].normal = n;
            }
    }
}

void Mesh::Create(std::vector<VertPosNormTanBiTex> v, std::vector<GLuint> i)
{
    Vertices.assign(v.begin(), v.end());
    Indices.assign(i.begin(), i.end());
}

/*!
 * \brief Mesh::loadOBJ
 * \param path
 * \return sucess
 *
 * Limited .obj parser
 */
bool Mesh::loadOBJ(const std::string &path)
{
    std::vector< GLuint > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    FILE * file = fopen(path.c_str(), "r");
    if( file == NULL ){    
        printf("Impossible to open the file !\n");    
        return false;
    }

    while( 1 ){
        char lineHeader[512];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                                                                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                                                                &vertexIndex[2], &uvIndex[2], &normalIndex[2],
                                                                                &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
            if (matches > 9){
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[3]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[3]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[3]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                uvIndices    .push_back(uvIndex[3]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[3]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                normalIndices.push_back(normalIndex[3]);
            } else {
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            if(matches < 6) {
                LOG(fatal) << "Model ruined";
            }
        }
    }

    Vertices.clear();
    Vertices.resize(vertexIndices.size());
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        Vertices[i] = VertPosNormTanBiTex(temp_normals[normalIndices[i]-1], temp_vertices[vertexIndices[i]-1], temp_uvs[uvIndices[i]-1]);
    }
    /*for( unsigned int i=0; i<vertexIndices.size(); i++ ){
    unsigned int vertexIndex = vertexIndices[i];
    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
    unsigned int normalIndex = normalIndices[i];
    glm::vec3 normal = temp_normals[ normalIndex-1 ];
    unsigned int uvIndex = uvIndices[i];
    glm::vec2 uv = temp_uvs[ uvIndex-1 ];
    Verteces[i] = VertexPositionTexture(vertex, normal, uv);
    }*/
    Indices.clear();
    Indices.resize(vertexIndices.size());
    for(size_t i=0;i<vertexIndices.size();i++){
        Indices[i] = i;
    }
}

bool Mesh::loadMTL(const std::string &path)
{
    return 0;
}

void Mesh::Bind(int type /* = 0 */)
{
    assert(shader && "need shader to bind");

    if(Vertices.size() == 0){
        return;
    }
    auto bindtype = type == 0 ? GL_STATIC_DRAW : GL_STREAM_DRAW;

    if(m_vbo != nullptr) { // rebind
        glBindVertexArray(0);
        glDisableVertexAttribArray(shader->posAttrib);
        glDisableVertexAttribArray(shader->uvAttrib);
        glDisableVertexAttribArray(shader->normAttrib);

        glDeleteBuffers(2, m_vbo);
        glDeleteVertexArrays(1, m_vao);
        m_vao = 0;
    }

    if(m_vao == nullptr){
        m_vao = new GLuint[1];
        glGenVertexArrays(1, m_vao);
        glBindVertexArray(*m_vao);
        if(m_vbo){
            delete[] m_vbo;
        }
        m_vbo = new GLuint[2];
        glGenBuffers(2, m_vbo);
    } else {
        glBindVertexArray(*m_vao);
    }
    GLuint stride = sizeof(VertPosNormTanBiTex);
    GLuint offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertPosNormTanBiTex)*Vertices.size(), &Vertices[0], bindtype);
    glEnableVertexAttribArray(shader->posAttrib);
    glVertexAttribPointer(shader->posAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
    glEnableVertexAttribArray(shader->normAttrib);
    glVertexAttribPointer(shader->normAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
    glEnableVertexAttribArray(shader->tangentAttrib);
    glVertexAttribPointer(shader->tangentAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
    glEnableVertexAttribArray(shader->binormalAttrib);
    glVertexAttribPointer(shader->binormalAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset)); offset += sizeof(glm::vec3);
    glEnableVertexAttribArray(shader->uvAttrib);
    glVertexAttribPointer(shader->uvAttrib, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));  offset += sizeof(glm::vec2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Indices.size(), &Indices[0], bindtype);

    loaded = Indices.size();
    vertices = Vertices.size();

    OPENGL_CHECK_ERRORS();
}

void Mesh::ForgetBind()
{
    Bind();
    Indices.clear();
    Indices.shrink_to_fit();
    Vertices.clear();
    Vertices.shrink_to_fit();
}

void Mesh::Render(const Camera &cam, bool patches /* = false*/)
{
    const glm::mat4 &MVP = cam.MVP();

    if(Vertices.size() == 0 && !loaded){
        return;
    }
    if(!m_vao || !m_vbo)
        return;
    if(!shader)
        throw std::logic_error("null shader");

    shader->Use();
    glUniform3fv(shader->viewPosition_location, 1, &cam.Position()[0]);

    glUniformMatrix4fv(shader->mat_model_location, 1, GL_FALSE, &World[0][0]);
    glUniformMatrix4fv(shader->mat_viewProjection_location, 1, GL_FALSE, &MVP[0][0]);
    glm::mat4 normal = glm::transpose(glm::inverse(World));
    glUniformMatrix4fv(shader->mat_normal_location, 1, GL_FALSE, &normal[0][0]);
    glUniform3fv(shader->lightPosition_location, 1, &glm::vec3(200000,234560,9850000)[0]);

    if(shader->ambient_location != -1)
        glUniform4fv(shader->ambient_location,   1, &material->ambient[0]);
    if(shader->diffuse_location != -1)
        glUniform4fv(shader->diffuse_location,   1, &material->diffuse[0]);
    if(shader->specular_location != -1)
        glUniform4fv(shader->specular_location,  1, &material->specular[0]);
    if(shader->emission_location != -1)
        glUniform4fv(shader->emission_location,  1, &material->emission[0]);
    if(shader->shininess_location != -1)
        glUniform1fv(shader->shininess_location, 1, &material->shininess);

    if(shader->texture_location != -1)
        glUniform1i(shader->texture_location, 0);
    if(shader->normal_location != -1)
        glUniform1i(shader->normal_location, 1);
    if(shader->height_location != -1)
        glUniform1i(shader->height_location, 2);
    if(shader->height_location != -1)
        glUniform1i(shader->grad_location, 3);

    if(material && material->texture != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
    }
    if(material && material->normal != nullptr) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->normal->textureId);
    }
    if(material && material->height != nullptr) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->height->textureId);
    }
    if(material && material->grad != nullptr) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, material->grad->textureId);
    }

    glBindVertexArray(*m_vao);

    if(!patches) {
        glDrawElements(GL_TRIANGLES, loaded, GL_UNSIGNED_INT, NULL);
    } else
    {
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glDrawElements(GL_PATCHES, loaded, GL_UNSIGNED_INT, NULL);
    }
    glBindVertexArray(0);
}

void Mesh::Combine(Mesh* com)
{	
    GLuint lastIndex = Vertices.size();

    int t = Vertices.size();
    Vertices.resize(Vertices.size() + com->Vertices.size());
    for (size_t i =0; i < com->Vertices.size();i++)
    {
        Vertices[t] = com->Vertices[i];
        t++;
    }

    t = Indices.size();
    Indices.resize(Indices.size() + com->Indices.size());
    for (size_t i =0; i < com->Indices.size();i++)
    {
        Indices[t] = com->Indices[i] + lastIndex;
        t++;
    }
}

void Mesh::loadSTL(const std::string &patch) {
            VertPosNormTanBiTex vertex1;
            VertPosNormTanBiTex vertex2;
            VertPosNormTanBiTex vertex3;
            int phase = -1; //0 normal, 1 nothing, 2 3 4 vertex, 5 6 nothing
            int type = -1; //0 ascii, 1 binary
            Vertices.clear();
            Indices.clear();

            std::ifstream file;
            file.open(patch);

            std::string lh;
            std::getline(file, lh);
            if (lh.find("solid") != std::string::npos) {
                type = 0;
            }
            else {
                type = 1;
            }

            if (type == 0) {
                while (!file.eof()) {
                    std::string lineHeader;
                    std::getline(file, lineHeader);

                    if (lineHeader.find("facet normal ") != std::string::npos) {
                        phase = 0;
                    }

                    switch (phase) {
                        case 0:
                            sscanf(lineHeader.c_str(), "facet normal %g %g %g", &vertex1.normal.x, &vertex1.normal.y, &vertex1.normal.z);
                            break;
                        case 2:
                            sscanf(lineHeader.c_str(), "vertex %g %g %g", &vertex1.position.x, &vertex1.position.y, &vertex1.position.z);
                            break;
                        case 3:
                            sscanf(lineHeader.c_str(), "vertex %g %g %g", &vertex2.position.x, &vertex2.position.y, &vertex2.position.z);
                            break;
                        case 4:
                            sscanf(lineHeader.c_str(), "vertex %g %g %g", &vertex3.position.x, &vertex3.position.y, &vertex3.position.z);
                            vertex3.normal = vertex1.normal;
                            vertex2.normal = vertex1.normal;
                            Vertices.push_back(vertex1);
                            Vertices.push_back(vertex2);
                            Vertices.push_back(vertex3);
                            break;
                    }
                    phase++;
                }
                file.close();
            }
            else {
//                var bfile = File.OpenRead(path);
//                bfile.Read(new byte[80], 0, 80); //80b header
//                var reader32 = new byte[4];
//                bfile.Read(reader32, 0, 4);
//                uint ntri = BitConverter.ToUInt32(reader32, 0);
//                for (int i = 0; i < ntri; i++) {
//                    bfile.Read(reader32, 0, 4);
//                    vertex1.Normal.X = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex1.Normal.Y = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex1.Normal.Z = BitConverter.ToSingle(reader32, 0);

//                    bfile.Read(reader32, 0, 4);
//                    vertex1.Position.X = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex1.Position.Y = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex1.Position.Z = BitConverter.ToSingle(reader32, 0);

//                    bfile.Read(reader32, 0, 4);
//                    vertex2.Position.X = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex2.Position.Y = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex2.Position.Z = BitConverter.ToSingle(reader32, 0);
//                    vertex2.Normal = vertex1.Normal;

//                    bfile.Read(reader32, 0, 4);
//                    vertex3.Position.X = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex3.Position.Y = BitConverter.ToSingle(reader32, 0);
//                    bfile.Read(reader32, 0, 4);
//                    vertex3.Position.Z = BitConverter.ToSingle(reader32, 0);
//                    vertex3.Normal = vertex1.Normal;

//                    Verteces.Add(vertex1);
//                    Verteces.Add(vertex2);
//                    Verteces.Add(vertex3);

//                    bfile.Read(reader32, 0, 2);
//                }
//                bfile.Close();
            }

            for (size_t i = 0; i < Vertices.size(); i++) {
                Indices.push_back(i);
            }
        }

        float Mesh::FarestPoint() {
            float max = 0;
            for (size_t i = 0; i < Vertices.size(); i++) {
                auto t = glm::length(Vertices[i].position);
                if (max < t) {
                    max = t;
                }
            }
            return max;
        }

//        public bool saveSTL(string path) {
//            if (File.Exists(path)) {
//                File.Delete(path);
//            }
//            FileStream file = File.OpenWrite(path);
//            StreamWriter sr = new StreamWriter(file);

//            sr.WriteLine("solid {0}", path);
//            for (int i = 0; i < Verteces.Count; i += 3)
//            {

//                sr.WriteLine(string.Format(ifp, "{0} {1} {2} {3}", "facet normal", Verteces[i].Normal.X, Verteces[i].Normal.Y, Verteces[i].Normal.Z));
//                sr.WriteLine(string.Format(ifp, "{0}", "outer loop"));
//                sr.WriteLine(string.Format(ifp, "{0} {1} {2} {3}", "vertex", Verteces[i].Position.X, Verteces[i].Position.Y, Verteces[i].Position.Z));
//                sr.WriteLine(string.Format(ifp, "{0} {1} {2} {3}", "vertex", Verteces[i + 1].Position.X, Verteces[i + 1].Position.Y, Verteces[i + 1].Position.Z));
//                sr.WriteLine(string.Format(ifp, "{0} {1} {2} {3}", "vertex", Verteces[i + 2].Position.X, Verteces[i + 2].Position.Y, Verteces[i + 2].Position.Z));
//                sr.WriteLine(string.Format(ifp, "{0}", "endloop"));
//                sr.WriteLine(string.Format(ifp, "{0}", "endfacet"));
//            }
//            sr.WriteLine("endsolid {0}", path);

//            sr.Flush();
//            sr.Close();
//            file.Close();
//            return true;
//        }

void Mesh::BuildBounding()
{
    if(Vertices.size() > 0){
        maxBound = minBound = Vertices[0].position;
        for(auto i: Vertices){
            if(i.position.x > maxBound.x){
                maxBound.x = i.position.x;
            }
            if(i.position.x < minBound.x){
                minBound.x = i.position.x;
            }

            if(i.position.z > maxBound.z){
                maxBound.z = i.position.z;
            }
            if(i.position.z < minBound.z){
                minBound.z = i.position.z;
            }

            if(i.position.y > maxBound.y){
                maxBound.y = i.position.y;
            }
            if(i.position.y < minBound.y){
                minBound.y = i.position.y;
            }
        }
    }
}
