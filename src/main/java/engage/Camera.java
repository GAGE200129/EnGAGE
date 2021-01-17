package engage;

import org.joml.Matrix4f;
import org.joml.Vector2f;
import org.joml.Vector3f;

public class Camera {

    private final Matrix4f projectionMatrix, viewMatrix, inverseProjection, inverseView;
    public Vector2f position;
    private Vector2f projectionSize;

    public Camera(Vector2f position) {
        this.position = position;
        this.projectionSize = new Vector2f(32f * 40f, 32f * 21f);
        this.projectionMatrix = new Matrix4f();
        this.viewMatrix = new Matrix4f();
        this.inverseProjection = new Matrix4f();
        this.inverseView = new Matrix4f();
        this.adjustProjection();
    }

    public void adjustProjection() {
        projectionMatrix.identity();
        projectionMatrix.ortho(0.0f, projectionSize.x, 0f, projectionSize.y, 0f, 100f);
        projectionMatrix.invert(inverseProjection);
    }

    public Matrix4f getViewMatrix() {
        Vector3f frontVector = new Vector3f(0f, 0f, -1f);
        Vector3f upVector = new Vector3f(0f, 1f, 0f);
        viewMatrix.identity();
        viewMatrix.lookAt(new Vector3f(position.x, position.y, 20f),
                frontVector.add(position.x, position.y, 0.0f),
                upVector);
        viewMatrix.invert(inverseView);
        return viewMatrix;
    }

    public Matrix4f getProjectionMatrix() {
        return this.projectionMatrix;
    }
    public Matrix4f getInverseProjection() { return inverseProjection; }
    public Matrix4f getInverseView() {
        return inverseView;
    }
    public Vector2f getProjectionSize() { return projectionSize; }
}
