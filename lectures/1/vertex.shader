#version 330 core

layout(location = 0) in vec3 position;

out vec4 col;

uniform float xMove;

void main()
{
	gl_Position = vec4(position.x*0.5 + xMove, position.y*0.5, position.z, 1.0);
	col = vec4(clamp(position, 0.0, 1.0), 1.0);
}
