#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uVP;
uniform vec3 handleActive;

out vec3 VertColor;

void main()
{
    vec3 halfMask = vec3(0.6);

    vec3 white = vec3(1.0, 1.0, 1.0);
    vec3 red = vec3(1.0, 0.0, 0.0);
    vec3 green = vec3(0.0, 1.0, 0.0);
    vec3 blue = vec3(0.0, 0.0, 1.0);

    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    VertColor = aColor;
    VertColor = round(VertColor);
    
    if (VertColor != white && 
        (VertColor * handleActive.x) != red &&
        (VertColor * handleActive.y) != green &&
        (VertColor * handleActive.z) != blue) {
        VertColor *= halfMask;
    }
}
#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 VertColor;

uniform vec4 objectColor;

void main()
{
    vec3 result = objectColor.xyz * VertColor.xyz;
    FragColor = vec4(result, objectColor.w);
}
