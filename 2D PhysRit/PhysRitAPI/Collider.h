#pragma once
#include "VectorOperation.h"
//#include "PhysicalOperation.h"
#include "C2DCamera.h"
#include<Windows.h>

class CCollider
{
protected:
	float m_fMass;				// kg

public:
	CCollider();
	CCollider(float);
	virtual ~CCollider();

	void SetMass(float fMass);
	float GetMass(void);
	virtual void Update(float fElapsedTime) {}
	virtual void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr) {}
};

class C2DCollider : public CCollider
{
protected:					
	C2DVector m_vRelative;		// ���� �߽ɿ��� �ݶ��̴����� ���� (�ݶ��̴���) ; �������ݶ��̴��� ��� �� ��ȿ
	float m_fIzz;				// z�࿡ ���� ȸ�� ����							; ����, ��ġ �׸��� ����� ���ǵǾ�� ��
	float m_fAngularA;			// z�࿡ ���� �����ӵ� ; rad/s^2
	float m_fAngularV;			// z�࿡ ���� ���� ���ӵ� ; rad/s
	C2DVector m_vLinearA;		// xy��鿡 ���� �����ӵ�
	C2DVector m_vLinearV;		// xy��鿡 ���� ���ӵ� ; m/s
	//C2DKinematicProcessUnit m_Kinematic;

public:
	C2DMatrix m_mtxWorld;		// ���ӵ� ��ǥ�迡�� ǥ���Ǵ� ��ġ �� ȸ�� ������ ���� ���

public:
	C2DCollider();
	C2DCollider(float);
	~C2DCollider();

	void SetRelative(C2DVector& vector);
	void SetPosition(C2DVector& vector);
	void SetAngularA(float fAngularA);
	void SetAngularV(float fAngularV);
	void SetLinearA(C2DVector& vLinearA);
	void SetLinearV(C2DVector& vLinearV);
	
	float GetIzz(void);
	C2DVector GetRelative(void);
	C2DVector GetPosition(void);
	float GetAngularA(void);
	float GetAngularV(void);
	C2DVector GetLinearA(void);
	C2DVector GetLinearV(void);
	C2DVector GetWorldUpV(void);
	C2DVector GetWorldRightV(void);

	void Move(C2DVector& vector);
	void RotateZ(float fRadian);

	void Update(float fElapsedTime);		// �־��� ���ӵ�, ���ӵ��� �ٰ��Ͽ� ȸ����� ����� ����.
};

class C3DCollider : public CCollider
{
public:
	C3DCollider();
	~C3DCollider();
};
/****************************************************************************************/
/*                                                                                      */		
/*                                 2D �ݶ��̴�                                          */
/*                                                                                      */
/****************************************************************************************/
class C2DColliderCircle : public C2DCollider
{
private:
	float m_fRadius;

private:
	void CalculateInertiaTensor(void);
	
public:
	C2DColliderCircle();
	C2DColliderCircle(float, float);
	~C2DColliderCircle();

	void DefineModelInLocal(float fRadius);
	float GetRadius(void);

	void RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera);		// ���� ���� �Լ��� ��� �۵��ұ�?
};

class C2DColliderRect : public C2DCollider
{
private:
	float m_fWidth;
	float m_fHeight;

private:
	void CalculateInertiaTensor(void);

public:
	C2DColliderRect();
	C2DColliderRect(float, float, float);
	~C2DColliderRect();

	void DefineModelInLocal(float fWidth, float fHeight);
	float GetWidth(void);
	float GetHeight(void);

	void RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera);
};