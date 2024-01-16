#include "Car.h"
#define FLOAT_MAX_VALUE 99999999999999999999999999999999999.99
#define FLOAT_MIN_VALUE -99999999999999999999999999999999999.99

Car::Car(MyMesh* cubeMesh, MyMesh* torusMesh, bool isCollisionEnabled_in, float radius) {
	bodyMesh = cubeMesh;
	wheelMesh = torusMesh;

	// body material
	float amb[] = { 1.0f, 0.2f, 0.2f, 1.0f };
	float diff[] = { 1.0f, 0.2f, 0.2f, 1.0f };
	float spec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(bodyMaterial.ambient, amb, 4 * sizeof(float));
	memcpy(bodyMaterial.diffuse, diff, 4 * sizeof(float));
	memcpy(bodyMaterial.specular, spec, 4 * sizeof(float));
	memcpy(bodyMaterial.emissive, emissive, 4 * sizeof(float));
	bodyMaterial.shininess = 0.0f;
	bodyMaterial.texCount = 0;
	// top material
	float amb2[] = { 0.5f, 0.2f, 1.0f, 1.0f };
	float diff2[] = { 0.5f, 0.2f, 1.0f, 1.0f };
	float spec2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(topMaterial.ambient, amb2, 4 * sizeof(float));
	memcpy(topMaterial.diffuse, diff2, 4 * sizeof(float));
	memcpy(topMaterial.specular, spec2, 4 * sizeof(float));
	memcpy(topMaterial.emissive, emissive2, 4 * sizeof(float));
	topMaterial.shininess = 0.0f;
	topMaterial.texCount = 0;
	// wheel material
	float amb3[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float diff3[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float spec3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(wheelMaterial.ambient, amb3, 4 * sizeof(float));
	memcpy(wheelMaterial.diffuse, diff3, 4 * sizeof(float));
	memcpy(wheelMaterial.specular, spec3, 4 * sizeof(float));
	memcpy(wheelMaterial.emissive, emissive3, 4 * sizeof(float));
	wheelMaterial.shininess = 0.0f;
	wheelMaterial.texCount = 0;

	isCollisionEnabled = isCollisionEnabled_in;

	wheel_radius = radius;
	wheel_rot_speed = speed / wheel_radius * 40.0f;

	float slPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float slPos2[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float slDir[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	sl1 = SpotLight(slPos, slDir);
	sl2 = SpotLight(slPos2, slDir);
}

float* Car::GetPosition() { return position; }
void Car::SetMeshMaterial(MyMesh* mesh, Material mat) {	mesh->mat = mat;}
Material Car:: GetBodyMaterial() { return bodyMaterial; }
Material Car::GetTopMaterial() { return topMaterial; }
Material Car::GetWheelMaterial() { return wheelMaterial; }
MyMesh* Car::GetBodyMesh() { return bodyMesh; }
MyMesh* Car::GetWheelMesh() { return wheelMesh; }

LightProperties* Car::GetSpotLight1() {
	return sl1.GetLightPtr();
}
LightProperties* Car::GetSpotLight2() {
	return sl2.GetLightPtr();
}

void Car::UpdateSpotLights()
{
	//translate(MODEL, sl1_offset[0], sl1_offset[1], sl1_offset[2]);
	pushMatrix(MODEL);
	sl1.UpdateTransform(MODEL, sl1_offset);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	////translate(MODEL, sl2_offset[0], sl2_offset[1], sl2_offset[2]);
	sl2.UpdateTransform(MODEL, sl2_offset);
	popMatrix(MODEL);

	float dir[4];
	dir[0] = direction[0];
	dir[1] = direction[1];
	dir[2] = direction[2];
	dir[3] = 1.0f;
	sl1.SetConeDirection(dir);
	sl2.SetConeDirection(dir);

}

void Car::MoveForwards()
{
	if (speed < maxSpeed)
	{
		speed += acceleration * TimeUtil::deltaTime;
		accelerating = true;
		wheel_rot_speed = speed / wheel_radius * 40.0f;
	}
}
void Car::MoveBackwards()
{
	if (speed > -maxSpeed)
	{
		speed -= acceleration * TimeUtil::deltaTime;
		accelerating = true;
		wheel_rot_speed = speed / wheel_radius * 40.0f;
	}
}
void Car::StopMovement() 
{
	if (speed > 0.1f)
	{
		MoveBackwards();
	}
	else if (speed < -0.1f) 
	{
		MoveForwards();
	}
	else
	{
		speed = 0;
		wheel_rot_speed = speed / wheel_radius * 40.0f;
	}
}
void Car::RotateRight()
{
	float up[3] = { 0.0f, 1.0f, 0.0f };
	rotationAngle += rotationSpeed *2* TimeUtil::deltaTime;
	float rotationRad = DegToRad(rotationAngle);
	direction[0] = cos(rotationRad);
	direction[2] = sin(rotationRad);
	add(direction, direction, direction);
	normalize(direction);
}
void Car::RotateLeft()
{
	float up[3] = { 0.0f, 1.0f, 0.0f };
	rotationAngle -= rotationSpeed *2* TimeUtil::deltaTime;
	float rotationRad = DegToRad(rotationAngle);
	direction[0] = cos(rotationRad);
	direction[2] = sin(rotationRad);
	add(direction, direction, direction);
	normalize(direction);
}

void Car::MoveCar()
{
	float velocity[3];
	constProduct(TimeUtil::deltaTime * speed, direction, velocity);
	if (!length(velocity) == 0) {
		std::copy(position, position + 3, old_position);
		//printf("velocity[0] - [%f], velocity[1] - [%f], velocity[2] - [%f]\n", velocity[0], velocity[1], velocity[2]);
	}
	add(position, velocity, position);

	// rotate wheels TODO
}

void Car::translate_bb(float offset[3]) {
	
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 3; i++){
			bb[k][i] += offset[i];
		}
	}
	
}

void Car::scale_bb(float values[3])
{
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 3; i++) {
			bb[k][i] *= values[i];
		}
	}
}

