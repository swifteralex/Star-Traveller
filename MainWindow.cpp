#include "MainWindow.h"

D2D1_COLOR_F MainWindow::RandomStarColor() {
	D2D1_COLOR_F c;
	int random = rand() % 7;
	switch (random) {
	case 0:
		c = D2D1::ColorF(155 / 255.0, 176 / 255.0, 255 / 255.0);
		break;
	case 1:
		c = D2D1::ColorF(170 / 255.0, 191 / 255.0, 255 / 255.0);
		break;				
	case 2:					
		c = D2D1::ColorF(202 / 255.0, 215 / 255.0, 255 / 255.0);
		break;				
	case 3:					
		c = D2D1::ColorF(248 / 255.0, 247 / 255.0, 255 / 255.0);
		break;				
	case 4:					
		c = D2D1::ColorF(255 / 255.0, 244 / 255.0, 234 / 255.0);
		break;				
	case 5:					
		c = D2D1::ColorF(255 / 255.0, 210 / 255.0, 161 / 255.0);
		break;				
	case 6:					
		c = D2D1::ColorF(255 / 255.0, 204 / 255.0, 111 / 255.0);
		break;
	}
	return c;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_TIMER: {
		pRenderTarget->BeginDraw();
		D2D1_RECT_F rectangle = D2D1::RectF(0, 0, monitorWidth, monitorHeight);
		pBrush->SetColor(D2D1::ColorF(0.0F, 0.0F, 0.0F));
		pRenderTarget->FillRectangle(&rectangle, pBrush);

		D3_vector playerPos = player.incrementPosition(1.0 / 60);
		D3_vector yaw = player.getPlayerYawVector();
		D3_vector pitch = player.getPlayerPitchVector();
		D3_vector roll = player.getPlayerRollVector() * fov;
		D3_vector n = roll;
		D3_vector p = playerPos + roll;

		// Sort bodies based on distance from player for a depth buffer
		for (unsigned i = 0; i < stars.size(); i++) {
			D3_vector a = stars.at(i).starPos;
			D3_vector v = stars.at(i).starPos - playerPos;
			stars.at(i).distanceToPlayer = 
				sqrt(pow((a - playerPos).x, 2) + pow((a - playerPos).y, 2) + pow((a - playerPos).z, 2));
			if (stars.at(i).distanceToPlayer > 126482) {
				D3_vector starToPlayerVec = stars.at(i).starPos - playerPos;
				double mSTPV = sqrt(pow(starToPlayerVec.x, 2) + pow(starToPlayerVec.y, 2) + pow(starToPlayerVec.z, 2));
				starToPlayerVec = starToPlayerVec * (-126482 / mSTPV);
				D3_vector newStarPos = starToPlayerVec;
				newStarPos = newStarPos + playerPos;
				Star newSystem(0.929473935 + (((rand() % 32767) - 16383) / 16383) * 0.5, RandomStarColor(), newStarPos);
				newSystem.distanceToPlayer =
					sqrt(pow((newStarPos - playerPos).x, 2) + pow((newStarPos - playerPos).y, 2) + pow((newStarPos - playerPos).z, 2));
				stars.erase(stars.begin() + i);
				stars.push_back(newSystem);
				
			}

		}
		std::sort(stars.begin(), stars.end());
		
		for (int i = stars.size() - 1; i >= 0; i--) {
			D3_vector a = stars.at(i).starPos;
			D3_vector v = stars.at(i).starPos - playerPos;

			// Calculate if a star is behind player
			D3_vector bodyProjToRollVec = roll * ((v.dot(roll)) / (pow(roll.x, 2) + pow(roll.y, 2) + pow(roll.z, 2)));
			if (roll.x / bodyProjToRollVec.x < 0)
				continue;

			// Calculate where on the screen the star should appear
			double t = n.dot(p - a) / n.dot(v);
			D3_vector intersection = a + (v * t);
			D3_vector intersectionScreenAtOrigin = intersection - (playerPos + roll);
			double x = (intersectionScreenAtOrigin.dot(pitch)) / (pow(pitch.x, 2) + pow(pitch.y, 2) + pow(pitch.z, 2));
			double y = (intersectionScreenAtOrigin.dot(yaw)) / (pow(yaw.x, 2) + pow(yaw.y, 2) + pow(yaw.z, 2));
			x = (1 - x) * (monitorWidth / 2);
			y = (1 - y) * (monitorHeight / 2);
			double bodyApparentRad = (stars.at(i).starRad / stars.at(i).distanceToPlayer);
			if (bodyApparentRad < 1.2) {
				pBrush->SetColor(D2D1::ColorF(
					stars.at(i).starColor.r, 
					stars.at(i).starColor.g, 
					stars.at(i).starColor.b, 
					(FLOAT)pow(-stars.at(i).distanceToPlayer / 126482 + 1, 2)
				));
				bodyApparentRad = 1.2;
			} else {
				pBrush->SetColor(stars.at(i).starColor);
			}
			pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F((FLOAT)x, (FLOAT)y), (FLOAT)bodyApparentRad, (FLOAT)bodyApparentRad), pBrush);
		}

		pBrush->SetColor(D2D1::ColorF(1.0F, 1.0F, 1.0F));
		pRenderTarget->DrawLine(
			D2D1::Point2F(monitorWidth / 2, (monitorHeight / 2) - 10), 
			D2D1::Point2F(monitorWidth / 2, (monitorHeight / 2) + 10), 
			pBrush, 2.0F, NULL
		);
		pRenderTarget->DrawLine(
			D2D1::Point2F((monitorWidth / 2) - 10, monitorHeight / 2),
			D2D1::Point2F((monitorWidth / 2) + 10, monitorHeight / 2),
			pBrush, 2.0F, NULL
		);
		UINT32 cTextLength_ = (UINT32)wcslen(L"THRUSTERS");
		pRenderTarget->DrawText(L"THRUSTERS", cTextLength_, pTextFormat, D2D1::RectF(0, monitorHeight - 195, 250, monitorHeight), pBrush);
		for (int i = 0; i < log10(thrusters) + 4; i++) {
			D2D1_RECT_F rectangle = D2D1::RectF(50, monitorHeight - 130 - i * 30, 200, monitorHeight - 150 - i * 30);
			pBrush->SetColor(D2D1::ColorF((i * 35) / 255.0, 1.0 - (i * 35) / 255.0, 0.0));
			pRenderTarget->FillRectangle(&rectangle, pBrush);
		}

		pRenderTarget->EndDraw();

		return 0;
	}

	case WM_SETCURSOR: {
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(NULL);
			return TRUE;
		}
		return 0;
	}

	case WM_KEYDOWN: {
		if (wParam == VK_SHIFT) {
			if (thrusters < 5000)
				thrusters *= 10;
		} else if (wParam == VK_CONTROL) {
			if (thrusters > 0.005)
				thrusters /= 10;
		}
		return 0;
	}

	case WM_MOUSEMOVE: {
		float x = LOWORD(lParam);
		float y = HIWORD(lParam);
		POINT clientToScreen;
		clientToScreen.x = monitorWidth / 2;
		clientToScreen.y = monitorHeight / 2;
		ClientToScreen(m_hwnd, &clientToScreen);
		player.changeOrientation((x - monitorWidth / 2)/1000, (monitorHeight / 2 - y)/1000, 0);
		SetCursorPos(clientToScreen.x, clientToScreen.y);
		return 0;
	}

	case WM_LBUTTONDOWN: {
		player.setAcceleration(thrusters);
		return 0;
	}

	case WM_LBUTTONUP: {
		player.setAcceleration(0);
		return 0;
	}

	case WM_RBUTTONDOWN: {
		player.setAcceleration(-thrusters);
		return 0;
	}

	case WM_RBUTTONUP: {
		player.setAcceleration(0);
		return 0;
	}

	case WM_CREATE: {
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
			return -1;
		}

		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));
		pWriteFactory->CreateTextFormat(L"MS Gothic", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.0f, L"en-us", &pTextFormat);
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget);
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0F, 0.0F, 0.0F), &pBrush);
		SetTimer(m_hwnd, 101, 15, NULL);

		for (int i = 0; i < 5000; i++) {
			double x = (rand() % 32767) - 16383;
			double y = (rand() % 32767) - 16383;
			double z = (rand() % 32767) - 16383;
			x *= 15.4401684622;
			y *= 15.4401684622;
			z *= 15.4401684622;
			double m = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
			if (m > 126482) {
				i--;
				continue;
			}
			D3_vector newStarPos(x, y, z);
			Star newSystem(0.929473935 + (((rand() % 32767) - 16383) / 16383) * 0.5, RandomStarColor(), newStarPos);
			stars.push_back(newSystem);
		}

		return 0;
	}

	case WM_GETMINMAXINFO: {
		MINMAXINFO* info = (MINMAXINFO*)lParam;
		info->ptMaxTrackSize.x = monitorWidth;
		info->ptMaxTrackSize.y = monitorHeight;
		info->ptMinTrackSize.x = monitorWidth;
		info->ptMinTrackSize.y = monitorHeight;
		return 0;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}

	default: {
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}

	}
	return TRUE;
}