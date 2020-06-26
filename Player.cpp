#include "Player.h"

D3_vector Player::getPlayerRollVector() {
	return playerBasis[0];
}

D3_vector Player::getPlayerPitchVector() {
	return playerBasis[1];
}

D3_vector Player::getPlayerYawVector() {
	return playerBasis[2];
}

D3_vector Player::incrementPosition(double step) {
	if ((playerVel < 63241 || playerAcc < 0) && (playerVel > -63241 || playerAcc > 0))
		playerVel += (playerAcc * step);

	// Move player forward in the direction they're facing
	playerPos.x += playerBasis[0].x * playerVel * step;
	playerPos.y += playerBasis[0].y * playerVel * step;
	playerPos.z += playerBasis[0].z * playerVel * step;
	return playerPos;
}

void Player::setAcceleration(double acc) {
	playerAcc = acc;
}

void Player::changeOrientation(double yaw, double pitch, double roll) {
	rotateAroundAxis(getPlayerYawVector(), -yaw);
	rotateAroundAxis(getPlayerPitchVector(), -pitch);
	rotateAroundAxis(getPlayerRollVector(), roll);
}

void Player::rotateAroundAxis(D3_vector axis, double o) {
	//Multiply playerBasis by a rotation matrix
	D3_vector tempPlayerBasis[3] = { playerBasis[0], playerBasis[1], playerBasis[2] };
	playerBasis[0].x =
		(cos(o) + pow(axis.x, 2) * (1 - cos(o))) * tempPlayerBasis[0].x +
		(axis.x * axis.y * (1 - cos(o)) - axis.z * sin(o)) * tempPlayerBasis[0].y +
		(axis.x * axis.z * (1 - cos(o)) + axis.y * sin(o)) * tempPlayerBasis[0].z;
	playerBasis[1].x = 
		(cos(o) + pow(axis.x, 2) * (1 - cos(o))) * tempPlayerBasis[1].x +
		(axis.x * axis.y * (1 - cos(o)) - axis.z * sin(o)) * tempPlayerBasis[1].y +
		(axis.x * axis.z * (1 - cos(o)) + axis.y * sin(o)) * tempPlayerBasis[1].z;
	playerBasis[2].x = 
		(cos(o) + pow(axis.x, 2) * (1 - cos(o))) * tempPlayerBasis[2].x +
		(axis.x * axis.y * (1 - cos(o)) - axis.z * sin(o)) * tempPlayerBasis[2].y +
		(axis.x * axis.z * (1 - cos(o)) + axis.y * sin(o)) * tempPlayerBasis[2].z;
	playerBasis[0].y =										 
		(axis.y * axis.x * (1 - cos(o)) + axis.z * sin(o)) * tempPlayerBasis[0].x + 
		(cos(o) + pow(axis.y, 2) * (1 - cos(o))) * tempPlayerBasis[0].y +
		(axis.y * axis.z * (1 - cos(o)) - axis.x * sin(o)) * tempPlayerBasis[0].z;
	playerBasis[1].y =										 
		(axis.y * axis.x * (1 - cos(o)) + axis.z * sin(o)) * tempPlayerBasis[1].x +
		(cos(o) + pow(axis.y, 2) * (1 - cos(o))) * tempPlayerBasis[1].y +
		(axis.y * axis.z * (1 - cos(o)) - axis.x * sin(o)) * tempPlayerBasis[1].z;
	playerBasis[2].y =										 
		(axis.y * axis.x * (1 - cos(o)) + axis.z * sin(o)) * tempPlayerBasis[2].x +
		(cos(o) + pow(axis.y, 2) * (1 - cos(o))) * tempPlayerBasis[2].y +
		(axis.y * axis.z * (1 - cos(o)) - axis.x * sin(o)) * tempPlayerBasis[2].z;
	playerBasis[0].z =										 
		(axis.z * axis.x * (1 - cos(o)) - axis.y * sin(o)) * tempPlayerBasis[0].x +
		(axis.z * axis.y * (1 - cos(o)) + axis.x * sin(o)) * tempPlayerBasis[0].y +
		(cos(o) + pow(axis.z, 2) * (1 - cos(o))) * tempPlayerBasis[0].z;
	playerBasis[1].z =										 
		(axis.z * axis.x * (1 - cos(o)) - axis.y * sin(o)) * tempPlayerBasis[1].x +
		(axis.z * axis.y * (1 - cos(o)) + axis.x * sin(o)) * tempPlayerBasis[1].y +
		(cos(o) + pow(axis.z, 2) * (1 - cos(o))) * tempPlayerBasis[1].z;
	playerBasis[2].z =										 
		(axis.z * axis.x * (1 - cos(o)) - axis.y * sin(o)) * tempPlayerBasis[2].x +
		(axis.z * axis.y * (1 - cos(o)) + axis.x * sin(o)) * tempPlayerBasis[2].y +
		(cos(o) + pow(axis.z, 2) * (1 - cos(o))) * tempPlayerBasis[2].z;
}