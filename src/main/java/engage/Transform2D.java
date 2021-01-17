package engage;

import org.joml.Vector2f;

public class Transform2D {
    public Vector2f position;
    public Vector2f scale;

    public Transform2D() {
        this.init(new Vector2f(), new Vector2f());
    }

    public Transform2D(Vector2f position) {
        this.init(position, new Vector2f());
    }

    public Transform2D(Vector2f position, Vector2f scale) {
        this.init(position, scale);
    }

    public void init(Vector2f position, Vector2f scale) {
        this.position = position;
        this.scale = scale;
    }

    public Transform2D copy() {
        return new Transform2D(new Vector2f(this.position), new Vector2f(this.scale));
    }

    public void copy(Transform2D to) {
        to.position.set(this.position);
        to.scale.set(this.scale);
    }

    public boolean equals(Object o) {
        if (o == null) return false;
        if (!(o instanceof Transform2D)) return false;

        Transform2D t = (Transform2D) o;
        return t.position.equals(this.position) && t.scale.equals(this.scale);
    }
}
