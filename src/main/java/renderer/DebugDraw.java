package renderer;

import engage.Window;
import org.joml.Vector2f;
import org.joml.Vector3f;
import util.AssetPool;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.glEnableVertexAttribArray;
import static org.lwjgl.opengl.GL20.glVertexAttribPointer;
import static org.lwjgl.opengl.GL30.*;

public class DebugDraw {
    private static int MAX_LINES = 500;

    private static List<Line2D> lines = new ArrayList<Line2D>();
    private static float[] vertexArray = new float[MAX_LINES * 6 * 2];
    private static Shader shader = AssetPool.getShader("assets/shaders/debugLine2D.glsl");
    private static int vaoID;
    private static int vboID;
    private static boolean started = false;

    public static void start() {
        // Tạo VAO
        vaoID = glGenVertexArrays();
        glBindVertexArray(vaoID);

        // Tạo VBO
        vboID = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, vertexArray.length * Float.BYTES, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * Float.BYTES, 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * Float.BYTES, 3 * Float.BYTES);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glLineWidth(2.0f);
    }

    public static void clear() {
        glDeleteVertexArrays(vaoID);
        glDeleteBuffers(vboID);
    }

    public static void beginFrame() {
        if (!started) {
            start();
            started = true;
        }
        for (int i = 0; i < lines.size(); i++) {
            if (lines.get(i).beginFrame() < 0) {
                lines.remove(i);
                i--;
            }
        }
    }

    //=========LINES=============//
    public static void addLine2D(Vector2f from, Vector2f to, Vector3f color, int lifeTime) {
        if (lines.size() >= MAX_LINES) return;
        lines.add(new Line2D(from, to, color, lifeTime));
    }
    public static void addLine2D(Vector2f from, Vector2f to, Vector3f color) {
        addLine2D(from, to, color, 1);
    }
    public static void addLine2D(Vector2f from, Vector2f to) {
        addLine2D(from, to, new Vector3f(0, 1, 0), 1);
    }
    public static void addLine2D(Line2D line) {
        addLine2D(line.getFrom(), line.getTo(), line.getColor(), line.getLifeTime());
    }


    public static void draw() {
        if (lines.size() <= 0) return;
        int index = 0;
        for (Line2D line : lines) {
            for (int i = 0; i < 2; i++) {
                Vector2f position = i == 0 ? line.getFrom() : line.getTo();
                Vector3f color = line.getColor();

                //Load position
                vertexArray[index] = position.x;
                vertexArray[index + 1] = position.y;
                vertexArray[index + 2] = -10f;

                //Load color
                vertexArray[index + 3] = color.x;
                vertexArray[index + 4] = color.y;
                vertexArray[index + 5] = color.z;
                index += 6;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Arrays.copyOfRange(vertexArray, 0, lines.size() * 6 * 2));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        shader.use();
        shader.uploadMat4("uProj", Window.getCurrentScene().getCamera().getProjectionMatrix());
        shader.uploadMat4("uView", Window.getCurrentScene().getCamera().getViewMatrix());
        glBindVertexArray(vaoID);
        glDrawArrays(GL_LINES, 0, lines.size() * 2);
        glBindVertexArray(0);
        shader.detach();

    }
}
