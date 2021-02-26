package physic2d.primitives;

import components.Component;
import org.joml.Vector2f;

public abstract class Collider2D extends Component {
    protected Vector2f offset = new Vector2f();

    //public abstract float getInertiaTenser(float pass);
}
