cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attLin;
    float attQuad;
    float attConst;
};

cbuffer ObjectCBuf
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};

float4 main(float3 worldPos : Position, float3 N : Normal) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - worldPos; // vector
    const float distToL = length(vToL); // distance
    const float3 L = vToL / distToL; // normalized
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(L, N));
    // reflected light vector
    const float3 R = 2.0f * N * dot(L, N) - L;
    // specular intensity
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * 
        pow(max(0.0f, dot(normalize(-R), normalize(worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient + specular) * materialColor), 1.0f);
}