#type vertex
#version 330 core

/* a = attribute
 *
 */
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexID;

out vec4 fColor;
out vec2 fTexCoord;
out float fTexID;

uniform mat4 uProj;
uniform mat4 uView;

void main()
{
    fColor = aColor;
    fTexCoord = aTexCoord;
    fTexID = aTexID;
    gl_Position = uProj * uView * vec4(aPos, 0.0, 1.0);
}
    #type fragment
    #version 330 core

in vec4 fColor;
in vec2 fTexCoord;
in float fTexID;

uniform sampler2D uTextures[8];

out vec4 color;


void main()
{
    if (fTexID > 0) {
        int id = int(fTexID);
        color = fColor * texture(uTextures[id], fTexCoord);
        //color = vec4(fTexCoord, 0, 1);
    } else {
        color = fColor;
    }
}

