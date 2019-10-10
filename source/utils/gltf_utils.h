#pragma once
#include "base.h"

struct GltfNode
{
	int node_id;
	int parent;
	std::vector<int> childrens;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::quat rotation;
};

struct GltfMesh
{
	
};

struct GltfScene
{
	std::map<int, GltfNode> nodes;
};

GltfScene* load_gltf_scene(std::string file);

void delete_gltf_scene(GltfScene* scene);

//��ȡ�ڵ�ľֲ�����
glm::mat4 get_local_matrix(GltfScene* scene, int id);

//��ȡ�ڵ���������
glm::mat4 get_world_matrix(GltfScene* scene, int id);