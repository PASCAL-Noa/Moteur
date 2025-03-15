#include "pch.h"
#include "ParticleSystem.h"
#include <algorithm>
#include "../Components/MeshRenderer.h"
#include "../Systems/RenderSystem.h"

ParticleSystem::ParticleSystem(Manager* manager) : 
    mManager(manager) {
}

void ParticleSystem::Init() {

}

const float FIXED_TIME_STEP = 1.0f / 90.f; // Target 60 FPS

void ParticleSystem::Update(float deltaTime) {
    float adjustedDeltaTime = deltaTime;
    if (adjustedDeltaTime > FIXED_TIME_STEP) {
        adjustedDeltaTime = FIXED_TIME_STEP;
    }

    for (auto it = particles.begin(); it != particles.end();) {
        Particle& particle = *it;

        if (particle.type == ParticleType::EXPLOSION) {
            particle.entity->transform.Translate(
                particle.velocity.x * adjustedDeltaTime,
                particle.velocity.y * adjustedDeltaTime,
                particle.velocity.z * adjustedDeltaTime
            );

            particle.scale *= 0.95f;
            particle.entity->transform.SetScale(particle.scale, particle.scale, particle.scale);

            if (particle.scale <= 0.05f) {
                particle.entity->Destroy();
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }

        else if (particle.type == ParticleType::TRAIL) {
            particle.entity->transform.Translate(
                particle.velocity.x * adjustedDeltaTime,
                particle.velocity.y * adjustedDeltaTime,
                particle.velocity.z * adjustedDeltaTime - 0.5f
            );

            particle.scale *= 0.95f;
            particle.entity->transform.SetScale(particle.scale, particle.scale, particle.scale);

            if (particle.scale <= 0.05f) {
                particle.entity->Destroy();
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

//                                                         pos (center)     time alive        texture       numberOfParticles
void ParticleSystem::CreateParticles(const DirectX::XMFLOAT3& position, float size, float lenght, int textureIndex, int partiNumber, ParticleType _type) {
    for (int i = 0; i < partiNumber; ++i) {
        Entity* particleEntity = &mManager->CreateEntity();

        mManager->AddComponent(*particleEntity, new MeshRenderer());
        mManager->GetComponent<MeshRenderer>(*particleEntity).textureIndex = textureIndex;
        mManager->GetSystem<RenderSystem>().AddMesh(particleEntity, "particules", MeshProperties::CreateSphere(size, 16, 16));

        float theta = static_cast<float>(rand()) / RAND_MAX * DirectX::XM_2PI;
        float phi = static_cast<float>(rand()) / RAND_MAX * DirectX::XM_PI; 
        float speed = 3.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f; 

        float randomX = speed * sinf(phi) * cosf(theta);
        float randomY = speed * cosf(phi);
        float randomZ = speed * sinf(phi) * sinf(theta);

        particleEntity->transform.SetPosition(position.x, position.y, position.z);
        particleEntity->transform.SetPosition(position.x, position.y, position.z);
        particleEntity->transform.SetScale(1.f, 1.f, 1.f);

        Particle particle = {
            particleEntity,
            lenght,
            1.f,
            {randomX, randomY, randomZ},
            _type
        };

        particles.push_back(particle);
    }
}
