#version 320 es

precision highp float;

// Input data
in vec4 v_color;

// Output color
out vec4 out_color;

void main()
{
    // Pass the color from the vertex shader to the output color
    out_color = v_color;
}