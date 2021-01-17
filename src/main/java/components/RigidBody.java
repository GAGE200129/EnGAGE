package components;

import org.joml.Vector3f;
import org.joml.Vector4f;

public class RigidBody extends Component {

    public transient Vector4f test = new Vector4f(0, 0, 0, 0);
    private int colliderType = 0;
    private float friction = 0.0f;
    private Vector3f velocity = new Vector3f(0, 0, 1);
}
