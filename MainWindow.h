#pragma once
#include <windows.h>
#include <vector>
#include <algorithm>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "BaseWindow.h"
#include "Player.h"

#include <dwrite.h>
#pragma comment(lib, "dwrite")

struct Star {
	double starRad;
	D2D1_COLOR_F starColor; 
	D3_vector starPos;
	double distanceToPlayer = DBL_MAX;
	bool operator<(const Star& b) {
		return (this->distanceToPlayer < b.distanceToPlayer ? true : false);
	}
	bool operator>(const Star& b) {
		return (this->distanceToPlayer > b.distanceToPlayer ? true : false);
	}
	Star(double bodyRad, D2D1_COLOR_F bodyColor, D3_vector bodyPos) : 
		starRad(bodyRad), starColor(bodyColor), starPos(bodyPos) {}
};

class MainWindow : public BaseWindow<MainWindow> {
private:
	ID2D1Factory* pFactory = 0;
	ID2D1HwndRenderTarget* pRenderTarget = 0;
	ID2D1SolidColorBrush* pBrush = 0;
	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pTextFormat;
	double fov = 1.5;
	Player player;
	std::vector<Star> stars;
	bool shiftHeldDown = false;
	bool ctrlHeldDown = false;
	double thrusters = 1000;

public:
	D2D1_COLOR_F RandomStarColor();
	PCWSTR ClassName() const { return L"Basic Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};