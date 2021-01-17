package renderer;

import org.joml.Matrix3f;
import org.joml.Matrix4f;
import org.joml.Vector4f;
import org.lwjgl.BufferUtils;

import java.io.IOException;
import java.nio.FloatBuffer;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.opengl.GL11.GL_FALSE;
import static org.lwjgl.opengl.GL20.*;

public class Shader {

    private static final FloatBuffer matrix4fBuffer = BufferUtils.createFloatBuffer(16);
    private static final FloatBuffer matrix3fBuffer = BufferUtils.createFloatBuffer(9);
    public static List<Integer> programs = new ArrayList<>();
    public static List<Integer> shaders = new ArrayList<>();
    private int shaderProgramID;
    private boolean beingUsed;
    private String vertexSource;
    private String fragmentSource;
    private String filePath;
    public Shader(String filePath) {
        this.filePath = filePath;
        try {
            String source = new String(Files.readAllBytes(Paths.get(filePath)));
            String[] splitString = source.split("(#type)( )+([a-zA-Z]+)");

            //First pattern
            int index = source.indexOf("#type") + 6;
            int eol = source.indexOf("\r\n", index);
            String firstPattern = source.substring(index, eol).trim();

            //Second pattern
            index = source.indexOf("#type", eol) + 6;
            eol = source.indexOf("\r\n", index);
            String secondPatern = source.substring(index, eol).trim();


            if (firstPattern.equals("vertex")) {
                vertexSource = splitString[1];
            } else if (firstPattern.equals("fragment")) {
                fragmentSource = splitString[1];
            } else {
                throw new IOException("Unexpected token '" + firstPattern + "'");
            }


            if (secondPatern.equals("vertex")) {
                vertexSource = splitString[2];
            } else if (secondPatern.equals("fragment")) {
                fragmentSource = splitString[2];
            } else {
                throw new IOException("Unexpected token '" + secondPatern + "'");
            }
        } catch (IOException e) {
            e.printStackTrace();
            assert false : "ERROR: Could not open file for shader: '" + filePath + "'";
        }

        this.compileAndLink();

    }

    public static void clear() {
        for (Integer i : programs) {
            glDeleteProgram(i);
        }
        for (Integer i : shaders) {
            glDeleteShader(i);
        }
    }

    public void compileAndLink() {
        int vertexID, fragmentID;
        //========CREATE VERTEX SHADER============//
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        shaders.add(vertexID);
        //Upload to gpu
        glShaderSource(vertexID, vertexSource);
        glCompileShader(vertexID);

        //Check for error
        int success = glGetShaderi(vertexID, GL_COMPILE_STATUS);
        if (success == GL_FALSE) {
            int logLength = glGetShaderi(vertexID, GL_INFO_LOG_LENGTH);
            System.out.println("ERROR: '" + filePath + "'\n\tVertex Shader compilation failed.");
            System.out.println(glGetShaderInfoLog(vertexID, logLength));
            assert false : "";

        }


        //========CREATE FRAGMENT SHADER============//
        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        shaders.add(fragmentID);
        //Upload to gpu
        glShaderSource(fragmentID, fragmentSource);
        glCompileShader(fragmentID);

        //Check for error
        success = glGetShaderi(fragmentID, GL_COMPILE_STATUS);
        if (success == GL_FALSE) {
            int logLength = glGetShaderi(fragmentID, GL_INFO_LOG_LENGTH);
            System.out.println("ERROR: '" + filePath + "'\n\tFragment Shader compilation failed.");
            System.out.println(glGetShaderInfoLog(fragmentID, logLength));
            assert false : "";

        }

        //=============CREATE PROGRAM===============//
        shaderProgramID = glCreateProgram();
        programs.add(shaderProgramID);
        glAttachShader(shaderProgramID, vertexID);
        glAttachShader(shaderProgramID, fragmentID);
        glLinkProgram(shaderProgramID);
        success = glGetProgrami(shaderProgramID, GL_LINK_STATUS);
        if (success == GL_FALSE) {
            int logLength = glGetProgrami(shaderProgramID, GL_INFO_LOG_LENGTH);
            System.out.println("ERROR: '" + filePath + "'\n\tShader linking failed.");
            System.out.println(glGetProgramInfoLog(shaderProgramID, logLength));
            assert false : "";
        }
    }

    public void use() {
        if (!beingUsed) {
            beingUsed = true;
            glUseProgram(shaderProgramID);
        }
    }

    public void detach() {

        glUseProgram(0);
        beingUsed = false;
    }


    public void uploadIntArray(String varName, int[] array) {
        this.use();
        glUniform1iv(glGetUniformLocation(shaderProgramID, varName), array);
    }

    public void uploadMat4(String varName, Matrix4f mat) {
        this.use();
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        mat.get(matrix4fBuffer);
        glUniformMatrix4fv(varLocation, false, matrix4fBuffer);
    }

    public void uploadMat3(String varName, Matrix3f mat) {
        this.use();
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        mat.get(matrix3fBuffer);
        glUniformMatrix3fv(varLocation, false, matrix3fBuffer);
    }

    public void uploadVec4f(String varName, Vector4f vec4f) {
        this.use();
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        glUniform4f(varLocation, vec4f.x, vec4f.y, vec4f.z, vec4f.w);
    }

    public void uploadFloat(String varName, float var) {
        this.use();
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        glUniform1f(varLocation, var);
    }

    public void uploadInt(String varName, int var) {
        this.use();
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        glUniform1i(varLocation, var);
    }

    public void uploadTexture2D(String varName, int slot) {
        this.use();
        int varLocation = glGetUniformLocation(shaderProgramID, varName);
        glUniform1i(varLocation, slot);
    }
}
