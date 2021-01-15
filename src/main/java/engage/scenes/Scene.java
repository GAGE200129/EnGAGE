package engage.scenes;

import engage.Camera;
import engage.GameObject;
import imgui.ImGui;
import renderer.Renderer;

import java.util.ArrayList;
import java.util.List;

public abstract class Scene {

    protected Renderer renderer = new Renderer();
    protected Camera camera;
    private boolean isRunning = false;
    protected List<GameObject> gameObjects = new ArrayList<GameObject>();
    protected GameObject activeGO = null;

    public Scene() {

    }

    public void init() {
    }

    public void start() {
        for (GameObject go : gameObjects) {
            go.start();
            this.renderer.add(go);
        }
        isRunning = true;
    }

    public void addGameObjectToScene(GameObject obj) {
        if(isRunning) {
            gameObjects.add(obj);
            obj.start();
            this.renderer.add(obj);
        } else {
            gameObjects.add(obj);
        }
    }

    public abstract void update(float dt);

    public Camera getCamera() {
        return this.camera;
    }

    public void sceneImGui() {
        if(activeGO != null) {
            ImGui.begin("Inspector");
            activeGO.imGui();
            ImGui.end();
        }

        this.imGui();
    }
    public void imGui() {

    }
}
