#ifndef SCENE_H
#define SCENE_H

#include <d3d12.h>

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void LoadResources(ID3D12Device* device) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};

#endif // SCENE_H
