package components;

import engage.Component;

public class FontRenderer extends Component {

    @Override
    public void start() {
        if(gameObject.getComponent(SpriteRenderer.class) != null) {
            System.out.println("FONT RENDERER: Found sprite renderer !");
        }
    }
}
