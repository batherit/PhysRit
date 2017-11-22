#pragma once
//#include "Colliders.h"
#include "VectorOperation.h"
//#include "PhysicalOperation.h"
#include "C2DCamera.h"
#include<Windows.h>

#define EF_IMPURSE		(1<<0)
#define EF_GRAVITY		(1<<1)

#define GRAVITATIONAL_ACCELERATION_MAGNITUDE	9.8		//m/s^2

enum COLLIDER_TYPE { COLLIDER_TYPE_NONE, COLLIDER_TYPE_RECT, COLLIDER_TYPE_CIRCLE, COLLIDER_TYPE_COLLIDERS };

class C2DColliderCircle;
class C2DColliderRect;
class C2DColliders;

class CCollider
{
protected:
	UINT m_EF_Flags;
	float m_fMass;				// kg
	COLLIDER_TYPE m_eType;

public:
	CCollider();
	CCollider(float fMass, UINT EF_Flags);
	virtual ~CCollider();

	void SetMass(float fMass);
	float GetMass(void);
	UINT GetEF_Flags(void);

	COLLIDER_TYPE GetType(void);

	virtual void Update(float fElapsedTime) {}
	virtual void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr) {}
};

class C2DCollider : public CCollider
{
protected:					
	C2DCollider* m_pParent;
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
	C2DCollider(float, UINT);
	~C2DCollider();

	void SetParent(C2DCollider* pParent);
	void SetRelative(C2DVector& vector);
	void SetPosition(C2DVector& vector);
	void SetAngularA(float fAngularA);
	void SetAngularV(float fAngularV);
	void SetLinearA(C2DVector& vLinearA);
	void SetLinearV(C2DVector& vLinearV);
	
	C2DCollider* GetParent(void);
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

	virtual bool IsCollided_(C2DColliderCircle* circle, C2DVector* colPos) { return false; }
	virtual bool IsCollided_(C2DColliderRect* rect, C2DVector* colPos) { return false; }
	virtual bool IsCollided_(C2DColliders* colliders, C2DVector* colPos) { return false; }
	virtual void Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos) { colPos = nullptr; }
	virtual void Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos) { colPos = nullptr; }
	virtual void Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos) { colPos = nullptr; }
	void Update(float fElapsedTime);		// �־��� ���ӵ�, ���ӵ��� �ٰ��Ͽ� ȸ����� ����� ����.

	virtual void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr) {}
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
	C2DColliderCircle(float fMass, float fRadius, UINT EF_Flags);
	~C2DColliderCircle();

	void DefineModelInLocal(float fRadius);
	float GetRadius(void);

	bool IsCollided_(C2DColliderCircle* circle, C2DVector* colPos);
	bool IsCollided_(C2DColliderRect* rect, C2DVector* colPos);
	bool IsCollided_(C2DColliders* colliders, C2DVector* colPos);
	void Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos);
	void Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos);
	void Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos);

	void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr);		// ���� ���� �Լ��� ��� �۵��ұ�?
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
	C2DColliderRect(float, float, float, UINT);
	~C2DColliderRect();

	void DefineModelInLocal(float fWidth, float fHeight);
	float GetWidth(void);
	float GetHeight(void);

	bool IsCollided_(C2DColliderCircle* circle, C2DVector* colPos);
	bool IsCollided_(C2DColliderRect* rect, C2DVector* colPos);
	bool IsCollided_(C2DColliders* colliders, C2DVector* colPos);
	void Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos);
	void Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos);
	void Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos);

	void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr);
};