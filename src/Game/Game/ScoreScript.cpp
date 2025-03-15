#include "pch.h"
#include "ScoreScript.h"

void ScoreScript::Init()
{
    std::cout << "[DEBUG] Score System Initialized" << std::endl;
    score = 0;
    survivalTime = 0.0f;
}

void ScoreScript::Update(float deltaTime)
{
    survivalTime += deltaTime;
    score = static_cast<int>(survivalTime * 10);

    //std::cout << "[SCORE] Score : " << score << std::endl;
}

void ScoreScript::AddPoints(int points)
{
    score += points;
    std::cout << "[SCORE] Gagné : " << points << " points ! Score total : " << score << std::endl;
}

int ScoreScript::GetScore() const
{
    return score;
}
