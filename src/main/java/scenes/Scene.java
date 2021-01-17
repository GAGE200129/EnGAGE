package scenes;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import components.Component;
import components.ComponentDeserializer;
import engage.*;
import imgui.ImGui;
import renderer.Renderer;
import renderer.Texture2D;
import renderer.Texture2DDeserializer;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public abstract class Scene {

    protected Renderer renderer = new Renderer();
    protected Camera camera;
    protected List<GameObject> gameObjects = new ArrayList<GameObject>();
    protected GameObject activeGO = null;
    protected boolean levelLoaded = false;
    private boolean isRunning = false;

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
        if (isRunning) {
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
        if (activeGO != null) {
            ImGui.begin("Inspector");
            activeGO.imGui();
            ImGui.end();
        }

        this.imGui();
    }

    public void imGui() {

    }

    public void saveExit() {
        Gson gson = new GsonBuilder()
                .setPrettyPrinting()
                .registerTypeAdapter(Component.class, new ComponentDeserializer())
                .registerTypeAdapter(GameObject.class, new GameObjectDeserializer())
                .registerTypeAdapter(Texture2D.class, new Texture2DDeserializer())
                .create();

        try {
            FileWriter fr = new FileWriter("level.txt");

            fr.write(gson.toJson(this.gameObjects));
            fr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void load() {
        Gson gson = new GsonBuilder()
                .setPrettyPrinting()
                .registerTypeAdapter(Component.class, new ComponentDeserializer())
                .registerTypeAdapter(GameObject.class, new GameObjectDeserializer())
                .registerTypeAdapter(Texture2D.class, new Texture2DDeserializer())
                .create();
        String inFile = "";
        try {
            inFile = new String(Files.readAllBytes(Paths.get("level.txt")));
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (!inFile.equals("")) {
            int maxGoID = -1;
            int maxComID = -1;
            GameObject[] objs = gson.fromJson(inFile, GameObject[].class);
            for (int i = 0; i < objs.length; i++) {
                this.addGameObjectToScene(objs[i]);

                for(Component c : objs[i].getAllComponents()) {
                    if(c.getUid() > maxComID) {
                        maxComID = c.getUid();
                    }
                }

                if(objs[i].getUid() > maxGoID) {
                    maxGoID = objs[i].getUid();
                }
            }
            GameObject.init(maxGoID++);
            Component.init(maxComID++);
            this.levelLoaded = true;
        }
    }
}
