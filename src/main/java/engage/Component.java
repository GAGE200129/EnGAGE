package engage;

public abstract class Component {

    public transient GameObject gameObject = null;

    public void start() {}
    public void imGui() {}
    public void update(double dt) {}

}
