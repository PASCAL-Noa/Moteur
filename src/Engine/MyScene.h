#ifndef MYSCENE_H
#define MYSCENE_H

#include "Scene.h"

class MyScene : public Scene
{
public:
	void RunScene()
	{
		std::cout << "MyScene is running\n";
	}

    void LoadResources(ID3D12Device* device) override
    {
        // Load Window Name
    }

    void Update(float deltaTime) override
    {

    }

    void Render() override
    {
        
    }
};

#endif 