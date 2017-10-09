#include "stdafx.h"
#include "C2DCamera.h"
#include "math.h"


C2DCamera::C2DCamera()
{
	m_vPosW.Set(0.0f, 0.0f);
	m_vUpW.Set(0.0f, 1.0f);
	m_vRightW.Set(1.0f, 0.0f);
}


C2DCamera::~C2DCamera()
{
}

void C2DCamera::SetPosition(C2DVector vPos)
{
	m_vPosW.Set(vPos);
}

C2DVector C2DCamera::GetPosition(void)
{
	return m_vPosW;
}


void C2DCamera::GenerateView(C2DVector vEye, C2DVector vUp)
{
	C2DVector vAxisY;
	C2DVector vAxisX;

	m_vPosW = vEye;

	vAxisY = vUp;
	CVectorOperation::C2DNormalize(&m_vUpW, &vAxisY);
	vAxisX.Set(vAxisY.GetY(), -vAxisY.GetX());
	CVectorOperation::C2DNormalize(&m_vRightW, &vAxisX);

	m_mtxView.m_f11 = vAxisX.GetX();	m_mtxView.m_f12 = vAxisY.GetX();	m_mtxView.m_f13 = 0.0f;
	m_mtxView.m_f21 = vAxisX.GetY();	m_mtxView.m_f22 = vAxisY.GetY();	m_mtxView.m_f23 = 0.0f;
	m_mtxView.m_f31 = -CVectorOperation::C2DDotProduct(&vAxisX, &vEye);
	m_mtxView.m_f32 = -CVectorOperation::C2DDotProduct(&vAxisY, &vEye);
	m_mtxView.m_f33 = 1.0f;
}

void C2DCamera::GenerateProj(float fWidth, float fHeight)
{
	m_mtxProj.m_f11 = 2.0f / fWidth;	m_mtxProj.m_f12 = 0.0f;	m_mtxProj.m_f13 = 0.0f;
	m_mtxProj.m_f21 = 0.0f;	m_mtxProj.m_f22 = 2.0f / fHeight;	m_mtxProj.m_f23 = 0.0f;
	m_mtxProj.m_f31 = 0.0f;	m_mtxProj.m_f32 = 0.0f;	m_mtxProj.m_f33 = 1.0f;
}
void C2DCamera::GenerateScreen(float xStart, float yStart, float fWidth, float fHeight)
{
	m_mtxScreen.m_f11 = fWidth / 2.0f;	m_mtxScreen.m_f12 = 0.0f;	m_mtxScreen.m_f13 = 0.0f;
	m_mtxScreen.m_f21 = 0.0f;	m_mtxScreen.m_f22 = -fHeight / 2.0f;	m_mtxScreen.m_f23 = 0.0f;
	m_mtxScreen.m_f31 = xStart + fWidth / 2.0f;	m_mtxScreen.m_f32 = yStart + fHeight / 2.0f;	m_mtxScreen.m_f33 = 1.0f;

	m_mtxRatio.m_f11 = fWidth / 2.0f;	m_mtxRatio.m_f12 = 0.0f;	m_mtxRatio.m_f13 = 0.0f;
	m_mtxRatio.m_f21 = 0.0f;	m_mtxRatio.m_f22 = fHeight / 2.0f;	m_mtxRatio.m_f23 = 0.0f;
	m_mtxRatio.m_f31 = 0.0f;	m_mtxRatio.m_f32 = 0.0f;	m_mtxRatio.m_f33 = 1.0f;
}

void C2DCamera::TransformView(C2DMatrix *outM, C2DMatrix *inM)
{
	CVectorOperation::C2DTransform(outM,inM, &m_mtxView);
}

void C2DCamera::TransformView(C2DVector *outV, C2DVector *inV)
{
	CVectorOperation::C2DTransform(outV, inV, &m_mtxView);
}

void C2DCamera::TransformScale(C2DMatrix *outM, C2DMatrix *inM)
{
	CVectorOperation::C2DTransform(outM, inM, &m_mtxScale);
}

void C2DCamera::TransformScale(C2DVector *outV, C2DVector *inV)
{
	CVectorOperation::C2DTransform(outV, inV, &m_mtxScale);
}

void C2DCamera::TransformProj(C2DMatrix *outM, C2DMatrix *inM)
{
	CVectorOperation::C2DTransform(outM, inM, &m_mtxProj);
}

void C2DCamera::TransformProj(C2DVector *outV, C2DVector *inV)
{
	CVectorOperation::C2DTransform(outV, inV, &m_mtxProj);
}

void C2DCamera::TransformScreen(C2DMatrix *outM, C2DMatrix *inM)
{
	CVectorOperation::C2DTransform(outM, inM, &m_mtxScreen);
}

void C2DCamera::TransformScreen(C2DVector *outV, C2DVector *inV)
{
	CVectorOperation::C2DTransform(outV, inV, &m_mtxScreen);
}

void C2DCamera::TransformRatio(C2DVector *outV, C2DVector *inV)
{
	CVectorOperation::C2DTransform(outV, inV, &m_mtxRatio);
}

void C2DCamera::RotateZ(float fRadian)
{
	GenerateView(m_vPosW, 
		C2DVector(m_vUpW.GetX() * cosf(fRadian) + m_vUpW.GetY() * -sinf(fRadian) 
			,m_vUpW.GetX() * sinf(fRadian) + m_vUpW.GetY() * cosf(fRadian)));
}

void C2DCamera::Move(C2DVector& vector)
{
	GenerateView(m_vPosW + vector, m_vUpW);
}

void C2DCamera::ZoomInOrOut(float fMultiple)
{
	CVectorOperation::C2DScalarProduct(&m_mtxScale, fMultiple, &m_mtxScale);
	m_mtxScale.m_f33 = 1.0f;
}

void C2DCamera::ZoomInit(void)
{
	m_mtxScale.Identify();
}