#pragma once
class C2DCamera
{
public:
	C2DCamera();
	virtual ~C2DCamera();

protected:
	C2DMatrix m_mtxView;
	C2DMatrix m_mtxScale;
	C2DMatrix m_mtxProj;
	C2DMatrix m_mtxScreen;
	C2DMatrix m_mtxRatio;

	C2DVector m_vPosW;
	C2DVector m_vUpW;
	C2DVector m_vRightW;

public:
	void SetPosition(C2DVector vPos);
	C2DVector GetPosition(void);

	void GenerateView(C2DVector vEye, C2DVector vUp);
	void GenerateProj(float fWidth, float fHeight);
	void GenerateScreen(float xStart, float yStart, float fWidth, float fHeight);

	void TransformView(C2DMatrix *outM, C2DMatrix *inM);
	void TransformView(C2DVector *outV, C2DVector *inV);
	void TransformScale(C2DMatrix *outM, C2DMatrix *inM);
	void TransformScale(C2DVector *outV, C2DVector *inV);
	void TransformProj(C2DMatrix *outM, C2DMatrix *inM);
	void TransformProj(C2DVector *outV, C2DVector *inV);
	void TransformScreen(C2DMatrix *outM, C2DMatrix *inM);
	void TransformScreen(C2DVector *outV, C2DVector *inV);

	void TransformRatio(C2DVector *outV, C2DVector *inV);

	void RotateZ(float fRadian);
	void Move(C2DVector& vector);

	void ZoomInOrOut(float fMultiple);
	void ZoomInit(void);
};

