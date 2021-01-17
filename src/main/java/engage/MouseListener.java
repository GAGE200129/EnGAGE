package engage;

import org.joml.Vector4f;

import static org.lwjgl.glfw.GLFW.GLFW_PRESS;
import static org.lwjgl.glfw.GLFW.GLFW_RELEASE;

public class MouseListener {
    private static MouseListener instance;
    private double scrollX, scrollY;
    private double xPos, yPos, lastX, lastY;
    private boolean buttonPressed[] = new boolean[9];
    private boolean dragging;
    private MouseListener() {
        this.scrollX = 0.0;
        this.scrollY = 0.0;
        this.xPos = 0.0;
        this.yPos = 0.0;
        this.lastX = 0.0;
        this.lastY = 0.0;
    }

    public static MouseListener get() {
        if (MouseListener.instance == null)
            MouseListener.instance = new MouseListener();

        return MouseListener.instance;
    }

    public static void mousePosCallback(long window, double xPos, double yPos) {
        get().lastX = get().xPos;
        get().lastY = get().yPos;

        get().xPos = xPos;
        get().yPos = yPos;
        get().dragging = get().buttonPressed[0] || get().buttonPressed[1] || get().buttonPressed[2];
    }


    public static void mouseButtonCallback(long window, int button, int action, int mod) {
        if (button < get().buttonPressed.length) {
            if (action == GLFW_PRESS) {
                get().buttonPressed[button] = true;
            } else if (action == GLFW_RELEASE) {
                get().buttonPressed[button] = false;
                get().dragging = false;
            }
        }
    }

    public static void mouseScrollCallback(long window, double xOffset, double yOffset) {
        get().scrollX = xOffset;
        get().scrollY = yOffset;
    }

    public static void endFrame() {
        get().scrollX = 0.0;
        get().scrollY = 0.0;
        get().lastX = get().xPos;
        get().lastY = get().yPos;
    }

    public static float getX() {
        return (float) get().xPos;
    }

    public static float getY() {
        return (float) get().yPos;
    }

    public static float getOrthoX() {
        float currentX = (getX() / (float) Window.getWidth()) * 2f - 1f;
        Vector4f temp = new Vector4f(currentX, 0, 0, 1);
        temp.mul(Window.getCurrentScene().getCamera().getInverseProjection()).mul(Window.getCurrentScene().getCamera().getInverseView());
        return temp.x;
    }


    public static float getOrthoY() {
        float currentY = ((Window.getHeight() - getY() ) / (float) Window.getHeight()) * 2f - 1f;
        Vector4f temp = new Vector4f(0, currentY, 0, 1);
        temp.mul(Window.getCurrentScene().getCamera().getInverseProjection()).mul(Window.getCurrentScene().getCamera().getInverseView());
        return temp.y;
    }

    public static float getDx() {
        return (float) (get().lastX - get().xPos);
    }

    public static float getDy() {
        return (float) (get().lastY - get().yPos);
    }

    public static float getScrollX() {
        return (float) get().scrollX;
    }

    public static float getScrollY() {
        return (float) get().scrollY;
    }

    public static boolean isDragging() {
        return get().dragging;
    }


    public static boolean mouseButtonDown(int button) {
        if (button < get().buttonPressed.length) {
            return get().buttonPressed[button];
        } else {
            return false;
        }

    }

}
