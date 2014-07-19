//#ifdef GL_FRAGMENT_PRECISION_HIGH
//precision highp float;
//#else
//precision mediump float;
//#endif
//
//void main()
//{
//    gl_FragColor = vec4(0, 0, 0, 0.03);
//}
uniform sampler2D texture;

void main()
{
    vec4 pixel_color = texture2D(texture, gl_TexCoord[0].xy);
    if(pixel_color.a < 0.049)
        pixel_color.a = 0;
    if(pixel_color.r < 0.049)
        pixel_color.r = 0;
    if(pixel_color.g < 0.049)
        pixel_color.g = 0;
    if(pixel_color.b < 0.049)
        pixel_color.b = 0;
    gl_FragColor = pixel_color;
}
//uniform sampler2D texture;
//
//void main()
//{
//    vec4 pixel_color = texture2D(texture, gl_TexCoord[0].xy);
//    pixel_color.a -= 0.3;
//    gl_FragColor = pixel_color;
//}
