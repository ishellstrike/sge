/*******************************************************************************
		Copyright (C) 2015 Samsonov Andrey

		This software is distributed freely under the terms of the MIT LICENSE.
		See "LICENSE.txt"
*******************************************************************************/

#include "quadplane.h"
#include <glm/gtx/transform.hpp>
#include "..\ClassicNoise.h"
#include "..\TextureGenerator.h"
#include "..\prefecences.h"
#include <thread>
#include "scattering.h"

QuadPlane::QuadPlane()
{
}

int GetLevel()
{
	return 1;
}

bool QuadPlane::is_terminal() const
{
	return m_parts[0] == nullptr || m_parts[1] == nullptr || m_parts[2] == nullptr || m_parts[3] == nullptr;
}

void CopyTex(std::shared_ptr<Material> &t, std::shared_ptr<Texture> height_map, std::shared_ptr<Texture> grad_map, const glm::vec4 & part)
{
	if (!t->height || !t->grad)
		return;

	TextureGenerator tg;
	tg.SetShader(Resources::instance()->Get<BasicJargShader>("tex_copy"));
	tg.AddTexture("source_map", t->height);
	tg.SetParams(part.x);
	tg.SetParams(part.y);
	tg.SetParams(part.z);
	tg.SetParams(part.w);
	tg.SetResultTexture(height_map);
	tg.RenderOnTempFbo();

	TextureGenerator tg2;
	tg2.SetShader(Resources::instance()->Get<BasicJargShader>("tex_copy"));
	tg2.AddTexture("source_map", t->grad);
	tg2.SetParams(part.x);
	tg2.SetParams(part.y);
	tg2.SetParams(part.z);
	tg2.SetParams(part.w);
	tg2.SetResultTexture(grad_map);
	tg2.RenderOnTempFbo();
}


void QuadPlane::GenTex(SphereParamsStorage *parent, std::shared_ptr<Texture> height_map, std::shared_ptr<Texture> grad_map, float _res, const glm::vec4 & part)
{
	TextureGenerator tg;
	tg.SetShader(parent->height_shader);
	tg.AddTexture("samplerPerlinPerm2D", parent->noise_map);
	tg.AddTexture("samplerPerlinGrad2D", parent->grad_map);
	tg.SetParams(static_cast<float>(offset.x) - (2 / _res)*scale);
	tg.SetParams(static_cast<float>(offset.y) - (2 / _res)*scale);
	tg.SetParams(scale + (4 / _res)*scale);
	tg.SetResultTexture(height_map);
	glScissor(height_map->width * part.x, 
			  height_map->height * part.y, 
		      height_map->width * part.z,
		      height_map->height * part.w);
	glEnable(GL_SCISSOR_TEST);
	tg.RenderOnTempFbo();

	TextureGenerator grad_gen;
	grad_gen.SetShader(parent->grad_shader);
	grad_gen.AddTexture("height_map", height_map);
	grad_gen.SetResultTexture(grad_map);
	grad_gen.SetParams(static_cast<float>(height_map->width));
	grad_gen.SetParams(static_cast<float>(height_map->height));
	grad_gen.SetParams(scale);
	grad_gen.RenderOnTempFbo();
	glDisable(GL_SCISSOR_TEST);
}

