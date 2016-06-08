#pragma once

class CTDMap
{
public:
	float m_Pro_Matrix[3 * 4];//相机透射投影矩阵
	float m_scale;
	bool isok;
	float m_xScale;
	float m_yScale;
	float m_zScale;
	CTDMap()
	{
		for (int i = 0; i < 12; i++) m_Pro_Matrix[i] = 0;
		m_scale = 1;
		isok = false;
		m_xScale = 1;
		m_yScale = 1;
		m_zScale = 1;
	}
};

class CMyTDMap
{
public:
	static void transpos(float *A, float *B, int m, int n);  //求转置矩阵,B = AT  A为m行n列
	static void multiply(float *A, float *B, float *C, int m, int n, int o);//求矩阵乘积，C[m][o]=A[m][n]*B[n][o]
	static void inverse(float *A, float *B, int m);//求逆矩阵，B[m][m]=inv(A[m][m])
	static void Calculate3DtoImg(CTDMap mTDMap,const float realCoord[3], int imgPos[2]);	//已知当前点的3维坐标P(X,Y,Z)，求其2D坐标p(u,v)
	static void Calculate3DtoImg(CTDMap mTDMap, const float realCoord[3], float imgPos[2]);
	static void CalculateImgto3D(CTDMap mTDMap,const int imgPos[2],const float& Z, float realCoord[3]);
	static void CalculateImgto3D(CTDMap mTDMap, const float imgPos[2], const float& Z, float realCoord[3]);
	static bool ReadTDMapMatrix(const char* path, CTDMap *mTDMap);
	static bool SaveTDMapMatrix(const char* path, CTDMap mTDMap);
};
inline void CMyTDMap::transpos(float *A, float *B, int m, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			B[i*m + j] = A[j*n + i];
		}
	}
}
inline void CMyTDMap::multiply(float *A, float *B, float *C, int m, int n, int o)
{
	float temp = 0;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < o; j++)
		{
			temp = 0;
			for (int k = 0; k < n; k++)
			{
				temp += A[i*n + k] * B[k*o + j];
			}
			C[i*o + j] = temp;
		}
	}
}
inline void CMyTDMap::inverse(float *A, float *B, int m)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 2 * m;
	float *M = new float[m*n];
	memset(M, 0, sizeof(float)*m*n);
	float temp = 0;
	float b = 0;

	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (j <= m - 1)
			{
				M[i*n + j] = A[i*m + j];
			}
			else
			{
				if (i == j - m)
				{
					M[i*n + j] = 1;
				}
				else
				{
					M[i*n + j] = 0;
				}
			}
		}
	}


	for (k = 0; k<m - 1; k++)
	{
		////////////////////////////////
		if (M[k*n + k] == 0)
		{
			for (i = k + 1; i<m; i++)
			{
				if (M[i*n + k] != 0)
				{
					break;
				}
			}
			if (i >= m)
			{
				AfxMessageBox(_T("A矩阵不可逆"));
			}
			else
			{  	//--                                                                                                                                       
				/*for (j = 0;j <n;j++)
				{

				temp = M[k*n+j];
				M[k*n+j] = M[(k+1)*n+j];
				M[(k+1)*n+j] = temp;
				}*/
				///////////++++++++++++++
				int a;
				for (a = 1; a<m - 1 - k; a++)
				{
					if (M[(k + a)*n + j] != 0)
					{
						for (j = 0; j <n; j++)
						{
							temp = M[k*n + j];
							M[k*n + j] = M[(k + a)*n + j];
							M[(k + a)*n + j] = temp;
						}
						break;
					}
				}
				if (a == m - 1 - k)
				{
					AfxMessageBox(_T("A矩阵不可逆"));
				}
				///////////++++++++++++++
			}
		}

		for (i = k + 1; i < m; i++)
		{
			b = M[i*n + k] / M[k*n + k];
			for (j = 0; j < n; j++)
			{
				temp = M[i*n + j] - b * M[k*n + j];
				M[i*n + j] = temp;
			}
		}
	}


	for (k = m - 1; k > 0; k--)
	{
		if (M[k*n + k] == 0)   AfxMessageBox(_T("A矩阵不可逆"));
		/*{  //--
		for (i = k + 1;i < m;i++)
		{
		if (M[i*n+k] != 0)
		{
		break;
		}
		}
		if (i >= m)
		{
		AfxMessageBox("A矩阵不可逆");
		}
		else
		{
		for (j = 0;j < n;j++)
		{
		temp = M[k*n+j];
		M[k*n+j] = M[(k+1)*n+j];
		M[(k+1)*n+j] = temp;
		}
		}
		//--
		}*/
		////////////++++++++++++++++++
		/*else
		{
		for(j=k;j<n;j++)
		{
		M[j*n+j]/=M[k*n+k];  //归一
		}
		}*/
		////////////++++++++++++++++++
		for (i = k - 1; i >= 0; i--)
		{
			b = M[i*n + k] / M[k*n + k];
			for (j = 0; j < n; j++)
			{
				temp = M[i*n + j] - b * M[k*n + j];
				M[i*n + j] = temp;
			}
		}
	}
	for (i = 0; i < m; i++)
	{
		if (M[i*n + i] != 1)
		{
			b = 1 / M[i*n + i];
			for (j = 0; j < n; j++)
			{
				temp = M[i*n + j] * b;
				M[i*n + j] = temp;
			}
		}
	}
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < m; j++)
		{
			B[i*m + j] = M[i*n + (j + m)];
		}
	}
	delete[]M;
}

