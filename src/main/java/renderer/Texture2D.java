package renderer;

import org.lwjgl.stb.STBImage;
import org.lwjgl.system.MemoryStack;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.stb.STBImage.*;

public class Texture2D {

    public static List<Integer> textureIDS = new ArrayList<>();

    public static void clear() {
        for(Integer i : textureIDS) {
            glDeleteTextures(i);
        }
    }

    private String filePath;
    private int textureID;
    private int width, height;

    public Texture2D(String filePath) {
        this.filePath = filePath;

        //Upload texture to GPU
        textureID = glGenTextures();
        textureIDS.add(textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        //Set texture parameters
        // 1: Repeat texture in both directions
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // X
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Y

        // 2: What to do when stretched ?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Get the nearest pixel

        // 3: What to do when shrank ?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Same

        ByteBuffer image = null;
        IntBuffer width = null, height = null, channels = null;
        try (MemoryStack stack = MemoryStack.stackPush()) {
            width = stack.mallocInt(1);
            height = stack.mallocInt(1);
            channels = stack.mallocInt(1);
            stbi_set_flip_vertically_on_load(true);
            image = stbi_load(filePath, width, height, channels, 0);
            if (image != null) {
                this.width = width.get(0);
                this.height = height.get(0);
                if (channels.get(0) == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width.get(0), height.get(0), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                } else if (channels.get(0) == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width.get(0), height.get(0), 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                } else {
                    assert false : "ERROR: (Texture2D) Unknown number of channels '" + channels.get(0) + "'";
                }

            } else {
                assert false : "ERROR: (Texture2D) Could not load image '" + filePath + "'\n STBImage error log: " + STBImage.stbi_failure_reason();
            }
            if (image != null) {
                stbi_image_free(image);
            }
        }


        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public void bind() {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    public void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    public int getID() {
        return textureID;
    }

    public int getWidth() {
        return this.width;
    }

    public int getHeight() {
        return this.height;
    }
}
