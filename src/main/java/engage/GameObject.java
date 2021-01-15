package engage;

import java.util.ArrayList;
import java.util.List;

public class GameObject {

    private String name;
    private List<Component> components;
    public Transform2D transform;
    private int zIndex;

    public GameObject(String name) {
        this.name = name;
        this.zIndex = 0;
        this.components = new ArrayList<Component>();
        this.transform = new Transform2D();
    }

    public GameObject(String name, Transform2D transform, int zIndex) {
        this.name = name;
        this.zIndex = zIndex;
        this.components = new ArrayList<Component>();
        this.transform = transform;
    }

    public void start() {
        for(int i = 0; i < components.size(); i++) {
            components.get(i).start();
        }
    }
    public void imGui() {
        for(Component c : components) {
            c.imGui();
        }
    }
    public void update(double dt) {
        for(int i = 0; i < components.size(); i++) {
            components.get(i).update(dt);
        }
    }

    public void addComponent(Component c) {
        this.components.add(c);
        c.gameObject = this;
    }

    public <T extends Component> void removeComponent(Class<T> componentClass) {
        for(int i = 0; i < components.size(); i++) {
            Component c = components.get(i);
            if(componentClass.isAssignableFrom(c.getClass())) {
                components.remove(i);
                return;
            }
        }
    }

    public <T extends Component> T getComponent(Class<T> componentClass) {
        for(Component c : components) {
            try {
                if (componentClass.isAssignableFrom(c.getClass())) {
                    return componentClass.cast(c);
                }
            } catch(ClassCastException e) {
                e.printStackTrace();
                assert false : "ERROR: Casting component.";
            }

        }

        return null;
    }

    public int getzIndex() {
        return this.zIndex;
    }

}
