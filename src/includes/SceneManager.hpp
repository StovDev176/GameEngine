#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include "EventBus.hpp"
#include "ECS.hpp"
#include "TaskScheduler.hpp"


class Scene {
    public:
    virtual ~Scene() = default;
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
};

class SceneManager {
    private:
    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<Scene> nextScene;
    public:

    void ChangeScene(std::unique_ptr<Scene> newScene) {
        nextScene = std::move(newScene);
    }

    void ProcessSceneChange() {
        if (!nextScene) {return;}
        if (currentScene) {
            currentScene->OnExit();
        }
        currentScene = std::move(nextScene);
        currentScene->OnEnter();
    }
    void Update(float dt) {
        if (currentScene) {
            currentScene->Update(dt);
        }
    }
    void Render() {
        if (currentScene) {
            currentScene->Render();
        }
    }
};

class GameScene : public Scene {
private:
    TaskScheduler& taskScheduler;
public:
    GameScene(TaskScheduler& taskScheduler) : taskScheduler(taskScheduler) {}

    void OnEnter() override {
        taskScheduler.start();
    }
    void OnExit() override {
        taskScheduler.stop();
    }
};
