#pragma once

#include "../ECS.h"

enum class ParticleType {
    EXPLOSION,
    TRAIL      
};

class ParticleSystem : public System {
public:
    ParticleSystem(Manager* manager);

    void Init() override;
    void Update(float deltaTime) override;
    void CreateParticles(const DirectX::XMFLOAT3& position, float size, float lenght, int textureIndex, int partiNumber, ParticleType _type);
    void DeleteParticles();

private:
    struct Particle {
        Entity* entity;
        float lifeTime;
        float scale;
        DirectX::XMFLOAT3 velocity;
		ParticleType type;
    };

    std::vector<Particle> particles;
    Manager* mManager;
};