void Car::scale_bb_identity(float values[3])
{
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 3; i++) {
			bb_identity[k][i] *= values[i];
		}
	}
}

void Car::UpdateBody() {
	float offset[3] = { 0.0f, 0.0f, 0.0f };
	float up[3] = { 0.0f, 1.0f, 0.0f };
	float aux[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	translate(MODEL, position[0], position[1], position[2]);

	//rotate around center of bottom axis
	translate(MODEL, 0.2f, 0.5f, 0.5f);
	rotate(MODEL, -rotationAngle, 0.0f, 1.0f, 0.0f);
	translate(MODEL, -0.2f, -0.5f, -0.5f);

	UpdateSpotLights();
	
	scale(MODEL, 2.0f, 0.5f, 1.0f);

	for (int k = 0; k < 8; k++) {
		std::copy(bb_identity[k], bb_identity[k] + 4, bb[k]);
	}

	for (int k = 0; k < 8; k++) {
		multMatrixPoint(MODEL, bb[k], aux);
		std::copy(aux, aux + 4, bb[k]);
	}

}

void Car::UpdateTopBody() {
	// transformations without popping body mesh model matrix
	translate(MODEL, 0.0f, 1.0f, 0.0f);
	scale(MODEL, 0.6f, 0.4f, 1.0f);
}

void Car::UpdateWheelTopLeft() {

	crossProduct(up, direction, rot_dir);
	//constProduct(-1.0f, rot_dir, rot_dir);

	// transformations without popping body mesh model matrix
	translate(MODEL, 0.7f, 0.1f, 0.0f);

	scale(MODEL, 0.5f, 2.0f, 1.0f); // scale back to primitive mesh size
	rotate(MODEL, -90, 1.0f, 0.0f, 0.0f);
	scale(MODEL, 0.3f, 0.3f, 0.3f);

	pushMatrix(MODEL);

	wheel_rot_angle += wheel_rot_speed * TimeUtil::deltaTime;
	rotate(MODEL, wheel_rot_angle, 0.0f, 1.0f, 0.0f);
}

void Car::UpdateWheelTopRight()
{
	// transformations without popping wheel top left mesh model matrix
	// reset previous transformation FIXME shouldn't be required


	translate(MODEL, 0.0f, -1.1f * 3, 0.0f); // y is z in world cause rotation, multiply by 3 cause scale	

}

void Car::UpdateWheelBotRight()
{
	translate(MODEL, -1.2f * 3, 0.0f, 0.0f);

	pushMatrix(MODEL);

	wheel_rot_angle += wheel_rot_speed * TimeUtil::deltaTime;
	rotate(MODEL, wheel_rot_angle, 0.0f, 1.0f, 0.0f);

}

void Car::UpdateWheelBotLeft()
{
	translate(MODEL, 0.0f, -1.1f * 3, 0.0f);
	wheel_rot_angle += wheel_rot_speed * TimeUtil::deltaTime;
	rotate(MODEL, wheel_rot_angle, 0.0f, 1.0f, 0.0f);
}

void Car::CalculateBoundingBox() {
	float min_x = FLOAT_MAX_VALUE, max_x = FLOAT_MIN_VALUE, min_y = FLOAT_MAX_VALUE, max_y = FLOAT_MIN_VALUE, min_z = FLOAT_MAX_VALUE
		, max_z = FLOAT_MIN_VALUE;

	for (int k = 0; k < 8; k++) {
		if (bb[k][0] < min_x) {
			min_x = bb[k][0];
		}

		if (bb[k][0] > max_x) {
			max_x = bb[k][0];
		}
		if (bb[k][1] < min_y) {
			min_y = bb[k][1];
		}
		if (bb[k][1] > max_y) {
			max_y = bb[k][1];
		}
		if (bb[k][2] < min_z) {
			min_z = bb[k][2];
		}
		if (bb[k][2] > max_z) {
			max_z = bb[k][2];
		}

	}

	bb_bounds[0] = min_x;
	bb_bounds[1] = max_x;
	bb_bounds[2] = min_y;
	bb_bounds[3] = max_y;
	bb_bounds[4] = min_z;
	bb_bounds[5] = max_z;
}

void Car::PrintBB()
{
	printf("{\n");
	for (int k = 0; k < 8; k++) {
		printf("{");
		for (int i = 0; i < 3; i++) {
			printf("[%f] ", bb[k][i]);
		}
		printf("}\n");
	}
	printf("}\n");
}

bool Car::DetectCollisionWithObject(GameObject3D* other)
{

	if (other->GetBBType() == GameObject3D::BB_TYPE::Spherical && other->GetType() == GameObject3D::TYPE::Orange) {
		// get box closest point to sphere center by clamping
		float x = std::max(bb_bounds[0], std::min(other->GetPosition()[0], bb_bounds[1]));
		float y = std:: max(bb_bounds[2], std::min(other->GetPosition()[1], bb_bounds[3]));
		float z = std::max(bb_bounds[4], std::min(other->GetPosition()[2], bb_bounds[5]));
		float aux[3];
		float distance[3] = { 0.0f, 0.0f, 0.0f };
		
		aux[0] = x;
		aux[1] = y;
		aux[2] = z;

		subtract(aux, other->GetPosition(), distance);

		Orange *o = dynamic_cast<Orange*>(other);
	

		return length(distance) < o->getRadius();
	}
	else if (other->GetBBType() == GameObject3D::BB_TYPE::Spherical && other->GetType() == GameObject3D::TYPE::Cheerio) {
		// get box closest point to sphere center by clamping
		float x = std::max(bb_bounds[0], std::min(other->GetPosition()[0], bb_bounds[1]));
		float y = std::max(bb_bounds[2], std::min(other->GetPosition()[1], bb_bounds[3]));
		float z = std::max(bb_bounds[4], std::min(other->GetPosition()[2], bb_bounds[5]));
		float aux[3];
		float distance[3] = { 0.0f, 0.0f, 0.0f };

		aux[0] = x;
		aux[1] = y;
		aux[2] = z;

		subtract(aux, other->GetPosition(), distance);

		Cheerios *c = dynamic_cast<Cheerios*>(other);

		return length(distance) < c->getRadius();
	}
	else{
		Table* t = dynamic_cast<Table*>(other);
		return (bb_bounds[0] <= t->GetBBBounds()[1] && bb_bounds[1] >= t->GetBBBounds()[0]) &&
			(bb_bounds[2] <= t->GetBBBounds()[3] && bb_bounds[3] >= t->GetBBBounds()[2]) &&
			(bb_bounds[4] <= t->GetBBBounds()[5] && bb_bounds[5] >= t->GetBBBounds()[4]);
	}

	return false;
}

void Car::ResolveCollisionWithObject(GameObject3D* other)
{
	if (other->GetType() == GameObject3D::TYPE::Table) {
		Table* t = dynamic_cast<Table*>(other);
		t->SetSpeed(1.5f);
		if(speed>=0)
			t->SetDirection(direction);
		else{
			float opositeDirection[3] = { 0.0f, 0.0f, 0.0f };
			constProduct(-1, direction, opositeDirection);
			t->SetDirection(opositeDirection);
		}
		speed = 0;
		wheel_rot_speed = speed / wheel_radius * 40.0f;
	}
	if (other->GetType() == GameObject3D::TYPE::Cheerio) {
		Cheerios* c = dynamic_cast<Cheerios*>(other);
		c->SetSpeed(1.5f);
		if (speed >= 0)
			c->SetDirection(direction);
		else {
			float opositeDirection[3] = { 0.0f, 0.0f, 0.0f };
			constProduct(-1, direction, opositeDirection);
			c->SetDirection(opositeDirection);
		}
		speed = 0;
		wheel_rot_speed = speed / wheel_radius * 40.0f;
	}
	if (other->GetType() == GameObject3D::TYPE::Orange) {
		float new_pos[3] = { 5.0f, 0.3f, 0.0f };///!!!!!!!!!!!!!!!!!!!!!!!
		std::copy(new_pos, new_pos + 3, position);
	}
}





// v: a vector in 3D space
// k: a unit vector describing the axis of rotation
// theta: the angle (in radians) that v rotates around k
float *Car::rotateBB(float *v, float* k, double theta)
{
	float rotated[3] = { 0.0f, 0.0f, 0.0f };
	float aux1[3] = { 0.0f, 0.0f, 0.0f };
	float aux2[3] = { 0.0f, 0.0f, 0.0f };
	float aux3[3] = { 0.0f, 0.0f, 0.0f };
	float aux4[3] = { 0.0f, 0.0f, 0.0f };

	double cos_theta = cos(theta);
	double sin_theta = sin(theta);

	//v * cos_theta
	for (int i = 0; i < 3; i++) {
		aux1[i] = v[i] * cos_theta;
	}

	//crossProduct(k, v) * sin_theta
	crossProduct(k, v, aux2);

	for (int i = 0; i < 3; i++) {
		aux3[i] = aux2[i] * sin_theta;
	}

	//k * dotProduct(k, v)
	aux4[0] = dotProduct(k, v) * k[0] * (1 - cos_theta);
	aux4[1] = dotProduct(k, v) * k[1] * (1 - cos_theta);
	aux4[2] = dotProduct(k, v) * k[2] * (1 - cos_theta);


	rotated[0] = aux1[0] + aux3[0] + aux4[0];
	rotated[1] = aux1[1] + aux3[1] + aux4[1];
	rotated[2] = aux1[2] + aux3[2] + aux4[2];


	return rotated;
}