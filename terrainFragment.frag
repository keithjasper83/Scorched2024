uniform sampler2D terrainTexture;

void main() {
    vec4 mask = texture(terrainTexture, gl_TexCoord[0].xy);
    gl_FragColor = gl_Color * mask;
}
