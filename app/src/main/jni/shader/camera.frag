const char* camera_play_frag = STRINGIFY(

precision mediump float;

uniform sampler2D   u_Texture;
uniform int         u_FilterMode;
uniform vec3        u_ChangeColor;
uniform float       u_Weight;

varying vec2        v_TextCoord;
varying vec4        v_Position;

void modifyColor(vec4 color)
{
    color.r = max(min(color.r, 1.0), 0.0);
    color.g = max(min(color.g, 1.0), 0.0);
    color.b = max(min(color.b, 1.0), 0.0);
    color.a = max(min(color.a, 1.0), 0.0);
}

void main()
{
    vec4 nColor = texture2D(u_Texture, v_TextCoord);
    if (u_FilterMode == 0)      // 原图
    {
        gl_FragColor = nColor;
    }
    else if (u_FilterMode == 1) // 黑白
    {
        float c         = nColor.r * u_ChangeColor.r + nColor.g * u_ChangeColor.g + nColor.b * u_ChangeColor.b;
        gl_FragColor    = vec4(c, c, c, nColor.a);
    }
    else if(u_FilterMode == 2)  // 冷色调
    {
        vec4 deltaColor = nColor + vec4(u_ChangeColor, 0.0);
        modifyColor(deltaColor);
        gl_FragColor = deltaColor;
    }
    else if(u_FilterMode == 3)  // 暖色调
    {
        vec4 deltaColor = nColor + vec4(u_ChangeColor, 0.0);
        modifyColor(deltaColor);
        gl_FragColor = deltaColor;
    }
    else if(u_FilterMode == 4)  // 模糊
    {
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x - u_ChangeColor.r, v_TextCoord.y - u_ChangeColor.r));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x - u_ChangeColor.r, v_TextCoord.y + u_ChangeColor.r));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x + u_ChangeColor.r, v_TextCoord.y - u_ChangeColor.r));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x + u_ChangeColor.r, v_TextCoord.y + u_ChangeColor.r));

        nColor += texture2D(u_Texture, vec2(v_TextCoord.x - u_ChangeColor.g, v_TextCoord.y - u_ChangeColor.g));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x - u_ChangeColor.g, v_TextCoord.y + u_ChangeColor.g));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x + u_ChangeColor.g, v_TextCoord.y - u_ChangeColor.g));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x + u_ChangeColor.g, v_TextCoord.y + u_ChangeColor.g));

        nColor += texture2D(u_Texture, vec2(v_TextCoord.x - u_ChangeColor.b, v_TextCoord.y - u_ChangeColor.b));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x - u_ChangeColor.b, v_TextCoord.y + u_ChangeColor.b));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x + u_ChangeColor.b, v_TextCoord.y - u_ChangeColor.b));
        nColor += texture2D(u_Texture, vec2(v_TextCoord.x + u_ChangeColor.b, v_TextCoord.y + u_ChangeColor.b));
        nColor /= 13.0;
        gl_FragColor = nColor;
    }
    else                        // 放大镜
    {
        float dis = distance(vec2(v_Position.x, v_Position.y / u_Weight), vec2(u_ChangeColor.r, u_ChangeColor.g));
        if(dis < u_ChangeColor.b)
        {
            nColor = texture2D(u_Texture, vec2(v_TextCoord.x / 2.0 + 0.25, v_TextCoord.y / 2.0 + 0.25));
        }
        gl_FragColor = nColor;
    }
}
);