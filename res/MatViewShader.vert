//#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 interpolatedTextureCoordinates;

void main() {
    interpolatedTextureCoordinates = textureCoordinates;
    interpolatedTextureCoordinates.x = 1.0 - interpolatedTextureCoordinates.x;
    interpolatedTextureCoordinates.y = 1.0 - interpolatedTextureCoordinates.y;
    gl_Position = position;
}
