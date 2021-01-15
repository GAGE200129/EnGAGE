package renderer;

import components.SpriteRenderer;
import engage.GameObject;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Renderer {
    private static final int MAX_BATCH_SIZE = 1000;
    private List<RenderBatch> batches;

    public Renderer() {
        this.batches = new ArrayList<RenderBatch>();
    }

    public void render() {
        for(RenderBatch batch : batches) {
            batch.render();
        }
    }


    public void add(GameObject go) {
        SpriteRenderer spr = go.getComponent(SpriteRenderer.class);
        if(spr != null) {
            add(spr);
        }
    }

    public void add(SpriteRenderer spr) {
        boolean added = false;
        for(RenderBatch batch : batches) {
            if(batch.hasRoom() && batch.getzIndex() == spr.gameObject.getzIndex()) {
                Texture2D tex = spr.getTexture();
                if(tex != null || batch.hasTextures(tex) || batch.hasTextureRoom()) {
                    batch.addSprite(spr);
                    added = true;
                    break;
                }
            }
        }

        if(!added) {
            RenderBatch newBatch = new RenderBatch(MAX_BATCH_SIZE, spr.gameObject.getzIndex());
            newBatch.start();
            newBatch.addSprite(spr);
            batches.add(newBatch);
            Collections.sort(batches, Collections.reverseOrder());
        }
    }
}
