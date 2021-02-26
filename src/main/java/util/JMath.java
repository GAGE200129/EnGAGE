package util;

import org.joml.Vector2f;

public class JMath {

    public static void rotate(Vector2f v, float angleInDegree, Vector2f o) {
        float x = v.x - o.x;
        float y = v.y - o.y;

        float cos = (float)Math.cos(Math.toRadians(angleInDegree));
        float sin = (float)Math.sin(Math.toRadians(angleInDegree));

        float xPrime = x * cos - y * sin;
        float yPrime = x * sin + y * cos;

        xPrime += o.x;
        yPrime += o.y;

        v.set(xPrime, yPrime);
    }

    public static boolean compare(float a, float b, float epsilon) {
        return Math.abs(a - b) <= epsilon * Math.max(1.0f, Math.max(Math.abs(a), Math.abs(b)));
    }


    public static boolean compare(Vector2f vec1, Vector2f vec2, float epsilon) {
        return compare(vec1.x, vec2.x, epsilon) && compare(vec1.y, vec2.y, epsilon);
    }

    public static boolean compare(float a, float b) {
        return Math.abs(a - b) <= Float.MIN_VALUE * Math.max(1.0f, Math.max(Math.abs(a), Math.abs(b)));
    }


    public static boolean compare(Vector2f vec1, Vector2f vec2) {
        return compare(vec1.x, vec2.x, Float.MIN_VALUE) && compare(vec1.y, vec2.y, Float.MIN_VALUE);
    }

}
