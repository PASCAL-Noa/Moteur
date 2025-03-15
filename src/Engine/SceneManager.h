#pragma once

#include "Scene.h"

class SceneManager {
public:
    SceneManager() = default;

    void AddScene(const std::string& name, const std::shared_ptr<Scene>& scene);
    bool SetActiveScene(const std::string& name, ID3D12Device* device);
    void Update(float deltaTime);
    std::string GetSceneName() const { return sceneName.empty() ? "No Scene" : sceneName; }
    void SetWindowName(float fps, HWND wnd) const;

    template<typename T>
    void Render(T& renderable) {
        if (m_activeScene)
            renderable.Run();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_activeScene;
    std::string sceneName;
};