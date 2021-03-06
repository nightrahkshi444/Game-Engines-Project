#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include <sstream>
#include "Mesh.h"
#include "../../Math/BoundingBox.h"
#include "../../FX/MaterialLoader.h"

class LoadOBJModel
{
public:
	LoadOBJModel();
	~LoadOBJModel();

	void LoadModel(const string& objFilePath_, const string& mtlFilePath_);
	vector<SubMesh> GetSubMeshes();

	inline BoundingBox GetBoundingBox() const { return boundingBox; };

private:
	void PostProcessing();
	void LoadModel(const string& filePath_);
	void LoadMaterial(const string& matName_);
	void LoadMaterialLibrary(const string& matFilePath_);

	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec2> textureCoords;
	vector<unsigned int> indices, normalIndices, textureIndices;
	vector <Vertex> meshVertices;
	vector <SubMesh> subMeshes;

	Material currentMaterial;
	BoundingBox boundingBox;
};

#endif