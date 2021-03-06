#pragma once

class Scene;

class TitleScene : public Scene {

public:
    TitleScene();
    TitleScene(SceneNum num, GameClient* const pGameClient);
    ~TitleScene();

    void InitScene() override;

    void Update(float fTimeElapsed) override;

    void Paint(HDC hDC) override;

    void KeyDown(unsigned char KEYCODE) override;
    void KeyUp(unsigned char KEYCODE) override;
};
