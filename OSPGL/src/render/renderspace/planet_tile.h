#pragma once
#include <vector>
#include "../../orbital/planet.h"
#include "quad_tree_node.h"
#include "../../util/math_util.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/normal.hpp>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "../renderlow/mesh.h"
#include "surface_provider.h"
#include "planet_tile_path.h"

struct PlanetTile
{
	// Is the tile being generated by a worker thread
	bool is_being_generated;
	bool is_generated;
	bool needs_upload;

	PlanetTilePath path;

	size_t vert_count;
	Planet& planet;

	std::vector<float> verts;
	std::vector<uint16_t> indices;
	// Up, right, down, left
	std::vector<uint16_t> tolower[4];
	// Up, right, down, left
	std::vector<uint16_t> tosame[4];

	bool used;

	GLuint vbo, vao, ebo;
	GLuint tolower_vao[4], tolower_ebo[4];
	GLuint tosame_vao[4], tosame_ebo[4];

	// Up, right, down, left, like always
	bool needs_lower[4];

	bool isUploaded();
	// Any combination of sides may be a lower quality (TODO)
	void upload();
	void unload();

	void generate();

	void generate_vertex(int ix, int iy, size_t vertCount,
		std::vector<float>& heights, glm::mat4 model, glm::mat4 inverse_model_spheric,
		std::vector<Vertex>& target, float planet_radius);

	void generate_normal(size_t i, std::vector<uint16_t>& indices, std::vector<float>& verts, size_t FLOATS_PER_VERTEX,
		glm::mat4 model_spheric);

	PlanetTile(PlanetTilePath nPath, size_t verticesPerSide, Planet& planet, bool generate_now = false);
};
