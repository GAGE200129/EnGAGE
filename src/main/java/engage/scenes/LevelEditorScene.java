package engage.scenes;

import components.Sprite;
import components.SpriteRenderer;
import components.SpriteSheet;
import engage.Camera;
import engage.GameObject;
import engage.Transform2D;
import imgui.ImGui;
import org.joml.Vector2f;
import org.joml.Vector4f;
import util.AssetPool;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;


public class LevelEditorScene extends Scene {

    private  GameObject go, go2;
    private SpriteSheet sprites;

    @Override
    public void init() {
        this.camera = new Camera(new Vector2f(0, 0));


        loadResources();

        sprites = AssetPool.getSpriteSheet("assets/images/spritesheet.png");

        go = new GameObject("Dummy", new Transform2D(new Vector2f(10, 10), new Vector2f(100, 100)), 2);
        go2 = new GameObject("Dummy2", new Transform2D(new Vector2f(50, 10), new Vector2f(100, 100)), -1);
        go.addComponent(new SpriteRenderer(new Vector4f(1, 0, 0, 1)));
        go2.addComponent(new SpriteRenderer(new Sprite(AssetPool.getTexture2D("assets/images/blendImage2.png"))));
        addGameObjectToScene(go);
        addGameObjectToScene(go2);
        this.activeGO = go;

    }


    @Override
    public void update(float dt) {

        this.renderer.render();

        for (GameObject go : this.gameObjects) {
            go.update(dt);
        }
    }
    @Override
    public void imGui() {

        ImGui.begin("Yeet");
        ImGui.text("Fucking hell");
        ImGui.end();
    }

    private void loadResources() {

        AssetPool.getShader("assets/shaders/default.glsl");
        AssetPool.getTexture2D("assets/images/testImage.png");
        AssetPool.getTexture2D("assets/images/testImage2.png");
        AssetPool.addSpriteSheet("assets/images/spritesheet.png", new SpriteSheet(AssetPool.getTexture2D("assets/images/spritesheet.png"), 16, 16, 26, 0));

    }
}
