#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

//texture prop
uniform bool isGround;
uniform float moveX;
uniform float moveY;

// Uniforms for light properties
uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;
out vec2 texcoord;

out vec3 world_position;
out vec3 world_n;

void main()
{
    color = object_color;
    texcoord = v_texture_coord;

    if(isGround)
    {
        texcoord.x += moveX;
        texcoord.y += moveY;
    }

   
    world_position = (Model * vec4(v_position,1)).xyz;
    world_n = normalize( mat3( Model) * v_normal );

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
