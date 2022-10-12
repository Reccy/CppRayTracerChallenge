#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uVP;
uniform vec3 lightColor;
uniform vec4 objectColor;

out vec3 VertColor;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    VertColor = mix(objectColor.xyz, lightColor, aColor.x);
}
#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 VertColor;

void main()
{
    FragColor = vec4(VertColor.xyz, 1);
}
