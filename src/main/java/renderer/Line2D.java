package renderer;

import org.joml.Vector2f;
import org.joml.Vector3f;

public class Line2D {
    private Vector2f from;
    private Vector2f to;
    private Vector3f color;
    private int lifeTime;

    public Line2D(Vector2f from, Vector2f to) {
        this.from = from;
        this.to = to;
    }

    public Line2D(Vector2f from, Vector2f to, Vector3f color, int lifeTime) {
        this.from = from;
        this.to = to;
        this.color = color;
        this.lifeTime = lifeTime;
    }

    public int beginFrame() {
        this.lifeTime--;
        return this.lifeTime;
    }

    public Vector2f getFrom() {
        return from;
    }
    public Vector2f getTo() {
        return to;
    }
    public Vector2f getStart() { return  from; }
    public Vector2f getEnd() { return  to; }

    public Vector3f getColor() {
        return color;
    }
    public int getLifeTime() {
        return lifeTime;
    }

    public float lengthSquared() {
        return new Vector2f(to).sub(from).lengthSquared();
    }
}
