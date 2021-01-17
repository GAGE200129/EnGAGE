package renderer;

import com.google.gson.*;

import java.lang.reflect.Type;

public class Texture2DDeserializer implements JsonDeserializer<Texture2D> {
    @Override
    public Texture2D deserialize(JsonElement json, Type typeOfT, JsonDeserializationContext context) throws JsonParseException {
        JsonObject jsonObject = json.getAsJsonObject();
        String filePath = jsonObject.get("filePath").getAsString();
        Texture2D texture = new Texture2D();
        texture.init(filePath);
        return texture;
    }
}
