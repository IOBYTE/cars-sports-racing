varying vec3 normal;
varying vec3 pos;

uniform sampler2D diffusemap;
//uniform sampler2D shadowmap;

uniform vec4 specularColor;
uniform float smoothness;
uniform vec3 lightvector;
uniform vec4 lightpower;
uniform vec4 ambientColor;

void main()
{
    vec3 norma = normal;
    vec3 v = normalize(-pos);
    vec3 l = normalize(lightvector);
    vec3 h = normalize(l+v);
    vec3 reflectDirection = reflect(v, normal);

    float cosTh = max(0.0, dot(norma, h));
    float cosTi = max(0.0, dot(norma, l));

    vec4 diffuse =  texture2D(diffusemap, vec2(gl_TexCoord[0]));
    vec4 fcolor  =  ambientColor*diffuse+(diffuse+ smoothness*specularColor*pow(cosTh,smoothness))*lightpower*cosTi;
    vec4 final_color;

    final_color = fcolor;
    gl_FragColor  = final_color;
}
