package physic2d.primitives;

import org.joml.Vector2f;
import physic2d.rigidbody.RigidBody2D;

//Axis Aligned Bounding Box
public class AABB {
    private Vector2f halfSize;
    private Vector2f size = new Vector2f(); // x = width, y = height
    private RigidBody2D rigidBody = null;

    public AABB() {
        this.halfSize = new Vector2f(size).mul(0.5f);
    }

    public AABB(Vector2f min, Vector2f max) {
        this.size = new Vector2f(max).sub(min);
        this.halfSize = new Vector2f(size).mul(0.5f);
    }

    public Vector2f getMin() {
        return new Vector2f(this.rigidBody.getPosition()).sub(this.halfSize);
    }

    public Vector2f getMax() {
        return new Vector2f(this.rigidBody.getPosition()).add(this.halfSize);
    }
}
