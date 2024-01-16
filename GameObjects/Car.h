#include <GL/glew.h>

#include "../geometry.h"
#include "../TimeUtil.h"
#include "GameObject3D.h"
#include "Orange.h"
#include "Table.h"
#include "Cheerios.h"
#include "../AVTmathLib.h"

#include "../Lights/SpotLight.h"
using namespace std;

#pragma once
class Car
{
public:
	float bb_identity[8][4] = { {1.0f, 0.0f, 0.0f, 1.0f}
					,{ 0.0f, 0.0f, 1.0f, 1.0f}
					,{ 1.0f, 0.0f, 1.0f, 1.0f}
					,{ 0.0f, 0.0f, 0.0f, 1.0f}
					,{ 1.0f, 1.0f, 1.0f, 1.0f}
					,{ 1.0f, 1.0f, 0.0f, 1.0f}
					,{ 0.0f, 1.0f, 1.0f, 1.0f}
					,{ 0.0f, 1.0f, 0.0f, 1.0f} };
	float bb[8][4] = { {1.0f, 0.0f, 0.0f, 1.0f}
					,{ 0.0f, 0.0f, 1.0f, 1.0f}
					,{ 1.0f, 0.0f, 1.0f, 1.0f}
					,{ 0.0f, 0.0f, 0.0f, 1.0f}
					,{ 1.0f, 1.0f, 1.0f, 1.0f}
					,{ 1.0f, 1.0f, 0.0f, 1.0f}
					,{ 0.0f, 1.0f, 1.0f, 1.0f}
					,{ 0.0f, 1.0f, 0.0f, 1.0f} };
private:
	float speed = 0.0f;
	float maxSpeed = 20.0f;
	float sl1_offset[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float sl2_offset[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	float position[3] = { 5.0f, 0.28f, 0.0f };
	float old_position[3] = { 0.0f, 0.0f, 0.0f };
	float direction[3] = { 1.0f, 0.0f, 0.0f };
	float acceleration = 10.0f;
	float rotationAngle = 0.0f;
	float wheel_rot_angle = 0.0f;
	float wheel_rot_speed = 0.0f;
	float up[3] = { 0.0f, 1.0f, 0.0f };
	float wheel_radius = 0.0f;
	float rot_dir[3] = { 0.0f, 0.0f, 0.0f };
	bool isCollisionEnabled;
	float rotationSpeed = 130.0f; // FIXME ROTATION SPEED
	//min_x, max_x, min_y, max_y, min_z, max_z
	
	float bb_bounds[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	bool accelerating = false;

	MyMesh* bodyMesh;
	Material bodyMaterial;
	Material topMaterial;
	MyMesh* wheelMesh;
	Material wheelMaterial;

	SpotLight sl1;
	SpotLight sl2;

public:
	Car(MyMesh* cubeMesh, MyMesh* torusMesh, bool isCollisionEnabled, float radius);
	Car() = default;

	void UpdateSpotLights();
	LightProperties* GetSpotLight1();
	LightProperties* GetSpotLight2();

	bool IsStopping = false;
	void MoveForwards();
	void MoveBackwards();
	void StopMovement();
	void RotateRight();
	void RotateLeft();
	void MoveCar();

	float* GetPosition();

	void SetMeshMaterial(MyMesh* mesh, Material mat);
	Material GetBodyMaterial();
	Material GetTopMaterial();
	Material GetWheelMaterial();
	MyMesh* GetBodyMesh();
	MyMesh* GetWheelMesh();

	void UpdateBody();
	void UpdateTopBody();
	void UpdateWheelTopLeft();
	void UpdateWheelTopRight();
	void UpdateWheelBotLeft();
	void UpdateWheelBotRight();

	float* GetBBBounds() { return bb_bounds; }
	bool GetIsCollisionEnabled() { return isCollisionEnabled; }
	void PrintBB();
	float* GetDirection() { return direction; }

	bool DetectCollisionWithObject(GameObject3D* other);
	void ResolveCollisionWithObject(GameObject3D* other);

	void CalculateBoundingBox();
	void translate_bb(float offset[3]);
	void scale_bb(float values[3]);
	float* Car::rotateBB(float* v, float* k, double theta);
	void scale_bb_identity(float values[3]);
};
