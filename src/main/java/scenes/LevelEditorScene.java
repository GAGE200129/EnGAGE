package scenes;

import components.*;
import engage.Camera;
import engage.GameObject;
import engage.Prefabs;
import engage.Transform2D;
import imgui.ImGui;
import imgui.ImVec2;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;
import renderer.DebugDraw;
import util.AssetPool;


public class LevelEditorScene extends Scene {

    private SpriteSheet sprites;

    GameObject levelEditor = new GameObject("LevelEditor", new Transform2D(new Vector2f()), 0);

    @Override
    public void init() {
        this.levelEditor.addComponent(new MouseControls());
        this.levelEditor.addComponent(new GridLines());
        this.camera = new Camera(new Vector2f(-250, 0));
        loadResources();
        sprites = AssetPool.getSpriteSheet("assets/images/spritesheets/decorationsAndBlocks.png");
        if (levelLoaded) {
            return;
        }


    }


    @Override
    public void update(float dt) {
        levelEditor.update(dt);

        this.renderer.render();
        for (GameObject go : this.gameObjects) {
            go.update(dt);
        }
    }

    @Override
    public void imGui() {

        ImGui.begin("Yeet");
        ImVec2 windowPos = new ImVec2();
        ImGui.getWindowPos(windowPos);
        ImVec2 windowSize = new ImVec2();
        ImGui.getWindowSize(windowSize);
        ImVec2 itemSpacing = new ImVec2();
        ImGui.getStyle().getItemSpacing(itemSpacing);

        float windowX2 = windowPos.x + windowSize.x;
        for (int i = 0; i < sprites.size(); i++) {
            Sprite sprite = sprites.getSprite(i);
            float spriteWidth = sprite.getWidth() * 4;
            float spriteHeight = sprite.getHeight() * 4;
            int id = sprite.getTexID();
            Vector2f[] texCoords = sprite.getTexCoords();

            ImGui.pushID(i);
            if (ImGui.imageButton(id, spriteWidth, spriteHeight, texCoords[2].x, texCoords[0].y, texCoords[0].x, texCoords[2].y)) {
                GameObject object = Prefabs.generateSpriteObject(sprite, 32, 32);
                //Attach to mouse cursor
                levelEditor.getComponent(MouseControls.class).pickupObject(object);
            }
            ImGui.popID();
            ImVec2 lastButtonPos = new ImVec2();
            ImGui.getItemRectMax(lastButtonPos);
            float lastButtonX2 = lastButtonPos.x;
            float nextButtonX2 = lastButtonX2 + itemSpacing.x + spriteWidth;
            if (i + 1 < sprites.size() && nextButtonX2 < windowX2) {
                ImGui.sameLine();
            }
        }

        ImGui.end();
    }

    private void loadResources() {
        AssetPool.getShader("assets/shaders/default.glsl");
        AssetPool.addSpriteSheet("assets/images/spritesheets/decorationsAndBlocks.png", new SpriteSheet(AssetPool.getTexture2D("assets/images/spritesheets/decorationsAndBlocks.png"), 16, 16, 81, 0));
        AssetPool.getTexture2D("assets/images/blendImage2.png");
        AssetPool.getTexture2D("assets/images/testImage.png");
        AssetPool.getTexture2D("assets/images/testImage2.png");
    }
}
