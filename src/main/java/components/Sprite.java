package components;

import org.joml.Vector2f;
import renderer.Texture2D;

public class Sprite {

    private final Texture2D texture;
    private final Vector2f[] texCoords;

    public Sprite(Texture2D texture) {
        this.texture = texture;
        this.texCoords = new Vector2f[]{
                new Vector2f(1, 1),
                new Vector2f(1, 0),
                new Vector2f(0, 0),
                new Vector2f(0, 1),
        } ;
    }

    public Sprite(Texture2D texture, Vector2f[] texCoords) {
        this.texture = texture;
        this.texCoords = texCoords;
    }

    public Texture2D getTexture() {
        return this.texture;
    }

    public Vector2f[] getTexCoords() {
        return this.texCoords;
    }
}
