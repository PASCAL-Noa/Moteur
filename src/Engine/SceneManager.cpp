#include "pch.h"
#include "SceneManager.h"

void SceneManager::AddScene(const std::string& name, const std::shared_ptr<Scene>& scene) {
    m_scenes.emplace(name, scene);
}

bool SceneManager::SetActiveScene(const std::string& name, ID3D12Device* device) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        m_activeScene = it->second;
        sceneName = name;

        if (m_activeScene) {
            m_activeScene->LoadResources(device);
            return true;
        }
    }
    else {
        std::cerr << "Error: Scene '" << name << "' not found!" << std::endl;
    }
    return false;
}

void SceneManager::Update(float deltaTime) {
    if (m_activeScene) {
        m_activeScene->Update(deltaTime);
    }
}

void SceneManager::SetWindowName(float fps, HWND wnd) const {
    std::string displaySceneName = sceneName.empty() ? "" : sceneName;
    std::string windowTitle = "Scene: " + displaySceneName + " | " + std::to_string(fps) + " FPS";
    SetWindowTextA(wnd, windowTitle.c_str());
}