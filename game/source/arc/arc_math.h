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

internal m4 M4RotateQuat(v4 left)
{
	m4 result;
	
	float XX, YY, ZZ,
	XY, XZ, YZ,
	WX, WY, WZ;
	
    XX = left.x * left.x;
    YY = left.y * left.y;
    ZZ = left.z * left.z;
    XY = left.x * left.y;
    XZ = left.x * left.z;
    YZ = left.y * left.z;
    WX = left.w * left.x;
    WY = left.w * left.y;
    WZ = left.w * left.z;
	
    result.elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    result.elements[1][0] = 2.0f * (XY + WZ);
    result.elements[2][0] = 2.0f * (XZ - WY);
    result.elements[3][0] = 0.0f;
	
    result.elements[0][1] = 2.0f * (XY - WZ);
    result.elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    result.elements[2][1] = 2.0f * (YZ + WX);
    result.elements[3][1] = 0.0f;
	
    result.elements[0][2] = 2.0f * (XZ + WY);
    result.elements[1][2] = 2.0f * (YZ - WX);
    result.elements[2][2] = 1.0f - 2.0f * (XX + YY);
    result.elements[3][2] = 0.0f;
	
    result.elements[0][3] = 0.0f;
    result.elements[1][3] = 0.0f;
    result.elements[2][3] = 0.0f;
    result.elements[3][3] = 1.0f;
	
	return result;
}