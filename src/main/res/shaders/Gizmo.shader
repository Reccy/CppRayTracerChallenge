#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uVP;
uniform vec3 handleActive;

out vec3 VertColor;

void main()
{
    const vec3 fullMask = vec3(1.0);
    const vec3 halfMask = vec3(0.6);

    const vec3 white = vec3(1.0, 1.0, 1.0);
    const vec3 red = vec3(1.0, 0.0, 0.0);
    const vec3 green = vec3(0.0, 1.0, 0.0);
    const vec3 blue = vec3(0.0, 0.0, 1.0);

    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    VertColor = aColor;
    VertColor = round(VertColor);

    // Calculate the comparison results as floats (1.0 for true, 0.0 for false)
    float isWhite = float(all(equal(VertColor, white)));
    float isRed = float(all(equal(VertColor * handleActive.x, red)));
    float isGreen = float(all(equal(VertColor * handleActive.y, green)));
    float isBlue = float(all(equal(VertColor * handleActive.z, blue)));

    // Combine the results
    float mask = isWhite + isRed + isGreen + isBlue;

    // Clamp the mask to ensure it's either 0.0 or 1.0
    mask = clamp(mask, 0.0, 1.0);
    
    // Perform the mix operation using the calculated mask
    VertColor *= mix(halfMask, fullMask, mask);
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
