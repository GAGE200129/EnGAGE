package physic2d.rigidbody;

import org.joml.Vector2f;
import physic2d.primitives.*;
import renderer.Line2D;
import util.JMath;

public class IntersectionDetector2D {
    public static boolean pointOnLine(Vector2f point, Line2D line) {

        //y = ax + b
        float dy = line.getEnd().y - line.getStart().y;
        float dx = line.getEnd().x - line.getStart().x;

        float a = dy / dx;
        if (dx == 0.0f) {
            return JMath.compare(point.x, line.getStart().x);
        }

        float b = line.getEnd().y - a * line.getEnd().x;

        return point.y == a * point.x + b;
    }

    public static boolean pointInCircle(Vector2f point, Circle circle) {
        Vector2f circleCenter = circle.getCenter();
        Vector2f centerToPoint = new Vector2f(point).sub(circleCenter);

        return centerToPoint.lengthSquared() <= circle.getRadius() * circle.getRadius();
    }

    public static boolean pointInAABB(Vector2f point, AABB box) {
        Vector2f min = box.getMin();
        Vector2f max = box.getMax();

        return point.x <= max.x && point.x >= min.x &&
                point.y <= max.y && point.y >= min.y;
    }

    public static boolean pointInBox2D(Vector2f point, Box2D box) {

        Vector2f pointInLocalBoxSpace = new Vector2f(point);
        JMath.rotate(pointInLocalBoxSpace, box.getBody().getRotation(), box.getBody().getPosition());

        Vector2f min = box.getMin();
        Vector2f max = box.getMax();

        return pointInLocalBoxSpace.x <= max.x && pointInLocalBoxSpace.x >= min.x &&
                pointInLocalBoxSpace.y <= max.y && pointInLocalBoxSpace.y >= min.y;
    }


    public static boolean lineAndCircle(Line2D l, Circle c) {
        if (pointInCircle(l.getStart(), c) || pointInCircle(l.getEnd(), c)) {
            return true;
        }

        Vector2f ab = new Vector2f(l.getEnd()).sub(l.getStart());
        Vector2f circleCenter = c.getCenter();
        Vector2f lineStartToCenter = new Vector2f(circleCenter).sub(l.getStart());

        float t = lineStartToCenter.dot(ab) / ab.dot(ab);

        if (t < 0.0f || t > 1.0f) {
            return false;
        }

        Vector2f closestPoint = new Vector2f(l.getStart()).add(ab.mul(t));

        return pointInCircle(closestPoint, c);
    }

    public static boolean lineAndAABB(Line2D line, AABB box) {
        if (pointInAABB(line.getStart(), box) || pointInAABB(line.getEnd(), box)) {
            return true;
        }

        Vector2f unit = new Vector2f(line.getEnd()).sub(line.getStart()).normalize();
        unit.x = (unit.x != 0f) ? 1.0f / unit.x : 0f;
        unit.y = (unit.y != 0f) ? 1.0f / unit.y : 0f;

        Vector2f min = new Vector2f(box.getMin());
        min.sub(line.getStart()).mul(unit);
        Vector2f max = new Vector2f(box.getMax());
        max.sub(line.getStart()).mul(unit);

        float tmin = Math.max(Math.min(min.x, max.x), Math.min(min.y, max.y));
        float tmax = Math.min(Math.max(min.x, max.x), Math.max(min.y, max.y));

        if (tmax < 0 || tmin > tmax) {
            return false;
        }

        float t = (tmin < 0f) ? tmax : tmin;
        return t > 0f && t * t < line.lengthSquared();
    }

    public static boolean lineAndBox2D(Line2D line, Box2D box) {
        float theta = -box.getBody().getRotation();
        Vector2f center = box.getBody().getPosition();
        Vector2f localStart = new Vector2f(line.getStart());
        Vector2f localEnd = new Vector2f(line.getEnd());
        JMath.rotate(localStart, theta, center);
        JMath.rotate(localEnd, theta, center);
        Line2D localLine = new Line2D(localStart, localEnd);
        AABB aabb = new AABB(box.getMin(), box.getMax());
        return lineAndAABB(localLine, aabb);
    }

    public static boolean raycast(Circle circle, Ray2D ray, RaycastResult result) {
        RaycastResult.reset(result);

        Vector2f originToCircle = new Vector2f(circle.getCenter()).sub(ray.getOrigin());
        float radiusSquared = circle.getRadius() * circle.getRadius();
        float originToCircleLengthSquared = originToCircle.lengthSquared();


        float a = originToCircle.dot(ray.getDirection());
        float bSq = originToCircleLengthSquared - (a * a);
        if (radiusSquared - bSq < 0f) {
            return false;
        }

        float f = (float) Math.sqrt(radiusSquared - bSq);
        float t;
        if (originToCircleLengthSquared < radiusSquared) {
            t = a + f;
        } else {
            t = a - f;
        }

        if (result != null) {
            Vector2f point = new Vector2f(ray.getOrigin()).add(ray.getDirection()).mul(t);
            Vector2f normal = new Vector2f(point).sub(circle.getCenter());
            normal.normalize();
            result.init(point, normal, t, true);
        }

        return true;

    }
}
