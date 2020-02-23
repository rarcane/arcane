/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#define DegreesToRadians(a) (PI / 180.f * (a))
#define RadiansToDegrees(a) (180.f / PI * (a))
#define PI                              3.1415926535897f
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PI  0.3989422804f
#define EULERS_NUMBER                   2.7182818284590452353602874713527f
#define PercentDifference(v1, v2) (AbsoluteValue(v1 - v2) / v1)

#define Sin                        sinf
#define Cos                        cosf
#define Tan                        tanf
#define Pow                        powf
#define FMod                       fmodf
#define AbsoluteValue              fabsf
#define AbsoluteValueI             abs
#define SquareRoot                 sqrtf

TS_DOC_DESC("An integer 2-vector type.")
typedef union IV2
{
    struct
    {
        i32 x;
        i32 y;
    };
    struct
    {
        i32 w;
        i32 h;
    };
    struct
    {
        i32 width;
        i32 height;
    };
    i32 elements[2];
}
IV2;

TS_DOC_DESC("A floating point 2-vector type.")
typedef union V2
{
    struct
    {
        f32 x;
        f32 y;
    };
    struct
    {
        f32 w;
        f32 h;
    };
    struct
    {
        f32 width;
        f32 height;
    };
    f32 elements[2];
}
V2;

TS_DOC_DESC("An floating point 3-vector type.")
typedef union V3
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
    };
    struct
    {
        f32 r;
        f32 g;
        f32 b;
    };
    struct
    {
        f32 red;
        f32 green;
        f32 blue;
    };
    struct
    {
        f32 w;
        f32 h;
        union {
            f32 l;
            f32 d;
        };
    };
    struct
    {
        f32 width;
        f32 height;
        union {
            f32 length;
            f32 depth;
        };
    };
    f32 elements[3];
}
V3;

TS_DOC_DESC("An floating point 4-vector type.")
typedef union V4
{
    struct
    {
        f32 x;
        f32 y;
        union
        {
            struct
            {
                f32 z;
                f32 w;
            };
            struct
            {
                f32 width;
                f32 height;
            };
        };
    };
    struct
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    struct
    {
        f32 red;
        f32 green;
        f32 blue;
        f32 alpha;
    };
    struct
    {
        V2 xy;
        V2 zw;
    };
    f32 elements[4];
}
V4;

TS_DOC_DESC("An floating point 4x4 matrix type.")
typedef union M4
{
    struct
    {
        V4 row1;
        V4 row2;
        V4 row3;
        V4 row4;
    };
    f32 elements[4][4];
}
M4;

TS_DOC_DESC("An floating point 3x3 matrix type.")
typedef union M3
{
    struct
    {
        V3 row1;
        V3 row2;
        V3 row3;
    };
    f32 elements[3][3];
}
M3;

typedef IV2  iv2;
typedef V2   v2;
typedef V3   v3;
typedef V4   v4;
typedef M3   m3;
typedef M4   m4;

TS_DOC_DESC("A type for storing three-dimensional axis-aligned bounding boxes..")
typedef union BoundingBox BoundingBox;
union BoundingBox
{
    struct
    {
        v3 min;
        v3 max;
    };
};

TS_DOC_DESC("Returns a uniformly-random floating point number in some range.")
internal f32
RandomF32(f32 low   TS_DOC_PARAM("The lower-bound of the random number."),
          f32 high  TS_DOC_PARAM("The upper-bound of the random number."))
{
    return low + ((f32)(((int)rand() % 1000) / 1000.f) * (high - low));
}

TS_DOC_DESC("Returns a uniformly-random integer in some inclusive range.")
internal i32
RandomI32(i32 low   TS_DOC_PARAM("The inclusive lower-bound of the random number."),
          i32 high  TS_DOC_PARAM("The inclusive upper-bound of the random number."))
{
    return low + (int)rand() % ((high + 1) - low);
}

#define ClampFunc(name, type)                  \
type Clamp##name(type a, type low, type high)  \
{                                              \
if(a < low)  a = low;                      \
if(a > high) a = high;                     \
return a;                                  \
}
ClampFunc(F32, f32)
ClampFunc(I32, i32)
#undef ClampFunc

