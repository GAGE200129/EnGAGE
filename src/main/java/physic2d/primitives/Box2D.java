package physic2d.primitives;

import org.joml.Vector2f;
import physic2d.rigidbody.RigidBody2D;

public class Box2D {
    private Vector2f size = new Vector2f();
    private Vector2f halfSize;
    private RigidBody2D rigidBody = null;

    public Box2D() {
        this.halfSize = new Vector2f(size).mul(0.5f);
    }

    public Box2D(Vector2f min, Vector2f max) {
        this.size = new Vector2f(max).sub(min);
        this.halfSize = new Vector2f(size).mul(0.5f);
    }

    public Vector2f getMin() {
        return new Vector2f(this.rigidBody.getPosition()).sub(this.halfSize);
    }

    public Vector2f getMax() {
        return new Vector2f(this.rigidBody.getPosition()).add(this.halfSize);
    }

    public Vector2f[] getVertices() {
        final Vector2f min = getMin();
        final Vector2f max = getMax();
        Vector2f[] vertices = {
                new Vector2f(min.x, min.y), new Vector2f(min.x, max.y),
                new Vector2f(max.x, min.y), new Vector2f(max.x, max.y),
        };
        if(rigidBody.getRotation() != 0.0f) {
            for (Vector2f vert : vertices) {

                //TODO: IMPLEMENT THIS
                //Rotates point(vector2) about center(Vector2) by rotate(in degrees)
                //JMath.rotate(vert, this.rigidBody.getPosition(), this.rigidBody.getRotation());
            }
        }
        return vertices;
    }

    public RigidBody2D getBody() {
        return this.rigidBody;
    }
}
