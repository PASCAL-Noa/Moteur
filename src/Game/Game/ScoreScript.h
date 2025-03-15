#pragma once
#include "../../Engine/ECS/ECS.h"

class ScoreScript : public Script
{
public:
    void Init() override;
    void Update(float deltaTime) override;

    void AddPoints(int points);
    int GetScore() const;


private:
    Manager* mManager;
    int score = 0;
    float survivalTime = 0.0f;
};
