#include "Model.h"

Model::Model(const string& objPath_, const string& matPath_, GLuint shaderProgram_) : meshes(vector<Mesh*>()), shaderProgram(0), modelInstances(vector<mat4>()), obj(nullptr)
{
	shaderProgram = shaderProgram_;
	meshes.reserve(10);
	modelInstances.reserve(5);
	obj = new LoadOBJModel();
	obj->LoadModel(objPath_, matPath_);
	LoadModel();
}

Model::~Model()
{
	if (meshes.size() > 0)
	{
		for (auto m : meshes)
		{
			delete m;
			m = nullptr;
		}
		meshes.clear();
	}

	if (modelInstances.size() > 0)
	{
		modelInstances.clear();
	}
}

void Model::Render(Camera* camera_)
{
	// Tells openGL which shader program to use
	glUseProgram(shaderProgram);
	for (auto m : meshes)
	{
		m->Render(camera_, modelInstances);
	}
}

void Model::AddMesh(Mesh* mesh_)
{
	meshes.push_back(mesh_);
}

unsigned int Model::CreateInstance(vec3 pos_, float angle_, vec3 rotation_, vec3 scale_)
{
	modelInstances.push_back(CreateTransform(pos_, angle_, rotation_, scale_));
	return modelInstances.size() - 1;
}

void Model::UpdateInstance(unsigned int index_, vec3 pos_, float angle_, vec3 rotation_, vec3 scale_)
{
	modelInstances[index_] = CreateTransform(pos_, angle_, rotation_, scale_);
}

mat4 Model::GetTransform(unsigned int index_) const
{
	return modelInstances[index_];
}

mat4 Model::CreateTransform(vec3 pos_, float angle_, vec3 rotation_, vec3 scale_) const
{
	mat4 model;
	model = translate(model, pos_);
	model = rotate(model, angle_, rotation_);
	model = scale(model, scale_);
	return model;
}

void Model::LoadModel()
{
	for (int i = 0; i < obj->GetSubMeshes().size(); i++)
	{
		meshes.push_back(new Mesh(obj->GetSubMeshes()[i], shaderProgram));
	}
	delete obj;
	obj = nullptr;
}