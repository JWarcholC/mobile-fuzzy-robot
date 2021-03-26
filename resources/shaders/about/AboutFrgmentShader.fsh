varying highp vec2 qt_TexCoord0;

uniform highp float angle;
uniform sampler2D source;

highp float calcShift(highp float pos) {
    highp float wave = 0.01;
    return wave * sin(radians(angle + pos * 360.0));
}

void main() {
    highp vec2 pos;
    pos.x = qt_TexCoord0.x + calcShift(qt_TexCoord0.x);
    pos.y = qt_TexCoord0.y + calcShift(qt_TexCoord0.y);

    gl_FragColor = texture2D(source, pos);
}
