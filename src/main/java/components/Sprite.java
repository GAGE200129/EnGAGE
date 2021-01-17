package components;

import org.joml.Vector2f;
import renderer.Texture2D;

public class Sprite {

    private float width, height;

    private Texture2D texture = null;
    private Vector2f[] texCoords = {
            new Vector2f(1, 1),
            new Vector2f(1, 0),
            new Vector2f(0, 0),
            new Vector2f(0, 1),
    };

//    public Sprite(Texture2D texture) {
//        this.texture = texture;
//        this.texCoords = new Vector2f[]{
//                new Vector2f(1, 1),
//                new Vector2f(1, 0),
//                new Vector2f(0, 0),
//                new Vector2f(0, 1),
//        } ;
//    }
//
//    public Sprite(Texture2D texture, Vector2f[] texCoords) {
//        this.texture = texture;
//        this.texCoords = texCoords;
//    }

    public Texture2D getTexture() {
        return this.texture;
    }

    public void setTexture(Texture2D tex) {
        this.texture = tex;
    }

    public Vector2f[] getTexCoords() {
        return this.texCoords;
    }

    public void setTexCoords(Vector2f[] texCoords) {
        this.texCoords = texCoords;
    }

    public float getWidth() {
        return width;
    }

    public void setWidth(float width) {
        this.width = width;
    }

    public float getHeight() {
        return height;
    }

    public void setHeight(float height) {
        this.height = height;
    }

    public int getTexID() {
        return texture == null ? 0 : texture.getID();
    }
}