inline void CMyTDMap::Calculate3DtoImg(CTDMap mTDMap, const float realCoord[3], int imgPos[2])
{
	float P[3];
	 P[0] = realCoord[0] / mTDMap.m_xScale;
	 P[1] = realCoord[1] / mTDMap.m_yScale;
	 P[2] = realCoord[2] / mTDMap.m_zScale;
	float Q[4 * 1] = { P[0], P[1], P[2], 1 };//P[3]的齐次坐标
	float sq[3 * 1] = { 0 };//相机方程左边值

	multiply(mTDMap.m_Pro_Matrix, Q, sq, 3, 4, 1);//用相机方程右边矩阵乘法求左边
	mTDMap.m_scale = mTDMap.m_Pro_Matrix[8] * P[0] + mTDMap.m_Pro_Matrix[9] * P[1] + mTDMap.m_Pro_Matrix[10] * P[2] + mTDMap.m_Pro_Matrix[11];
	imgPos[0] = int(sq[0] / mTDMap.m_scale);
	imgPos[1] = int(sq[1] / mTDMap.m_scale);
}
inline void CMyTDMap::Calculate3DtoImg(CTDMap mTDMap, const float realCoord[3], float imgPos[2])
{
	float P[3];
	P[0] = realCoord[0] / mTDMap.m_xScale;
	P[1] = realCoord[1] / mTDMap.m_yScale;
	P[2] = realCoord[2] / mTDMap.m_zScale;
	float Q[4 * 1] = { P[0], P[1], P[2], 1 };//P[3]的齐次坐标
	float sq[3 * 1] = { 0 };//相机方程左边值

	multiply(mTDMap.m_Pro_Matrix, Q, sq, 3, 4, 1);//用相机方程右边矩阵乘法求左边
	mTDMap.m_scale = mTDMap.m_Pro_Matrix[8] * P[0] + mTDMap.m_Pro_Matrix[9] * P[1] + mTDMap.m_Pro_Matrix[10] * P[2] + mTDMap.m_Pro_Matrix[11];
	imgPos[0] = (sq[0] / mTDMap.m_scale);
	imgPos[1] = (sq[1] / mTDMap.m_scale);
}
inline void CMyTDMap::CalculateImgto3D(CTDMap mTDMap, const int imgPos[2], const float& Z, float realCoord[3])
{
	float p[2];
	p[0] = imgPos[0];
	p[1] = imgPos[1];
	float P[3];
	int i = 0;
	float u = p[0], v = p[1];
	float K[3 * 3] = { mTDMap.m_Pro_Matrix[2 * 4 + 0] * u - mTDMap.m_Pro_Matrix[0 * 4 + 0], mTDMap.m_Pro_Matrix[2 * 4 + 1] * u - mTDMap.m_Pro_Matrix[0 * 4 + 1], 0,
		mTDMap.m_Pro_Matrix[2 * 4 + 0] * v - mTDMap.m_Pro_Matrix[1 * 4 + 0], mTDMap.m_Pro_Matrix[2 * 4 + 1] * v - mTDMap.m_Pro_Matrix[1 * 4 + 1], 0,
		0, 0, 1 };
	float tz[3 * 1] = { mTDMap.m_Pro_Matrix[0 * 4 + 3] - mTDMap.m_Pro_Matrix[2 * 4 + 3] * u + Z*(mTDMap.m_Pro_Matrix[0 * 4 + 2] - mTDMap.m_Pro_Matrix[2 * 4 + 2] * u),
		mTDMap.m_Pro_Matrix[1 * 4 + 3] - mTDMap.m_Pro_Matrix[2 * 4 + 3] * v + Z*(mTDMap.m_Pro_Matrix[1 * 4 + 2] - mTDMap.m_Pro_Matrix[2 * 4 + 2] * v),
		Z };
	float KV[3 * 3] = { 0 };

	inverse(K, KV, 3);
	multiply(KV, tz, P, 3, 3, 1);
	realCoord[0] = P[0] * mTDMap.m_xScale;
	realCoord[1] = P[1] * mTDMap.m_yScale;
	realCoord[2] = P[2] * mTDMap.m_zScale;
}
inline void CMyTDMap::CalculateImgto3D(CTDMap mTDMap, const float imgPos[2], const float& Z, float realCoord[3])
{
	float p[2];
	p[0] = imgPos[0];
	p[1] = imgPos[1];
	float P[3];
	int i = 0;
	float u = p[0], v = p[1];
	float K[3 * 3] = { mTDMap.m_Pro_Matrix[2 * 4 + 0] * u - mTDMap.m_Pro_Matrix[0 * 4 + 0], mTDMap.m_Pro_Matrix[2 * 4 + 1] * u - mTDMap.m_Pro_Matrix[0 * 4 + 1], 0,
		mTDMap.m_Pro_Matrix[2 * 4 + 0] * v - mTDMap.m_Pro_Matrix[1 * 4 + 0], mTDMap.m_Pro_Matrix[2 * 4 + 1] * v - mTDMap.m_Pro_Matrix[1 * 4 + 1], 0,
		0, 0, 1 };
	float tz[3 * 1] = { mTDMap.m_Pro_Matrix[0 * 4 + 3] - mTDMap.m_Pro_Matrix[2 * 4 + 3] * u + Z*(mTDMap.m_Pro_Matrix[0 * 4 + 2] - mTDMap.m_Pro_Matrix[2 * 4 + 2] * u),
		mTDMap.m_Pro_Matrix[1 * 4 + 3] - mTDMap.m_Pro_Matrix[2 * 4 + 3] * v + Z*(mTDMap.m_Pro_Matrix[1 * 4 + 2] - mTDMap.m_Pro_Matrix[2 * 4 + 2] * v),
		Z };
	float KV[3 * 3] = { 0 };

	inverse(K, KV, 3);
	multiply(KV, tz, P, 3, 3, 1);
	realCoord[0] = P[0] * mTDMap.m_xScale;
	realCoord[1] = P[1] * mTDMap.m_yScale;
	realCoord[2] = P[2] * mTDMap.m_zScale;
}
inline bool CMyTDMap::ReadTDMapMatrix(const char* path, CTDMap *mTDMap)
{
	FILE *stream;
	if ((stream = fopen(path, "r+b")) != NULL)
	{
		fread(mTDMap, sizeof(CTDMap), 1, stream);
	}
	else
	{
		return false;
	}
	fclose(stream);
	return true;
}
inline bool CMyTDMap::SaveTDMapMatrix(const char* path, CTDMap mTDMap)
{
	FILE *stream;
	if ((stream = fopen(path, "w+b")) != NULL)
	{
		fwrite(&mTDMap, sizeof(CTDMap), 1, stream);
	}
	else
	{
		return false;
	}
	fclose(stream);
	return true;
}