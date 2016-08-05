//#version 330
uniform mat3 matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 interpolatedTextureCoordinates;

void main() {
    interpolatedTextureCoordinates = textureCoordinates;
    interpolatedTextureCoordinates.x = 1.0 - interpolatedTextureCoordinates.x;
    interpolatedTextureCoordinates.y = 1.0 - interpolatedTextureCoordinates.y;
    gl_Position = vec4(matrix*(position.xyw), 0.0).xywz;;
}
