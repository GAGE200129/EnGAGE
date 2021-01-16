package engage.scenes;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import components.Sprite;
import components.SpriteRenderer;
import components.SpriteSheet;
import engage.*;
import imgui.ImGui;
import org.joml.Vector2f;
import org.joml.Vector4f;
import util.AssetPool;


public class LevelEditorScene extends Scene {

    private  GameObject go, go2;
    private SpriteSheet sprites;

    @Override
    public void init() {
        this.camera = new Camera(new Vector2f(0, 0));
        if(levelLoaded) {
            return;
        }

        loadResources();

        sprites = AssetPool.getSpriteSheet("assets/images/spritesheet.png");

        go = new GameObject("Dummy", new Transform2D(new Vector2f(10, 10), new Vector2f(100, 100)), 2);
        go2 = new GameObject("Dummy2", new Transform2D(new Vector2f(50, 10), new Vector2f(100, 100)), -1);
        SpriteRenderer goSprite = new SpriteRenderer();
        SpriteRenderer go1Sprite = new SpriteRenderer();
        Sprite go1SpriteImg = new Sprite();
        go1SpriteImg.setTexture(AssetPool.getTexture2D("assets/images/blendImage2.png"));
        goSprite.setColor(new Vector4f(1, 0, 0, 1));
        go1Sprite.setSprite(go1SpriteImg);


        go.addComponent(goSprite);
        go2.addComponent(go1Sprite);
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
