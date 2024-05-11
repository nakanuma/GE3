#pragma once
#include "Object3D.h"

class OutlinedObject:public Object3D
{
public:
	OutlinedObject();

	void UpdateMatrix();
	
	void Draw();

	bool enableOutline = true;
private:
	Object3D outline_;
};