#define MinmaxFunc(type, name)                                         \
internal type Minimum##name(type a, type b) { return a < b ? a : b; }  \
internal type Maximum##name(type a, type b) { return a > b ? a : b; }
MinmaxFunc(i32, I32)
MinmaxFunc(u32, U32)
MinmaxFunc(u64, U64)
MinmaxFunc(f32, F32)
MinmaxFunc(f64, F64)
#undef MinmaxFunc

internal f32
Distance(f32 x1, f32 y1, f32 x2, f32 y2)
{
    return SquareRoot((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

internal f32
DistanceSquared(f32 x1, f32 y1, f32 x2, f32 y2)
{
    return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

#define IV2(x, y) IV2Init(x, y)
#define iv2(x, y) IV2(x, y)
internal IV2
IV2Init(i32 x, i32 y)
{
    IV2 v = {x, y};
    return v;
}

#define V2(x, y) V2Init(x, y)
#define v2(x, y) V2(x, y)
internal V2
V2Init(f32 x, f32 y)
{
    V2 v = {x, y};
    return v;
}

#define V3(x, y, z) V3Init(x, y, z)
#define v3(x, y, z) V3(x, y, z)
internal V3
V3Init(f32 x, f32 y, f32 z)
{
    V3 v = {x, y, z};
    return v;
}

#define V4(x, y, z, w) V4Init(x, y, z, w)
#define V4u(x) V4InitUniform(x)
#define v4(x, y, z, w) V4(x, y, z, w)
#define v4u(x) V4InitUniform(x)
internal V4
V4Init(f32 x, f32 y, f32 z, f32 w)
{
    V4 v = {x, y, z, w};
    return v;
}
internal V4
V4InitUniform(f32 x)
{
    V4 v = {x, x, x, x};
    return v;
}

#define IV2IV2OpFunc(name, op)                        \
internal IV2 IV2##name##IV2(IV2 a, IV2 b)             \
{                                                     \
IV2 result = {a.x op b.x, a.y op b.y};            \
return result;                                    \
}

#define IV2I32OpFunc(name, op)                        \
internal IV2 IV2##name##I32(IV2 a, i32 v)             \
{                                                     \
IV2 result = {a.x op v, a.y op v};                \
return result;                                    \
}

IV2IV2OpFunc(Add, +)
IV2I32OpFunc(Add, +)
IV2IV2OpFunc(Subtract, -)
IV2I32OpFunc(Subtract, -)
IV2IV2OpFunc(Multiply, *)
IV2I32OpFunc(Multiply, *)
IV2IV2OpFunc(Divide, /)
IV2I32OpFunc(Divide, /)

#undef IV2IV2OpFunc
#undef IV2I32OpFunc

#define V2V2OpFunc(name, op)                         \
internal V2 V2##name##V2(V2 a, V2 b)                 \
{                                                    \
V2 result = {a.x op b.x, a.y op b.y};        \
return result;                               \
}

#define V2F32OpFunc(name, op)                        \
internal V2 V2##name##F32(V2 a, f32 v)               \
{                                                    \
V2 result = {a.x op v, a.y op v};            \
return result;                               \
}

V2V2OpFunc (Add, +)
V2F32OpFunc(Add, +)
V2V2OpFunc (Subtract, -)
V2F32OpFunc(Subtract, -)
V2V2OpFunc (Multiply, *)
V2F32OpFunc(Multiply, *)
V2V2OpFunc (Divide, /)
V2F32OpFunc(Divide, /)

#undef V2V2OpFunc
#undef V2F32OpFunc

#define V3V3OpFunc(name, op)                                 \
internal V3 V3##name##V3(V3 a, V3 b)                         \
{                                                            \
V3 result = {a.x op b.x, a.y op b.y, a.z op b.z};    \
return result;                                       \
}
#define V3F32OpFunc(name, op)                                \
internal V3 V3##name##F32(V3 a, f32 v)                       \
{                                                            \
V3 result = {a.x op v, a.y op v, a.z op v};          \
return result;                                       \
}

V3V3OpFunc (Add, +)
V3F32OpFunc(Add, +)
V3V3OpFunc (Subtract, -)
V3F32OpFunc(Subtract, -)
V3V3OpFunc (Multiply, *)
V3F32OpFunc(Multiply, *)
V3V3OpFunc (Divide, /)
V3F32OpFunc(Divide, /)

#undef V3V3OpFunc
#undef V3F32OpFunc

#define V4V4OpFunc(name, op)                                            \
internal V4 V4##name##V4(V4 a, V4 b)                                    \
{                                                                       \
V4 result = {a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w};   \
return result;                                                  \
}
#define V4F32OpFunc(name, op)                                           \
internal V4 V4##name##F32(V4 a, f32 v)                                  \
{                                                                       \
V4 result = {a.x op v, a.y op v, a.z op v, a.w op v};           \
return result;                                                  \
}

V4V4OpFunc (Add, +)
V4F32OpFunc(Add, +)
V4V4OpFunc (Subtract, -)
V4F32OpFunc(Subtract, -)
V4V4OpFunc (Multiply, *)
V4F32OpFunc(Multiply, *)
V4V4OpFunc (Divide, /)
V4F32OpFunc(Divide, /)

#undef V4V4OpFunc
#undef V4F32OpFunc

internal f32
V2LengthSquared(V2 a)
{
    return a.x * a.x + a.y * a.y;
}

internal f32
V2Dot(v2 a, v2 b)
{
    return a.x*b.x + a.y*b.y;
}

internal f32
V3LengthSquared(v3 v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

internal f32
V3Length(v3 v)
{
    f32 length = SquareRoot(V3LengthSquared(v));
    return length;
}

internal f32
V3Dot(v3 a, v3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

internal v3
V3Cross(v3 a, v3 b)
{
    v3 c =
    {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x,
    };
    return c;
}

internal v3
V3Normalize(v3 v)
{
    f32 v_length = V3Length(v);
    v3 result =
    {
        v.x /= v_length,
        v.y /= v_length,
        v.z /= v_length,
    };
    return result;
}

internal f32
MaximumInV3(v3 vec)
{
    f32 max_val = vec.x;
    if(vec.y > max_val)
    {
        max_val = vec.y;
    }
    if(vec.z > max_val)
    {
        max_val = vec.z;
    }
    return max_val;
}

internal f32
MinimumInV3(v3 vec)
{
    f32 min_val = vec.x;
    if(vec.y < min_val)
    {
        min_val = vec.y;
    }
    if(vec.z < min_val)
    {
        min_val = vec.z;
    }
    return min_val;
}

internal m3
M3InitD(f32 diagonal)
{
    m3 m =
    {
        {
            { diagonal                },
            { 0.f, diagonal           },
            { 0.f, 0.f, diagonal      },
        }
    };
    return m;
}

internal m4
M4InitD(f32 diagonal)
{
    m4 m =
    {
        {
            { diagonal                },
            { 0.f, diagonal           },
            { 0.f, 0.f, diagonal      },
            { 0.f, 0.f, 0.f, diagonal },
        }
    };
    return m;
}

internal m4
M4MultiplyM4(m4 a, m4 b)
{
    m4 c = {0};
    
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
        {
            c.elements[i][j] = (a.elements[0][j]*b.elements[i][0] +
                                a.elements[1][j]*b.elements[i][1] +
                                a.elements[2][j]*b.elements[i][2] +
                                a.elements[3][j]*b.elements[i][3]);
        }
    }
    
    return c;
}

internal m4
M4MultiplyF32(m4 a, f32 b)
{
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
        {
            a.elements[i][j] *= b;
        }
    }
    
    return a;
}

internal f32
V4Dot(v4 a, v4 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

internal v4
V4MultiplyM4(v4 v, m4 m)
{
    v4 result = {0};
    
    for(int i = 0; i < 4; ++i)
    {
        result.elements[i] = (v.elements[0]*m.elements[0][i] +
                              v.elements[1]*m.elements[1][i] +
                              v.elements[2]*m.elements[2][i] +
                              v.elements[3]*m.elements[3][i]);
    }
    
    return result;
}

internal m3
M3MultiplyM3(m3 a, m3 b)
{
    m3 c = {0};
    
    for(int j = 0; j < 3; ++j)
    {
        for(int i = 0; i < 3; ++i)
        {
            c.elements[i][j] = (a.elements[0][j]*b.elements[i][0] +
                                a.elements[1][j]*b.elements[i][1] +
                                a.elements[2][j]*b.elements[i][2]);
        }
    }
    
    return c;
}

internal m3
M3Rotate(f32 angle, v3 axis)
{   
    m3 result = M3InitD(1.f);
    
    f32 axis_length = V3Length(axis);
    axis.x /= axis_length;
    axis.y /= axis_length;
    axis.z /= axis_length;
    
    f32 sin_theta = Sin(DegreesToRadians(angle));
    f32 cos_theta = Cos(DegreesToRadians(angle));
    f32 cos_value = 1.f - cos_theta;
    
    result.elements[0][0] = (axis.x * axis.x * cos_value) + cos_theta;
    result.elements[0][1] = (axis.x * axis.y * cos_value) + (axis.z * sin_theta);
    result.elements[0][2] = (axis.x * axis.z * cos_value) - (axis.y * sin_theta);
    result.elements[1][0] = (axis.y * axis.x * cos_value) - (axis.z * sin_theta);
    result.elements[1][1] = (axis.y * axis.y * cos_value) + cos_theta;
    result.elements[1][2] = (axis.y * axis.z * cos_value) + (axis.x * sin_theta);
    result.elements[2][0] = (axis.z * axis.x * cos_value) + (axis.y * sin_theta);
    result.elements[2][1] = (axis.z * axis.y * cos_value) - (axis.x * sin_theta);
    result.elements[2][2] = (axis.z * axis.z * cos_value) + cos_theta;
    
    return result;
}

internal m4
M4TranslateV3(v3 translation)
{
    m4 result = M4InitD(1.f);
    result.elements[3][0] = translation.x;
    result.elements[3][1] = translation.y;
    result.elements[3][2] = translation.z;
    return result;
}

internal m4
M4ScaleV3(v3 scale)
{
    m4 result = M4InitD(1.f);
    result.elements[0][0] = scale.x;
    result.elements[1][1] = scale.y;
    result.elements[2][2] = scale.z;
    return result;
}

internal M4
M4Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_depth, f32 far_depth)
{
    M4 result = {0};
    
    result.elements[0][0] = 2.f / (right - left);
    result.elements[1][1] = 2.f / (top - bottom);
    result.elements[2][2] = -2.f / (far_depth - near_depth);
    result.elements[3][3] = 1.f;
    result.elements[3][0] = (left + right) / (left - right);
    result.elements[3][1] = (bottom + top) / (bottom - top);
    result.elements[3][2] = (far_depth + near_depth) / (near_depth - far_depth);
    
    return result;
}

internal m4
M4Perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z)
{
    m4 result = {0};
    f32 tan_theta_over_2 = Tan(fov * (PI / 360.f));
    result.elements[0][0] = 1.f / tan_theta_over_2;
    result.elements[1][1] = aspect_ratio / tan_theta_over_2;
    result.elements[2][3] = -1.f;
    result.elements[2][2] = (near_z + far_z) / (near_z - far_z);
    result.elements[3][2] = (2.f * near_z * far_z) / (near_z - far_z);
    result.elements[3][3] = 0.f;
    return result;
}

internal m4
M4LookAt(v3 eye, v3 center, v3 up)
{
    m4 result;
    
    v3 f = V3Normalize(V3SubtractV3(center, eye));
    v3 s = V3Normalize(V3Cross(f, up));
    v3 u = V3Cross(s, f);
    
    result.elements[0][0] = s.x;
    result.elements[0][1] = u.x;
    result.elements[0][2] = -f.x;
    result.elements[0][3] = 0.0f;
    
    result.elements[1][0] = s.y;
    result.elements[1][1] = u.y;
    result.elements[1][2] = -f.y;
    result.elements[1][3] = 0.0f;
    
    result.elements[2][0] = s.z;
    result.elements[2][1] = u.z;
    result.elements[2][2] = -f.z;
    result.elements[2][3] = 0.0f;
    
    result.elements[3][0] = -V3Dot(s, eye);
    result.elements[3][1] = -V3Dot(u, eye);
    result.elements[3][2] = V3Dot(f, eye);
    result.elements[3][3] = 1.0f;
    
    return result;
}

internal m4
M4Inverse(m4 m)
{
    f32 coef00 = m.elements[2][2] * m.elements[3][3] - m.elements[3][2] * m.elements[2][3];
    f32 coef02 = m.elements[1][2] * m.elements[3][3] - m.elements[3][2] * m.elements[1][3];
    f32 coef03 = m.elements[1][2] * m.elements[2][3] - m.elements[2][2] * m.elements[1][3];
    f32 coef04 = m.elements[2][1] * m.elements[3][3] - m.elements[3][1] * m.elements[2][3];
    f32 coef06 = m.elements[1][1] * m.elements[3][3] - m.elements[3][1] * m.elements[1][3];
    f32 coef07 = m.elements[1][1] * m.elements[2][3] - m.elements[2][1] * m.elements[1][3];
    f32 coef08 = m.elements[2][1] * m.elements[3][2] - m.elements[3][1] * m.elements[2][2];
    f32 coef10 = m.elements[1][1] * m.elements[3][2] - m.elements[3][1] * m.elements[1][2];
    f32 coef11 = m.elements[1][1] * m.elements[2][2] - m.elements[2][1] * m.elements[1][2];
    f32 coef12 = m.elements[2][0] * m.elements[3][3] - m.elements[3][0] * m.elements[2][3];
    f32 coef14 = m.elements[1][0] * m.elements[3][3] - m.elements[3][0] * m.elements[1][3];
    f32 coef15 = m.elements[1][0] * m.elements[2][3] - m.elements[2][0] * m.elements[1][3];
    f32 coef16 = m.elements[2][0] * m.elements[3][2] - m.elements[3][0] * m.elements[2][2];
    f32 coef18 = m.elements[1][0] * m.elements[3][2] - m.elements[3][0] * m.elements[1][2];
    f32 coef19 = m.elements[1][0] * m.elements[2][2] - m.elements[2][0] * m.elements[1][2];
    f32 coef20 = m.elements[2][0] * m.elements[3][1] - m.elements[3][0] * m.elements[2][1];
    f32 coef22 = m.elements[1][0] * m.elements[3][1] - m.elements[3][0] * m.elements[1][1];
    f32 coef23 = m.elements[1][0] * m.elements[2][1] - m.elements[2][0] * m.elements[1][1];
    
    v4 fac0 = { coef00, coef00, coef02, coef03 };
    v4 fac1 = { coef04, coef04, coef06, coef07 };
    v4 fac2 = { coef08, coef08, coef10, coef11 };
    v4 fac3 = { coef12, coef12, coef14, coef15 };
    v4 fac4 = { coef16, coef16, coef18, coef19 };
    v4 fac5 = { coef20, coef20, coef22, coef23 };
    
    v4 vec0 = { m.elements[1][0], m.elements[0][0], m.elements[0][0], m.elements[0][0] };
    v4 vec1 = { m.elements[1][1], m.elements[0][1], m.elements[0][1], m.elements[0][1] };
    v4 vec2 = { m.elements[1][2], m.elements[0][2], m.elements[0][2], m.elements[0][2] };
    v4 vec3 = { m.elements[1][3], m.elements[0][3], m.elements[0][3], m.elements[0][3] };
    
    v4 inv0 = V4AddV4(V4SubtractV4(V4MultiplyV4(vec1, fac0), V4MultiplyV4(vec2, fac1)), V4MultiplyV4(vec3, fac2));
    v4 inv1 = V4AddV4(V4SubtractV4(V4MultiplyV4(vec0, fac0), V4MultiplyV4(vec2, fac3)), V4MultiplyV4(vec3, fac4));
    v4 inv2 = V4AddV4(V4SubtractV4(V4MultiplyV4(vec0, fac1), V4MultiplyV4(vec1, fac3)), V4MultiplyV4(vec3, fac5));
    v4 inv3 = V4AddV4(V4SubtractV4(V4MultiplyV4(vec0, fac2), V4MultiplyV4(vec1, fac4)), V4MultiplyV4(vec2, fac5));
    
    v4 sign_a = { +1, -1, +1, -1 };
    v4 sign_b = { -1, +1, -1, +1 };
    
    m4 inverse;
    for(u32 i = 0; i < 4; ++i)
    {
        inverse.elements[0][i] = inv0.elements[i] * sign_a.elements[i];
        inverse.elements[1][i] = inv1.elements[i] * sign_b.elements[i];
        inverse.elements[2][i] = inv2.elements[i] * sign_a.elements[i];
        inverse.elements[3][i] = inv3.elements[i] * sign_b.elements[i];
    }
    
    v4 row0 = { inverse.elements[0][0], inverse.elements[1][0], inverse.elements[2][0], inverse.elements[3][0] };
    v4 m0 = { m.elements[0][0], m.elements[0][1], m.elements[0][2], m.elements[0][3] };
    v4 dot0 = V4MultiplyV4(m0, row0);
    f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
    
    f32 one_over_det = 1 / dot1;
    
    return M4MultiplyF32(inverse, one_over_det);
}

internal m4
M4RemoveRotation(m4 mat)
{
    v3 scale = {
        V3Length(v3(mat.elements[0][0], mat.elements[0][1], mat.elements[0][2])),
        V3Length(v3(mat.elements[1][0], mat.elements[1][1], mat.elements[1][2])),
        V3Length(v3(mat.elements[2][0], mat.elements[2][1], mat.elements[2][2])),
    };
    
    mat.elements[0][0] = scale.x;
    mat.elements[1][0] = 0.f;
    mat.elements[2][0] = 0.f;
    
    mat.elements[0][1] = 0.f;
    mat.elements[1][1] = scale.y;
    mat.elements[2][1] = 0.f;
    
    mat.elements[0][2] = 0.f;
    mat.elements[1][2] = 0.f;
    mat.elements[2][2] = scale.z;
    
    return mat;
}

internal m4
M4Rotate(f32 angle, v3 axis)
{   
    m4 result = M4InitD(1.f);
    
    f32 axis_length = V3Length(axis);
    axis.x /= axis_length;
    axis.y /= axis_length;
    axis.z /= axis_length;
    
    f32 sin_theta = Sin(DegreesToRadians(angle));
    f32 cos_theta = Cos(DegreesToRadians(angle));
    f32 cos_value = 1.f - cos_theta;
    
    result.elements[0][0] = (axis.x * axis.x * cos_value) + cos_theta;
    result.elements[0][1] = (axis.x * axis.y * cos_value) + (axis.z * sin_theta);
    result.elements[0][2] = (axis.x * axis.z * cos_value) - (axis.y * sin_theta);
    result.elements[1][0] = (axis.y * axis.x * cos_value) - (axis.z * sin_theta);
    result.elements[1][1] = (axis.y * axis.y * cos_value) + cos_theta;
    result.elements[1][2] = (axis.y * axis.z * cos_value) + (axis.x * sin_theta);
    result.elements[2][0] = (axis.z * axis.x * cos_value) + (axis.y * sin_theta);
    result.elements[2][1] = (axis.z * axis.y * cos_value) - (axis.x * sin_theta);
    result.elements[2][2] = (axis.z * axis.z * cos_value) + cos_theta;
    
    return result;
}

#define BoundingBox(min, max) BoundingBoxInit((min), (max))
internal BoundingBox
BoundingBoxInit(v3 min, v3 max)
{
    BoundingBox bounding_box = {0};
    bounding_box.min = min;
    bounding_box.max = max;
    return bounding_box;
}

internal v3
BoundingBoxCenter(BoundingBox bounding_box)
{
    v3 v = V3DivideF32(V3AddV3(bounding_box.min, bounding_box.max), 2);
    return v;
}

internal v3
RGBToHSV(v3 rgb)
{
    f32 c_max = MaximumInV3(rgb);
    f32 c_min = MinimumInV3(rgb);
    f32 delta = c_max - c_min;
    b32 c_max_is_r = rgb.r > rgb.g && rgb.r > rgb.b;
    b32 c_max_is_g = rgb.g > rgb.r && rgb.g > rgb.b;
    b32 c_max_is_b = rgb.b > rgb.r && rgb.b > rgb.g;
    f32 h = (c_max_is_r ? (rgb.g - rgb.b) / delta + 0 : c_max_is_g ? (rgb.b - rgb.r) / delta + 2 : c_max_is_b ? (rgb.r - rgb.g) / delta + 4 : 0);
    f32 s = c_max == 0 ? 0 : (delta / c_max);
    f32 v = c_max;
    v3 hsv = {h / 6.f, s, v};
    return hsv;
}

internal v3
HSVToRGB(v3 hsv)
{
    f32 h = FMod(hsv.x * 360.f, 360.f);
    f32 s = hsv.y;
    f32 v = hsv.z;
    
    f32 c = v * s;
    f32 x = c * (1 - AbsoluteValue(FMod((h / 60.f), 2) - 1));
    f32 m = v - c;
    
    f32 r;
    f32 g;
    f32 b;
    
    if(h >= 0.f && h < 60.f)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if(h >= 60.f && h < 120.f)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if(h >= 120.f && h < 180.f)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if(h >= 180.f && h < 240.f)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if(h >= 240.f && h < 300.f)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if(h >= 300.f && h <= 360.f)
    {
        r = c;
        g = 0;
        b = x;
    }
    
    v3 rgb = {r + m, g + m, b + m};
    return rgb;
}

/*c
t = 2 * abs((time() / 4) % 1 - 0.5)

plot_xaxis('t', -0.25, 1.25)
plot_yaxis('v', -0.25, 1.25)

plot_title('Linear')
v = t
plot(x, [[t],[v]], v)

plot_title('Smooth')
v = (4/9)*t^6 - (17/9)*t^4 + (22/9)*t^2
plot((4/9)*x^6 - (17/9)*x^4 + (22/9)*x^2, [[t],[v]], v)
*/

internal f32
InterpolateLinear(f32 t)
{
    f32 result = 0;
    if(t < 0)
    {
        result = 0;
    }
    else if(t > 1)
    {
        result = 1;
    }
    else
    {
        result = t;
    }
    return result;
}

internal f32
InterpolateSmooth(f32 t)
{
    f32 result = 0;
    if(t < 0)
    {
        result = 0;
    }
    else if(t > 1)
    {
        result = 1;
    }
    else
    {
        f32 t2 = t * t;
        f32 t4 = t2 * t2;
        f32 t6 = t2 * t4;
        result = (4 / 9) * t6 - (17 / 9) * t4 + (22 / 9) * t2;
    }
    return result;
}

internal b32
V4sIntersect(v4 a, v4 b)
{
    return (a.x + a.width >= b.x &&
            a.x <= b.x + b.width &&
            a.y + a.height >= b.y &&
            a.y <= b.y + b.height);
}

internal b32
V4HasPoint(v4 v, v2 p)
{
    return (p.x >= v.x && p.x <= v.x + v.width &&
            p.y >= v.y && p.y <= v.y + v.height);
}

internal v2
V4Center(v4 v)
{
    v2 result =
    {
        v.x + v.width/2,
        v.y + v.height/2,
    };
    
    return result;
}

internal b32
BoundingBoxesIntersect(f32 x1, f32 y1, f32 w1, f32 h1,
                       f32 x2, f32 y2, f32 w2, f32 h2)
{
    return (x1 + w1 >= x2 && x1 <= x2 + w2 &&
            y1 + h1 >= y2 && y2 <= y2 + h2);
}
