#include "OctSpacialPartition.h"

int OctNode::childNum = 0;

OctNode::OctNode(vec3 position_, float size_, OctNode* parent_) : octBounds(nullptr), parent(nullptr), children(), objectList(vector<GameObject*>())
{
	objectList.reserve(10);

	octBounds = new BoundingBox();
	octBounds->minVert = position_;
	octBounds->maxVert = position_ + vec3(size_);

	size = size_;
	parent = parent_;

	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		children[i] = nullptr;
	}
}

OctNode::~OctNode()
{
	delete octBounds;
	octBounds = nullptr;

	if (objectList.size() > 0)
	{
		for (auto obj : objectList)
		{
			obj = nullptr;
		}
		objectList.clear();
	}

	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		if (children[i] != nullptr)
		{
			delete children[i];
		}
		children[i] = nullptr;
	}
}

void OctNode::Octify(int depth_)
{
	if (depth_ > 0 && this)
	{
		float half = size / 2.0f;
		children[static_cast<int>(OctChildren::OCT_TLF)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_BLF)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_BRF)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_TRF)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_TLR)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z), half, this);
		children[static_cast<int>(OctChildren::OCT_BLR)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[static_cast<int>(OctChildren::OCT_BRR)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[static_cast<int>(OctChildren::OCT_TRR)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		childNum += 8;
	}

	if (depth_ > 0 && this)
	{
		for (int i = 0; i < CHILDREN_NUMBER; i++)
		{
			children[i]->Octify(depth_ - 1);
		}
	}
}

void OctNode::AddCollisionObject(GameObject* obj_)
{
	objectList.push_back(obj_);
}

bool OctNode::IsLeaf() const
{
	if (children[0] == nullptr)
	{
		return true;
	}
	return false;
}

OctSpacialPartition::OctSpacialPartition(float worldSize_) : root(nullptr), rayIntersectionList(vector<OctNode*>())
{
	root = new OctNode(vec3(-(worldSize_ / 2.0f)), worldSize_, nullptr);
	root->Octify(3);
	cout << "There are " << root->GetChildCount() << " child nodes" << endl;

	rayIntersectionList.reserve(20);
}

OctSpacialPartition::~OctSpacialPartition()
{
	if (rayIntersectionList.size() > 0)
	{
		for (auto cell : rayIntersectionList)
		{
			cell = nullptr;
		}
		rayIntersectionList.clear();
	}

	delete root;
	root = nullptr;
}

void OctSpacialPartition::AddObject(GameObject* obj_)
{
	AddObjectToCell(root, obj_);
}

GameObject* OctSpacialPartition::GetCollision(Ray ray_)
{
	if (rayIntersectionList.size() > 0)
	{
		for (auto cell : rayIntersectionList)
		{
			cell = nullptr;
		}
		rayIntersectionList.clear();
	}

	PrepareCollisionQuery(root, ray_);
	GameObject* result = nullptr;
	float shortestDistance = FLT_MAX;

	for (auto cell : rayIntersectionList)
	{
		for (auto obj : cell->objectList)
		{
			if (ray_.IsColliding(&obj->GetBoundingBox()))
			{
				if (ray_.intersectionDist < shortestDistance)
				{
					result = obj;
					shortestDistance = ray_.intersectionDist;
				}
			}
		}

		if (result != nullptr)
		{
			return result;
		}
	}

	return nullptr;
}

void OctSpacialPartition::AddObjectToCell(OctNode* cell_, GameObject* obj_)
{
	if (cell_->IsLeaf() == true)
	{
		if (cell_->GetBoundingBox()->Intersects(&obj_->GetBoundingBox()))
		{
			cell_->objectList.push_back(obj_);
		}
	}
	else if (cell_->IsLeaf() == false)
	{
		for (int i = 0; i < cell_->childNum; i++)
		{
			AddObjectToCell(cell_->children[i], obj_);
		}
	}
}

void OctSpacialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	if (cell_->IsLeaf() == true)
	{
		if (ray_.IsColliding(cell_->GetBoundingBox()))
		{
			rayIntersectionList.push_back(cell_);
		}
	}
	else if (cell_->IsLeaf() == false)
	{
		for (int i = 0; i < cell_->childNum; i++)
		{
			PrepareCollisionQuery(cell_->children[i], ray_);
		}
	}
}