const float res = 512.0f;
void QuadPlane::GenerateSubTexture(std::shared_ptr<Material> &t, SphereParamsStorage *parent, float devider, bool skip, const glm::vec4 & part)
{
	float _res = res / devider;
	std::shared_ptr<Texture> height_map = std::make_shared<Texture>(glm::vec2(_res), true, false, GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
	std::shared_ptr<Texture> grad_map = std::make_shared<Texture>(glm::vec2(_res), true);

	t->height = height_map;
	t->grad = grad_map;
}

void QuadPlane::Render(const Camera &cam, int side, SphereParamsStorage *parent)
{
	if (is_terminal())
	{
		parent->basic->Use();
		//float distance = glm::distance(cam.Position(), parent->center);
		parent->basic->SetUniform("R", parent->R);
		parent->basic->SetUniform("S", parent->s);

		parent->basic->SetUniform("c", cam.Position());
		parent->basic->SetUniform("projInverse", glm::inverse(cam.Projection()));
		parent->basic->SetUniform("viewInverse", glm::inverse(cam.View()));

		parent->basic->SetUniform("reflectanceSampler", 10);
		parent->basic->SetUniform("transmittanceSampler", 11);
		parent->basic->SetUniform("irradianceSampler", 12);
		parent->basic->SetUniform("inscatterSampler", 13);

		glActiveTexture(GL_TEXTURE0 + 10);
		glBindTexture(GL_TEXTURE_2D, Scattering::instance().reflectanceTexture);
		glActiveTexture(GL_TEXTURE0 + 11);
		glBindTexture(GL_TEXTURE_2D, Scattering::instance().transmittanceTexture);
		glActiveTexture(GL_TEXTURE0 + 12);
		glBindTexture(GL_TEXTURE_2D, Scattering::instance().irradianceTexture);
		glActiveTexture(GL_TEXTURE0 + 13);
		glBindTexture(GL_TEXTURE_2D, Scattering::instance().inscatterTexture);

		if (status == READY || status == PRE_READY)
		{
			terminal_mesh->Render(cam, parent->world);
			float pre_size = 222.f;
			//texture vis
			//WinS::ws->sb->drawQuad(glm::vec2(10,10) + pre_size*offset + glm::vec2(pre_size,0)*(float)(side/2) + glm::vec2(0,pre_size)*(float)(side%2), glm::vec2(pre_size)*scale, *terminal_mesh->material->grad, Color::White);
			//quadtree vis
			//WinS::ws->sb->drawRect(glm::vec2(10,10) + pre_size*offset + glm::vec2(pre_size,0)*(float)(side/2) + glm::vec2(0,pre_size)*(float)(side%2), glm::vec2(pre_size)*scale, glm::vec4(rand()%255/255.0,rand()%255/255.0,rand()%255/255.0,1 ));
		}
		else if (status == TEXTURE_READY || status == TEXTURE_PRE_READY)
		{
			int size = parent->tess_size;
			terminal_mesh->indices.reserve(size * size * 6);
			terminal_mesh->vertices.reserve((size + 1) * (size + 1));
			int co = 0;

			float xs = (-0.5f + offset.x); /*< x координата начала сектора сферы с отступом*/
			float ys = (-0.5f + offset.y); /*< y координата начала сектора сферы с отступом*/
			float dd = ((1.0f * scale) / static_cast<float>(size)); /*< размер 1 квада сектора сферы*/

			//Генерация R=1 сферы. Нормализуемые плоскости имеют координаты [-0.5, 0.5]. В шейдере сфера приводится к радиусу R

			terminal_mesh->vertices.reserve((size + 3)*(size + 3));
			for (int j = -1; j < size + 2; j++)
			{
				for (int i = -1; i < size + 2; i++)
				{
					VertexType a;
					a.position = { xs + i * dd, ys + j * dd, 0.5f };

					if (i == -1)
						a.position.x = xs - 1 / 200.f * scale;
					if (j == -1)
						a.position.y = ys - 1 / 200.f * scale;

					if (i == size + 1)
						a.position.x = xs + i * dd + 1 / 200.f * scale;
					if (j == size + 1)
						a.position.y = ys + j * dd + 1 / 200.f * scale;

					a.position = glm::normalize(a.position);
					if (j == -1 || i == -1 || j == size + 1 || i == size + 1)
						a.position *= 0.999f;

					a.uv = { i * ((1.0 - 4 / res) / (float)size) + 2 / res, j * ((1.0f - 4 / res) / (float)size) + 2 / res };

					a.uv_glob = { xs + i * dd + 0.5f, ys + j * dd + 0.5f };

					terminal_mesh->vertices.push_back(a);
				}
			}


			//трансформация вершин сферы в соответствии с матрицей трансформации, заданной при создании объекта сферы
			//матрица трансформации разворачивает части сферы в соответствующие стороны
			for (size_t j = 0; j < terminal_mesh->vertices.size(); j++)
			{
				terminal_mesh->vertices[j].position =
					glm::vec3(transformation *
						glm::vec4(terminal_mesh->vertices[j].position, 1));
			}

			subsurface_centers[0] = glm::vec3(transformation *
				glm::normalize(glm::vec4(xs + dd * size / 4.0,
					ys + dd * size / 4.0,
					0.5,
					0)
					)
				);
			subsurface_centers[1] = glm::vec3(transformation *
				glm::normalize(glm::vec4(xs + dd * size / (3 / 4.0),
					ys + dd * size / 4.0,
					0.5,
					0)
					)
				);
			subsurface_centers[2] = glm::vec3(transformation *
				glm::normalize(glm::vec4(xs + dd * size / 4.0,
					ys + dd * size / (3 / 4.0),
					0.5,
					0)
					)
				);
			subsurface_centers[3] = glm::vec3(transformation *
				glm::normalize(glm::vec4(xs + dd * size / (3 / 4.0),
					ys + dd * size / (3 / 4.0),
					0.5,
					0)
					)
				);

			terminal_mesh->indices = parent->indices;
			terminal_mesh->ComputeAABB(&VertexType::position);
			terminal_mesh->ForgetBind();
			terminal_mesh->primitives = GL_PATCHES;

			//terminal_mesh->BindExistingIBO(parent->ibo, parent->Indeces.size());
			status = PRE_READY;
			terminal_mesh->Render(cam, parent->world);
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			m_parts[i]->Render(cam, side, parent);
	}

}

void QuadPlane::Update(const Camera &camera, float Rs, float eps, int max_divide, SphereParamsStorage *sps)
{
	glm::vec3 cent[4];
	for (int i = 0; i < 4; i++)
	{
		cent[i] = glm::vec3(sps->world * glm::vec4(subsurface_centers[i] + sps->center, 0));
	}

	if (status == ERROR)
	{
		sub_texture = std::make_shared<Material>(*sps->mat);
		GenerateSubTexture(sub_texture, sps, 1, true);
		sub_texture->texture = sps->mat->texture;
		sub_texture->global_height = sps->mat->global_height;

		terminal_mesh->material = sub_texture;
		terminal_mesh->shader = sps->basic;

		if(parent)
			CopyTex(parent->sub_texture, sub_texture->height, sub_texture->grad, glm::vec4(glm::vec2(part)*glm::vec2(0.5, 0.5), 0.5, 0.5));

		status = TEXTURE_PRE_READY;
		return;
	}

	if (status == PRE_READY)
	{
		int ddim = 4;
		int of = int(done * ddim * ddim);
		glm::vec4 a = { (of % ddim)/float(ddim), (of / ddim)/float(ddim), 1/float(ddim*.95), 1 / float(ddim*.95) };
		GenTex(sps, sub_texture->height, sub_texture->grad, res, a);

		done += 1 / float(ddim) / float(ddim);

		if (done >= 1)
		{
			status = READY;
		}

		return;
	}


	if (is_terminal() &&
		(glm::distance(cent[0] * Rs, camera.Position()) < eps * scale ||
			glm::distance(cent[1] * Rs, camera.Position()) < eps * scale ||
			glm::distance(cent[2] * Rs, camera.Position()) < eps * scale ||
			glm::distance(cent[3] * Rs, camera.Position()) < eps * scale)
		&& level < max_divide)
	{
		m_parts[0] = std::make_shared<QuadPlane>();
		m_parts[0]->terminal_mesh = std::make_shared< UMesh<VertexType> >();
		m_parts[0]->offset = offset;
		m_parts[0]->scale = scale / 2.0f;
		m_parts[0]->level = level + 1;
		m_parts[0]->transformation = transformation;
		m_parts[0]->parent = this;
		m_parts[0]->part = {0, 0};

		m_parts[1] = std::make_shared<QuadPlane>();
		m_parts[1]->terminal_mesh = std::make_shared< UMesh<VertexType> >();
		m_parts[1]->offset = offset + glm::vec2(0.5f, 0) * scale;
		m_parts[1]->scale = scale / 2.0f;
		m_parts[1]->level = level + 1;
		m_parts[1]->transformation = transformation;
		m_parts[1]->parent = this;
		m_parts[1]->part = { 1, 0 };

		m_parts[2] = std::make_shared<QuadPlane>();
		m_parts[2]->terminal_mesh = std::make_shared< UMesh<VertexType> >();
		m_parts[2]->offset = offset + glm::vec2(0, 0.5f) * scale;
		m_parts[2]->scale = scale / 2.0f;
		m_parts[2]->level = level + 1;
		m_parts[2]->transformation = transformation;
		m_parts[2]->parent = this;
		m_parts[2]->part = { 0, 1 };

		m_parts[3] = std::make_shared<QuadPlane>();
		m_parts[3]->terminal_mesh = std::make_shared< UMesh<VertexType> >();
		m_parts[3]->offset = offset + glm::vec2(0.5f, 0.5f) * scale;
		m_parts[3]->scale = scale / 2.0f;
		m_parts[3]->level = level + 1;
		m_parts[3]->transformation = transformation;
		m_parts[3]->parent = this;
		m_parts[3]->part = { 1, 1 };

		//status = ERROR;
	}
	else
		if ((glm::distance(cent[0] * Rs, camera.Position()) > eps * 1.1f * scale &&
			glm::distance(cent[1] * Rs, camera.Position()) > eps * 1.1f * scale &&
			glm::distance(cent[2] * Rs, camera.Position()) > eps * 1.1f * scale &&
			glm::distance(cent[3] * Rs, camera.Position()) > eps * 1.1f * scale))
		{
			for (int i = 0; i < 4; ++i)
				m_parts[i] = nullptr;
		}

	if (!is_terminal())
	{
		for (int i = 0; i < 4; ++i)
			m_parts[i]->Update(camera, Rs, eps, max_divide, sps);
	}
}

