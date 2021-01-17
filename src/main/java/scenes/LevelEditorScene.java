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

    private MouseControls mouseControls = new MouseControls();

    @Override
    public void init() {
        this.camera = new Camera(new Vector2f(-250, 0));
        loadResources();
        sprites = AssetPool.getSpriteSheet("assets/images/spritesheets/decorationsAndBlocks.png");
        DebugDraw.addLine2D(new Vector2f(0, 0), new Vector2f(100, 100), new Vector3f(1, 0, 0), 180);
        if (levelLoaded) {
            return;
        }


    }


    float t = 0.0f;
    @Override
    public void update(float dt) {

        float x = ((float)Math.sin(t) * 200.0f) + 600;
        float y = ((float)Math.cos(t) * 200.0f) + 400;
        t += 0.05f;
        DebugDraw.addLine2D(new Vector2f(600, 400), new Vector2f(x, y),  new Vector3f(0, 1, 0), 100);

        this.renderer.render();
        this.mouseControls.update(dt);
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
            if (ImGui.imageButton(id, spriteWidth, spriteHeight, texCoords[0].x, texCoords[0].y, texCoords[2].x, texCoords[2].y)) {
                GameObject object = Prefabs.generateSpriteObject(sprite, spriteWidth, spriteHeight);
                //Attach to mouse cursor
                mouseControls.pickupObject(object);
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
