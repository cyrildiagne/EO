//#version 330

uniform sampler2D textureData;
uniform int numChannels;

in vec2 interpolatedTextureCoordinates;

out vec4 fragmentColor;

void main() {
    if (numChannels==1) {
      float v = texture(textureData, interpolatedTextureCoordinates).r;
      fragmentColor.rgb = vec3(v,v,v);
    } else {
      fragmentColor.rgb = texture(textureData, interpolatedTextureCoordinates).rgb;
    }
    fragmentColor.a = 1.0;
}
