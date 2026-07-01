class BasicVM {
public:
    // ...
private:
    Video &video;
    Input &input;
    Engine2D &engine2d;
    Engine3D &engine3d;
    CompositeColor &color;

    int worldW = 64;
    int worldH = 32;
    int px = 32;
    int py = 16;
    int blockSel = 1;

    void runMinecraftFrame();
    void handleInput();
};
