#version 330 core

layout(location = 0) out vec4 color;
uniform vec2 u_C0;
uniform float u_AspectRatio = 1.0; // spax / spany
uniform float u_SpanY = 3.0;
uniform vec2  u_Center;
uniform int   u_Mode = 0;
uniform int u_Width;
uniform int u_Height;
// in vec2 v_texCoord;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
struct complex
{
    float x, y;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
float absSq(complex z)
{
    return z.x * z.x + z.y * z.y;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
complex conjugate(complex z)
{
    complex zbar = z;
    zbar.y *= -1.0;
    return zbar;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
complex mult(complex a, complex b)
{
    complex res;
    res.x = a.x * b.x - a.y * b.y;
    res.y = a.x * b.y + b.x * a.y;
    return res;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
complex exponent(complex z, int n)
{
    if ( n == 1 )
        return z;

    complex zn = z;
    int ii = 1;
    for ( ii = 1; ii < n; ++ii )
    {
        zn = mult(zn, z);
    }

    return zn;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vec2 getFragmentCoordinate(vec2 texcoord)
{
    float spanX = u_AspectRatio * u_SpanY;
    float spanY = u_SpanY;
    vec2 minCorner = u_Center - vec2(0.5 * spanX, 0.5 * spanY);

    // texcoord between 0 to 1
    float x = minCorner.x + texcoord.x * spanX;
    float y = minCorner.y + texcoord.y * spanY;
    return vec2(x, y);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vec4 juliaShader(vec2 texcoord)
{
    int iter = 100;

    vec2 z = getFragmentCoordinate(texcoord);

    int i = 0;

    for ( i = 0; i < iter; ++i )
    {
        float x = (z.x * z.x - z.y * z.y) + u_C0.x;
        float y = (z.y * z.x + z.x * z.y) + u_C0.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    vec4 r = vec4(0.5, 0.0, 0.0, 1.0);
    vec4 g = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 b = vec4(0.0, 0.0, 0.0, 1.0);

    float normi = float(i)/100.0;
    normi = 2.0 * normi - 1.0;
    normi = normi * normi;
    float mixfac = exp(-4.0*normi*normi);

    return g * mixfac + (1.0 - mixfac) * b;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vec4 mandelbrotShader(vec2 texcoord)
{
    int iter = 100;

    vec2 z = vec2(0.0);
    vec2 c0 = getFragmentCoordinate(texcoord);

    int i = 0;

    for ( i = 0; i < iter; ++i )
    {
        float x = (z.x * z.x - z.y * z.y) + c0.x;
        float y = (z.y * z.x + z.x * z.y) + c0.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    vec4 r = vec4(0.5, 0.0, 0.0, 1.0);
    vec4 g = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 b = vec4(0.0, 0.0, 0.0, 1.0);

    float normi = float(i)/100.0;
    normi = 2.0 * normi - 1.0;
    normi = normi * normi;
    float mixfac = exp(-4.0*normi*normi);

    // return vec4(1.0, 0.0, 0.0, 1.0);
    return g * mixfac + (1.0 - mixfac) * b;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
vec4 newtonShader(vec2 texcoord)
{
    int iter = 100;

    vec2 z = getFragmentCoordinate(texcoord);

    // z_n+1 -> z_n - f(z_n) / f'(z_n)
    // f: z^3 -2z + 2
    // f': 3z^2 - 2

    vec2 roots[3];
    roots[0] = vec2(-1.76929235f, 0.0);
    roots[1] = vec2(0.88464618, 0.58974281);
    roots[2] = vec2(0.88464618, -0.58974281);

    float tol = 0.00001;

    int i = 0;

    for ( i = 0; i < iter; ++i )
    {
        complex zn;
        zn.x = z.x;
        zn.y = z.y;

        complex fz = exponent(zn, 3);
        fz.x += 2.0;
        fz.x -= 2*zn.x;
        fz.y -= 2*zn.y;

        complex fprimez = exponent(zn,2);
        fprimez.x *= 3.0;
        fprimez.x -= 2.0;
        fprimez.y *= 3.0;

        complex fzbyfprimez = mult(fz, conjugate(fprimez));
        float len = absSq(fprimez);
        fzbyfprimez.x /= len;
        fzbyfprimez.y /= len;

        // apply newton's iteration
        z.x = z.x - fzbyfprimez.x;
        z.y = z.y - fzbyfprimez.y;

        int j;
        int foundroot = 0;
        for ( j = 0; j < 3; ++j)
        {
            if ( length(z-roots[j]) < tol )
            {
                foundroot = 1;
                break;
            }
        }

        if ( foundroot == 1 )
            break;
    }

    vec4 r = vec4(1.0, 0.5, 0.0, 1.0);
    vec4 g = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 b = vec4(0.5, 0.7, 0.1, 1.0);

    float a0 = 0.233;
    float a1 = 1.609;
    float normi = float(i)/(1.0 * iter);
    normi = pow(normi, a1);
    normi = 2.0 * normi - 1.0;
    float mixfac = a0 - exp(-2.0f * normi * normi);

    vec4 colors[3];
    colors[0] = r;
    colors[1] = g;
    colors[2] = b;

    vec2 fragcoord = getFragmentCoordinate(texcoord);

    for ( int ii = 0; ii < 3; ++ii )
    {
        vec2 diff = z - roots[ii];
        if ( abs(diff.x) < tol && abs(diff.y) < tol )
        {
            return colors[ii] * mixfac * 10.0;
        }
    }

    return vec4(vec3(0.0), 1.0) * vec4(vec3(mixfac), 1.0);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
float plot(float edge, float x)
{
    return smoothstep(edge-0.01, edge, x) - smoothstep(edge, edge+0.01, x);
}

// -----------------------------------------------------------------------------
vec4 gridlinesShader(vec2 texcoord)
{
    vec2 fragcoord = getFragmentCoordinate(texcoord);
    float e = round(fragcoord.x);
    float f = round(fragcoord.y);
    return vec4(vec2(clamp(max(plot(e, fragcoord.x), plot(f, fragcoord.y)), 0.0, 1.0)), 0.0, 1.0);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void main()
{
    vec2 v_texCoord = vec2((1.0 * gl_FragCoord.x)/u_Width, (1.0 * gl_FragCoord.y)/u_Height);
    if ( u_Mode == 0 )
        color = juliaShader(v_texCoord);
    else if ( u_Mode == 1 )
        color = mandelbrotShader(v_texCoord);
    else
        color = newtonShader(v_texCoord);

    if ( u_Mode != 2 )
        color = max(0.2 * gridlinesShader(v_texCoord), 0.9 * color);
}
