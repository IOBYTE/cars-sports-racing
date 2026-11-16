#define MODE_OFF 0
#define MODE_DIFFUSE 1
#define MODE_AMBIENT_AND_DIFFUSE 2

varying vec4 diffuse_term;
varying vec3 normal;

uniform int colorMode;

void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    normal = gl_NormalMatrix * gl_Normal;
    vec4 ambient_color, diffuse_color;

    if (colorMode == MODE_DIFFUSE)
    {
        diffuse_color = gl_Color;
        ambient_color = gl_FrontMaterial.ambient;
    }
    else if (colorMode == MODE_AMBIENT_AND_DIFFUSE)
    {
        diffuse_color = gl_Color;
        ambient_color = gl_Color;
    }
    else
    {
        diffuse_color = gl_FrontMaterial.diffuse;
        ambient_color = gl_FrontMaterial.ambient;
    }

    diffuse_term = diffuse_color * gl_LightSource[0].diffuse;
    vec4 ambient_term = ambient_color * gl_LightSource[0].ambient;

    if (gl_FrontMaterial.diffuse.a < 1.0)
        diffuse_term.a = gl_FrontMaterial.diffuse.a;
    else
        diffuse_term.a = gl_Color.a;

    gl_FrontColor.rgb = ambient_term.rgb;  gl_FrontColor.a = 1.0;
    gl_BackColor.rgb = ambient_term.rgb; gl_FrontColor.a = 0.0;
}
