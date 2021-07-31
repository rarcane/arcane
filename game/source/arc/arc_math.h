internal m4 M4Transpose(m4 mat)
{
	m4 result;
	
    for (i32 columns = 0; columns < 4; ++columns)
    {
        for (i32 rows = 0; rows < 4; ++rows)
        {
            result.elements[rows][columns] = mat.elements[columns][rows];
        }
    }
	
	return result;
}

internal v4 DivideQuatF(v4 quat, f32 f)
{
	return v4(quat.x / f, quat.y / f, quat.z / f, quat.w / f);
}

internal f32 DotQuat(v4 left, v4 right)
{
	return ((left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w));
}

internal v4 NormaliseQuat(v4 quat)
{
	f32 length = SquareRoot(DotQuat(quat, quat));
	return DivideQuatF(quat, length);
}

internal m3 M3RotateQuat(v4 q)
{
	q = NormaliseQuat(q);
	
	f32 q0 = q.x;
	f32 q1 = q.y;
	f32 q2 = q.z;
	f32 q3 = q.w;
	
    f32 r00 = 2 * (q0 * q0 + q1 * q1) - 1;
	f32 r01 = 2 * (q1 * q2 - q0 * q3);
	f32 r02 = 2 * (q1 * q3 + q0 * q2);
	
    f32 r10 = 2 * (q1 * q2 + q0 * q3);
	f32 r11 = 2 * (q0 * q0 + q2 * q2) - 1;
	f32 r12 = 2 * (q2 * q3 - q0 * q1);
    
	f32 r20 = 2 * (q1 * q3 - q0 * q2);
	f32 r21 = 2 * (q2 * q3 + q0 * q1);
	f32 r22 = 2 * (q0 * q0 + q3 * q3) - 1;
    
	m3 mat = {
		r00, r01, r02,
		r10, r11, r12,
		r20, r21, r22,
	};
	
	return mat;
}

internal m4 M4RotateQuat(v4 q)
{
	m4 result = M4InitD(1.0f);
	
	q = NormaliseQuat(q);
	
	float XX, YY, ZZ,
	XY, XZ, YZ,
	WX, WY, WZ;
	
	// NOTE(randy): Swapped around X and W because why the fuck can't everything be standard
    XX = q.w * q.w;
    YY = q.y * q.y;
    ZZ = q.z * q.z;
    XY = q.w * q.y;
    XZ = q.w * q.z;
    YZ = q.y * q.z;
    WX = q.x * q.w;
    WY = q.x * q.y;
    WZ = q.x * q.z;
	
    result.elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    result.elements[0][1] = 2.0f * (XY + WZ);
    result.elements[0][2] = 2.0f * (XZ - WY);
    result.elements[0][3] = 0.0f;
	
    result.elements[1][0] = 2.0f * (XY - WZ);
    result.elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    result.elements[1][2] = 2.0f * (YZ + WX);
    result.elements[1][3] = 0.0f;
	
    result.elements[2][0] = 2.0f * (XZ + WY);
    result.elements[2][1] = 2.0f * (YZ - WX);
    result.elements[2][2] = 1.0f - 2.0f * (XX + YY);
    result.elements[2][3] = 0.0f;
	
    result.elements[3][0] = 0.0f;
    result.elements[3][1] = 0.0f;
    result.elements[3][2] = 0.0f;
    result.elements[3][3] = 1.0f;
	
	// M4Transpose(result);
	
	return result;
}