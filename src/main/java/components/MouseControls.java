package components;

import engage.GameObject;
import engage.MouseListener;
import engage.Window;
import util.Settings;

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
            holding.transform.position.x = MouseListener.getOrthoX();
            holding.transform.position.y = MouseListener.getOrthoY();
            holding.transform.position.x = (int)(holding.transform.position.x / Settings.GRID_WIDTH) * Settings.GRID_WIDTH;
            holding.transform.position.y = (int)(holding.transform.position.y / Settings.GRID_HEIGHT) * Settings.GRID_HEIGHT;

            if(MouseListener.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
                place();
            }
        }
    }

}
