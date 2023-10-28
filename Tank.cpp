#pragma once
#include <stdio.h>
#include <string>
#include <vector>

class Tank {
	struct {
		float x = 0;
		float y = 0;
	} location;
	struct {
		int index = 0;
		std::string playerName = "A";
		int health = 100;
		int ammo = 100;
		std::string weapon = "Cannon";
		float startPower = 50.0f;
		float maxPower = 100.0f;
		float minPower = 1.0f;
	} playerInfo;
	struct {
		int bodyX	= 64;
		int bodyY	= 64;
	} tankSize;
	struct {
		int angle = 270;
		int width = 10;
		int height = 60;
	} turret;

	public:
		Tank() {
			// Constructor logic, if any
		}

		Tank(float x, float y, int index, std::string playerName) {
			location.x = x;
			location.y = y;
			playerInfo.playerName = playerName;
			playerInfo.index = index;
		}
		void setAngle(float angle) {
			printf("Tank.cpp - Player: %s setAngle: %d\n", getPlayerName().c_str(), angle);
			turret.angle = angle;
		}

		void incAngle() {
			turret.angle++;
		}
		void decAngle() {
			turret.angle--;
		}

		void incPower() {
			playerInfo.startPower++;
		}
		void decPower() {
			playerInfo.startPower--;
		}
		float getPower() {
			if (playerInfo.startPower > playerInfo.maxPower) {
				playerInfo.startPower = playerInfo.maxPower;
			}
			else if (playerInfo.startPower < playerInfo.minPower) {
				playerInfo.startPower = playerInfo.minPower;
			}
			return playerInfo.startPower;
		}


		
		float getAngle() {
			normalizeAngle();
			return turret.angle;
		}

		void normalizeAngle() {
			if (turret.angle > 360) {
				turret.angle = 0;
			}
			if (turret.angle < 0) {
				turret.angle = 360;
			}
		}
		void setLocation(float x, float y) {
			location.x = x;
			location.y = y;
		}
		float getX() {
			return location.x;
		}
		float getY() {
			return location.y;
		}
		void Fire() {
			printf("Tank.cpp - FIRE!\n");
			// Create a new projectile and add it to the container
			

			// Now, you can tell the renderer to draw the projectiles
			//renderer.drawProjectiles(projectiles);
		}


		int getOriginX() {
			return tankSize.bodyX / 2;
		}
		int getOriginY() {
			return tankSize.bodyY / 2;
		}
		int getBodyX() {
			return tankSize.bodyX;
		}
		int getBodyY() {
			return tankSize.bodyY;
		}
		int getTurretX() {
			return tankSize.bodyX / 2;
		}
		int getTurretY() {
			return -tankSize.bodyY;
		}
		int getTurretWidth() {
			return turret.width;
		}
		int getTurretHeight() {
			return turret.height;
		}

		std::string getPlayerName() {
				return playerInfo.playerName;
		}

		int getIndex() {
			return playerInfo.index;
		}

		int getHealth() {
			return playerInfo.health;
		}
		int getAmmo() {
			return playerInfo.ammo;
		}
		std::string getWeaponName() {
			return playerInfo.weapon;
		}


};