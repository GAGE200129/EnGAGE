package engage;

import renderer.DebugDraw;
import scenes.LevelEditorScene;
import scenes.LevelScene;
import scenes.Scene;
import org.lwjgl.Version;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;
import renderer.RenderBatch;
import renderer.Shader;
import renderer.Texture2D;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.GL_MAX_TEXTURE_IMAGE_UNITS;
import static org.lwjgl.opengl.GL20.GL_SHADING_LANGUAGE_VERSION;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window {
    private static Window instance = null;
    private static Scene currentScene = null;

    public static Window get() {
        if (Window.instance == null) {
            Window.instance = new Window();
        }
        return Window.instance;
    }

    private final String title;
    public float r, g, b;
    private long glfwWindow;
    private int width;
    private int height;

    private ImGuiLayer imguiLayer;


    private Window() {
        this.glfwWindow = 0;
        this.width = 1600;
        this.height = 900;
        this.title = "EnGAGE engine.";

        this.r = 1f;
        this.g = 1f;
        this.b = 1f;
    }



    public static void changeScene(int newScene) {
        switch (newScene) {
            case 0:
                currentScene = new LevelEditorScene();
                break;
            case 1:
                currentScene = new LevelScene();
                break;
            default:
                assert false : "Unknown scene '" + newScene + "'";
                break;
        }
        currentScene.load();
        currentScene.init();
        currentScene.start();
    }

    public static Scene getCurrentScene() {
        return Window.currentScene;
    }
    public static int getWidth() {
        return Window.get().width;
    }
    public static void setWidth(int width) {
        get().width = width;
    }
    public static int getHeight() {
        return Window.get().height;
    }
    public static void setHeight(int height) {
        get().height = height;
    }

    public void run() {
        System.out.println("Hello LWJGL" + Version.getVersion() + "!");


        init();
        loop();


        //Free memory
        glfwFreeCallbacks(glfwWindow);
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
        glfwSetErrorCallback(null).free();

    }

    public void init() {
        // Setup an error callback. The default implementation
        // will print the error message in System.err.
        GLFWErrorCallback.createPrint(System.err).set();

        //Initialize GLFW
        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW.");
        }
        //Configure glfw
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        //Create the window

        glfwWindow = glfwCreateWindow(this.width, this.height, this.title, NULL, NULL);
        if (glfwWindow == NULL) {
            throw new IllegalStateException("Failed to create GLFW window.");
        }

        glfwSetCursorPosCallback(glfwWindow, MouseListener::mousePosCallback);
        glfwSetScrollCallback(glfwWindow, MouseListener::mouseScrollCallback);
        glfwSetMouseButtonCallback(glfwWindow, MouseListener::mouseButtonCallback);
        glfwSetKeyCallback(glfwWindow, KeyListener::keyCallback);
        glfwSetWindowSizeCallback(glfwWindow, (w, width, height) -> {
            Window.setWidth(width);
            Window.setHeight(height);
        });

        //Make the OpenGL context to this window
        glfwMakeContextCurrent(glfwWindow);

        //Show the window
        glfwShowWindow(glfwWindow);
        this.setVsync(true);

        GL.createCapabilities();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        //Print device infos
        System.out.println("\tOpengl Info");
        System.out.println("Opengl version: " + glGetString(GL_VERSION));
        System.out.println("Vendor: " + glGetString(GL_VENDOR));
        System.out.println("Renderer: " + glGetString(GL_RENDERER));
        System.out.println("Glsl version: " + glGetString(GL_SHADING_LANGUAGE_VERSION));
        System.out.println("Max texture units: " + glGetInteger(GL_MAX_TEXTURE_IMAGE_UNITS));


        //Create imgui context
        this.imguiLayer = new ImGuiLayer(glfwWindow);
        this.imguiLayer.initImGui();

        //Create debug draw
        DebugDraw.start();
        Window.changeScene(0);
    }

    public void loop() {
        float startTime = (float) glfwGetTime();
        float endTime;
        float dt = -1.0f;


        while (!glfwWindowShouldClose(glfwWindow)) {
            glfwPollEvents();
            DebugDraw.beginFrame();
            glClearColor(r, g, b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (dt >= 0) {
                DebugDraw.draw();
                currentScene.update(dt);
            }
            this.imguiLayer.update(dt, currentScene);
            glfwSwapBuffers(glfwWindow);

            endTime = (float) glfwGetTime();
            dt = endTime - startTime;
            startTime = endTime;
        }
        currentScene.saveExit();
        DebugDraw.clear();
        Shader.clear();
        Texture2D.clear();
        RenderBatch.clear();
        this.imguiLayer.destroyImGui();
    }

    public void setVsync(boolean enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
    }
}
