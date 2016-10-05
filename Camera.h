#ifndef CAMERA_INC
#define CAMERA_INC

class Camera : MemManaged{
private:
	XMVECTOR DefaultForward;
	XMVECTOR DefaultRight;
	XMVECTOR Forward;
	XMVECTOR Right;
	XMMATRIX RotationMatrix;
	XMMATRIX GroundWorld;
	float moveLeftRight;
	float moveBackForward;
	float yaw;
	float pitch;

	XMVECTOR Target;
	XMVECTOR Position;
	XMMATRIX View;
	XMMATRIX Projection;
	XMMATRIX ViewTimesProjection;
	XMVECTOR Up;

	bool mr,ml,mf,mb;
	POINT CursorLastPos;
public:

}; 
typedef struct Camera Camera;

HRESULT Camera_New(Camera** ppCamera);
HRESULT Camera_ShutdownAndDelete(void*);
HRESULT Camera_Run(TIME elapsed);
HRESULT Camera_Startup(void*);
XMMATRIX* Camera_GetViewTimesProjection(Camera* pCamera);




#endif // !CAMERA_INC

