#pragma once
#include <math.h>

struct D3_vector {
	double x;
	double y;
	double z;
	D3_vector operator+(const D3_vector& v) {
		return D3_vector(this->x + v.x, this->y + v.y, this->z + v.z);
	}
	D3_vector operator-(const D3_vector& v) {
		return D3_vector(this->x - v.x, this->y - v.y, this->z - v.z);
	}
	D3_vector operator*(double n) {
		return D3_vector(this->x * n, this->y * n, this->z * n);
	}
	double dot(const D3_vector& v) {
		return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
	}
	D3_vector cross(const D3_vector& v) {
		double i_component = (this->y * v.z) - (v.y * this->z);
		double j_component = -((this->x * v.z) - (v.x * this->z));
		double k_component = (this->x * v.y) - (v.x * this->y);
		return D3_vector(i_component, j_component, k_component);
	}
	D3_vector() : x(0), y(0), z(0) {}
	D3_vector(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Player {
private:
	/*
		playerBasis is a matrix that represents the orientation of the player.
		The first vector is the direction in which the player is looking.
		Rotations along the first vector represent rolling camera movements
		Rotations along the second vector represent pitching camera movements.
		Rotations along the third vector represent yawing camera movements.
	*/
	D3_vector playerBasis[3] = { D3_vector(1, 0, 0), D3_vector(0, 1, 0), D3_vector(0, 0, 1) };
	D3_vector playerPos;
	double playerVel = 0;
	double playerAcc = 0;

public:
	D3_vector getPlayerRollVector();
	D3_vector getPlayerPitchVector();
	D3_vector getPlayerYawVector();
	D3_vector incrementPosition(double);
	void setAcceleration(double);
	void changeOrientation(double, double, double);

private:
	void rotateAroundAxis(D3_vector, double);
};