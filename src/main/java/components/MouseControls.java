package components;

import engage.GameObject;
import engage.MouseListener;
import engage.Window;

import static org.lwjgl.glfw.GLFW.GLFW_MOUSE_BUTTON_LEFT;

public class MouseControls extends Component {

    private GameObject holding = null;

    public void pickupObject(GameObject go) {
        this.holding = go;
        Window.getCurrentScene().addGameObjectToScene(go);
    }

    public void place() {
        this.holding = null;
    }
    @Override
    public void update(double dt) {
        if(holding != null) {
            holding.transform.position.x = MouseListener.getOrthoX() - 16;
            holding.transform.position.y = MouseListener.getOrthoY() - 16;

            if(MouseListener.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
                place();
            }
        }
    }

}
