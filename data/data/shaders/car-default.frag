varying vec4 diffuse_term;
varying vec3 normal;

uniform sampler2D texture;

uniform int fogType;

vec3 fog_Func(vec3 color, int type);

float luminance(vec3 color)
{
    return dot(vec3(0.212671, 0.715160, 0.072169), color);
}

void main()
{
    vec3 n, halfV;
    float NdotL, NdotHV, fogFactor;
    vec4 color = gl_FrontLightModelProduct.sceneColor + gl_Color;
    vec3 lightDir = gl_LightSource[0].position.xyz;
    vec3 halfVector = gl_LightSource[0].halfVector.xyz;
    vec4 texel;
    vec4 fragColor;
    vec4 specular = vec4(0.0);

    n = (2.0 * gl_Color.a - 1.0) * normal;
    n = normalize(n);
    NdotL = dot(n, lightDir);

    if (NdotL > 0.0)
    {
        color += diffuse_term * NdotL;
        halfV = normalize(halfVector);
        NdotHV = max(dot(n, halfV), 0.0);

        if (gl_FrontMaterial.shininess > 0.0)
            specular.rgb = (gl_FrontMaterial.specular.rgb
                            * gl_LightSource[0].specular.rgb
                            * pow(NdotHV, gl_FrontMaterial.shininess));
    }

    color.a = diffuse_term.a;
    color = clamp(color, 0.0, 1.0);
    texel = texture2D(texture, gl_TexCoord[0].st);
    fragColor = color * texel + specular;
	fragColor.rgb = fog_Func(fragColor.rgb, fogType);
	gl_FragColor = fragColor;
}
