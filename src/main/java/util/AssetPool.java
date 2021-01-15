package util;

import components.SpriteSheet;
import renderer.Shader;
import renderer.Texture2D;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

public class AssetPool {
    private static Map<String, Shader> shaders = new HashMap<String, Shader>();
    private static Map<String, Texture2D> textures = new HashMap<String, Texture2D>();
    private static Map<String, SpriteSheet> spriteSheets = new HashMap<String, SpriteSheet>();

    public static Shader getShader(String resourceName) {
        File file = new File(resourceName);
        if(AssetPool.shaders.containsKey(file.getAbsolutePath())) {
            return AssetPool.shaders.get(file.getAbsolutePath());
        } else {
            Shader shader = new Shader(resourceName);;
            AssetPool.shaders.put(file.getAbsolutePath(), shader);
            return shader;
        }
    }

    public static Texture2D getTexture2D(String resourceName) {
        File file = new File(resourceName);
        if(textures.containsKey(file.getAbsolutePath())) {
            return AssetPool.textures.get(file.getAbsolutePath());
        } else {
            Texture2D texture = new Texture2D(resourceName);
            AssetPool.textures.put(file.getAbsolutePath(), texture);
            return texture;
        }
    }

    public static void addSpriteSheet(String resourceName, SpriteSheet spriteSheet) {
        File file = new File(resourceName);

        if(!AssetPool.spriteSheets.containsKey(file.getAbsolutePath())) {
            AssetPool.spriteSheets.put(file.getAbsolutePath(), spriteSheet);
        }

    }

    public static SpriteSheet getSpriteSheet(String resourceName) {
        File file = new File(resourceName);
        if(!AssetPool.spriteSheets.containsKey(file.getAbsolutePath())) {
            assert false : "ERROR: Tried to access spritesheet '" + resourceName + "' and it has not been added to asset pool";
        }
        return AssetPool.spriteSheets.getOrDefault(file.getAbsolutePath(), null);
    }


}
