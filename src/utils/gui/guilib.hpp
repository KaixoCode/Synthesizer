#ifndef GUILIB
#define GUILIB
#ifdef WIN32
#ifndef UNICODE
#define UNICODE
#endif 
#pragma comment(lib, "d2d1")
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <thread>
#include <vector>
#include <string>
#include <cmath>
#include <Windowsx.h>
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
#define PI 3.14159265359
template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

int main();
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (SUCCEEDED(CoInitialize(NULL)))
		main();
}

namespace Bounds {
	const bool Ellipse(int x, int y, int cx, int cy, int w, int h) 
	{
		return (std::pow(x - cx, 2) / std::pow(w, 2)) + (std::pow(y - cy, 2) / std::pow(h, 2)) <= 1;
	}

	const bool Rectangle(int x, int y, int rx, int ry, int w, int h) 
	{
		return x > rx && x < rx + w && y > ry && y < ry + h;
	}
};

typedef ID2D1HwndRenderTarget Canvas;
typedef ID2D1SolidColorBrush SolidBrush;
typedef ID2D1LinearGradientBrush GradientBrush;

struct Event 
{
	const static int NONE = 1;
	const static int KEY = 1;
	const static int MOUSE = 1;

	int eventType = NONE;
};

struct KeyEvent : Event
{
	const static int NONE = 0;
	const static int PRESSED = 1;
	const static int RELEASED = 2;

	KeyEvent(int key, int type) :
		key(key),
		type(type)
	{
		eventType = Event::KEY;
	};

	int key;
	int type;
};

struct MouseEvent : Event
{
	const static int LEFT = 1;
	const static int RIGHT = 3;
	const static int MIDDLE = 2;

	const static int NONE = 0;
	const static int PRESSED = 1;
	const static int RELEASED = 2;
	const static int MOVED = 3;
	const static int DRAGGED = 4;
	const static int ENTERED = 5;
	const static int EXITED = 6;

	MouseEvent(int button, int type, int x, int y) : 
		button(button), 
		type(type), 
		x(x), 
		y(y) 
	{ 
		eventType = Event::MOUSE; 
	};

	int button;
	int type;
	int x;
	int y;
};

class Window;
struct Drawable
{
	bool hovering = false;
	bool focussed = false;
	Window* window;
	LPCWSTR cursor = IDC_ARROW;
	virtual void Draw(Canvas&) = 0;
	virtual bool WithinBounds(int x, int y) = 0;
	virtual void Position(int x, int y) {};

	virtual void MousePressed(MouseEvent&) {};
	virtual void MouseReleased(MouseEvent&) {};
	virtual void MouseMoved(MouseEvent&) {};
	virtual void MouseDragged(MouseEvent&) {};
	virtual void MouseEntered(MouseEvent&) {};
	virtual void MouseExited(MouseEvent&) {};

	virtual void KeyPressed(KeyEvent&) {};
	virtual void KeyReleased(KeyEvent&) {};
};

class Window
{
public:
	const unsigned int FPS = 60;
	const unsigned int TPS = 60;
	const unsigned int MAX_FRAMESKIP = 1;

	bool mousePressed = false;
	int mouseButton = 0;
	int mouseX = 0;
	int mouseY = 0;
	unsigned int width = 0;
	unsigned int height = 0;

	virtual void Draw() {};
	virtual void Setup() {};
	virtual void Clean() {};

	virtual void MousePressed(MouseEvent&) {};
	virtual void MouseReleased(MouseEvent&) {};
	virtual void MouseMoved(MouseEvent&) {};
	virtual void MouseDragged(MouseEvent&) {};
	virtual void KeyPressed(KeyEvent&) {};
	virtual void KeyReleased(KeyEvent&) {};


	Window() :
		m_hwnd(NULL),
		m_pDirect2dFactory(NULL),
		canvas(NULL),
		color(NULL),
		m_pDWriteFactory(NULL)
	{
		thread = std::thread(
			[this]()
			{
				Start();
				Clean();
				exit(0);
			}
		);
	};

	~Window()
	{
		SafeRelease(&m_pDirect2dFactory);
		DiscardDeviceResources();
		CoUninitialize();
	}

	void CursorPos(int x, int y) 
	{
		POINT pt{ x, y };
		ClientToScreen(m_hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void Background(D2D1_COLOR_F c)
	{
		canvas->Clear(c);
	}

	void FillEllipse(int x, int y, int rx, int ry)
	{
		canvas->FillEllipse(D2D1::Ellipse(D2D1::Point2(x, y), rx, ry), color);
	}

	void DrawEllipse(int x, int y, int rx, int ry)
	{
		canvas->DrawEllipse(D2D1::Ellipse(D2D1::Point2(x, y), rx, ry), color, strokeWidth);
	}

	void FillRect(int x, int y, int w, int h)
	{
		canvas->FillRectangle(D2D1::Rect(x, y, x + w, y + h), color);
	}

	void DrawRect(int x, int y, int w, int h)
	{
		canvas->DrawRectangle(D2D1::Rect(x, y, x + w, y + h), color, strokeWidth);
	}

	void StrokeWidth(unsigned int s)
	{
		strokeWidth = s;
	}

	void Fill(D2D1_COLOR_F c)
	{
		canvas->CreateSolidColorBrush(c, &color);
	}

	void DrawString(const WCHAR* str, const size_t len, int x, int y)
	{
		canvas->DrawTextW(str, len, m_pTextFormat, D2D1::RectF(x, y, width, height), color);
	}

	void Draw(Drawable& t)
	{
		t.window = this;
		drawables.push_back(t);
	}

	void Draw(Drawable& t, float x, float y)
	{
		t.window = this;
		t.Position(x, y);
		drawables.push_back(t);
	}

private:
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	Canvas* canvas;
	SolidBrush* color;
	unsigned int strokeWidth = 1;
	std::thread thread;
	std::vector<std::reference_wrapper<Drawable>> drawables;
	std::vector<MouseEvent> mouseEvents;
	std::vector<KeyEvent> keyEvents;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	void Start()
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

		if (SUCCEEDED(hr))
		{
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(m_pDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
			);
		}

		static const WCHAR msc_fontName[] = L"Verdana";
		static const FLOAT msc_fontSize = 50;
		if (SUCCEEDED(hr))
		{
			// Create a DirectWrite text format object.
			hr = m_pDWriteFactory->CreateTextFormat(
				msc_fontName,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				msc_fontSize,
				L"", //locale
				&m_pTextFormat
			);
		}

		if (SUCCEEDED(hr))
		{
			WNDCLASSEX wcex =
			{
				sizeof(WNDCLASSEX)
			};

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = Window::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(LONG_PTR);
			wcex.hInstance = HINST_THISCOMPONENT;
			wcex.hbrBackground = NULL;
			wcex.lpszMenuName = NULL;
			wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
			wcex.lpszClassName = L"Window";

			RegisterClassEx(&wcex);

			m_hwnd = CreateWindow
			(
				L"Window",
				L"Window",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				NULL,
				HINST_THISCOMPONENT,
				this
			);

			hr = m_hwnd ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				Setup();
				UpdateWindow(m_hwnd);
			}
		}

		if (SUCCEEDED(hr))
			RunMessageLoop();
	}

	void RunMessageLoop()
	{
		MSG msg = { 0 };

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Render();
			}
		}
	}

	void Render()
	{
		HRESULT hr = CreateDeviceResources();

		if (SUCCEEDED(hr))
		{
			canvas->BeginDraw();
			Draw();
			EventHandler();
			Redraw();
			hr = canvas->EndDraw();
		}

		if (hr == D2DERR_RECREATE_TARGET)
			DiscardDeviceResources();
	}

	void Redraw()
	{
		for (Drawable& d : drawables)
			d.Draw(*canvas);

		drawables.clear();
	}

	Drawable* hovering = 0;
	Drawable* focussed = 0;
	void EventHandler()
	{
		for (MouseEvent& e : mouseEvents) {
			if (e.type == MouseEvent::MOVED || e.type == MouseEvent::PRESSED)
			{
				Drawable* newhov = 0;
				for (Drawable& d : drawables)
				{
					if (d.WithinBounds(e.x, e.y))
						newhov = &d;
				}

				if (newhov != hovering)
				{
					MouseEvent en{ MouseEvent::NONE, MouseEvent::EXITED, e.x, e.y };
					if (hovering) hovering->MouseExited(en);
					MouseEvent ex{ MouseEvent::NONE, MouseEvent::ENTERED, e.x, e.y };
					if (newhov != nullptr) newhov->MouseEntered(ex);
				}

				hovering = newhov;
			}

			if (MouseEvent::PRESSED)
				focussed = hovering;

			switch (e.type)
			{
			case MouseEvent::MOVED:
				if (hovering) hovering->MouseMoved(e);
				MouseMoved(e);
				break;
			case MouseEvent::PRESSED:
				if (hovering) hovering->MousePressed(e);
				MousePressed(e);
				break;
			case MouseEvent::RELEASED:
				if (hovering) hovering->MouseReleased(e);
				MouseReleased(e);
				break;
			case MouseEvent::DRAGGED:
				if (hovering) hovering->MouseDragged(e);
				MouseDragged(e);
				break;
			}
		}

		if (hovering)
		{
			if (hovering->cursor == 0) ShowCursor(false);
			else ShowCursor(true);
			SetCursor(LoadCursor(NULL, hovering->cursor));
		}
		else
		{
			ShowCursor(true);
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}

		for (KeyEvent& e : keyEvents) {
			switch (e.type)
			{
			case KeyEvent::PRESSED:
				if (focussed) focussed->KeyPressed(e);
				KeyPressed(e);
				break;
			case KeyEvent::RELEASED:
				if (focussed) focussed->KeyReleased(e);
				KeyPressed(e);
				break;

			}
		}

		mouseEvents.clear();
	}

	void OnResize(UINT width, UINT height)
	{
		this->width = width;
		this->height = height;
		if (canvas) canvas->Resize(D2D1::SizeU(width, height));
	}

	HRESULT CreateDeviceResources()
	{
		if (!canvas)
		{
			RECT rc;
			GetClientRect(m_hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

			return m_pDirect2dFactory->CreateHwndRenderTarget
			(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hwnd, size),
				&canvas
			);
		}

		return S_OK;
	}

	void DiscardDeviceResources()
	{
		SafeRelease(&canvas);
		SafeRelease(&color);
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_CREATE) {
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>((Window*)((LPCREATESTRUCT)lParam)->lpCreateParams));
			return 1;
		}
		else
		{
			Window* app = reinterpret_cast<Window*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
			if (app) switch (message)
			{
			case WM_KEYDOWN:
				
				return 0;

			case WM_LBUTTONDOWN:
				app->mousePressed = true;
				app->mouseButton = MouseEvent::LEFT;
				app->mouseEvents.push_back({ MouseEvent::LEFT, MouseEvent::PRESSED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_RBUTTONDOWN:
				app->mousePressed = true;
				app->mouseButton = MouseEvent::RIGHT;
				app->mouseEvents.push_back({ MouseEvent::RIGHT, MouseEvent::PRESSED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_MBUTTONDOWN:
				app->mousePressed = true;
				app->mouseButton = MouseEvent::MIDDLE;
				app->mouseEvents.push_back({ MouseEvent::MIDDLE, MouseEvent::PRESSED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_LBUTTONUP:
				app->mousePressed = false;
				app->mouseEvents.push_back({ MouseEvent::LEFT, MouseEvent::RELEASED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_RBUTTONUP:
				app->mousePressed = false;
				app->mouseEvents.push_back({ MouseEvent::RIGHT, MouseEvent::RELEASED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_MBUTTONUP:
				app->mousePressed = false;
				app->mouseEvents.push_back({ MouseEvent::MIDDLE, MouseEvent::RELEASED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_MOUSEMOVE:
				app->mouseX = GET_X_LPARAM(lParam);
				app->mouseY = GET_Y_LPARAM(lParam);
				if (app->mousePressed) app->mouseEvents.push_back({ MouseEvent::NONE, MouseEvent::DRAGGED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				else app->mouseEvents.push_back({ MouseEvent::NONE, MouseEvent::MOVED, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
				return 0;

			case WM_SIZE: app->OnResize(LOWORD(lParam), HIWORD(lParam));
				return 0;

			case WM_DESTROY: PostQuitMessage(0);
				return 1;
			}

			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
};

struct Knob : Drawable
{

	float x = 0, y = 0, w = 50, h = 50;
	double val = 0, aval = 0;
	float xo = 0, yo = 0;
	
	Knob() : Drawable(), color(0)
	{
	}

	SolidBrush* color;
	void Draw(Canvas& canvas)
	{
		if (hovering) canvas.CreateSolidColorBrush({ 0.78, 0.78, 0.78, 1 }, &color);
		else canvas.CreateSolidColorBrush({ 0.75, 0.75, 0.75, 1 }, &color);
		canvas.FillEllipse({ x, y, w, h }, color);
		canvas.CreateSolidColorBrush({ 1, 1, 1, 1 }, &color);
		canvas.DrawEllipse({ x, y, w, h }, color);
		
		double angle = val * PI * 1.7 - PI * 1.35;
		double r = w;
		float yp = std::sin(angle) * r;
		float xp = std::cos(angle) * r;
		canvas.DrawLine({ x, y},{ x + xp, y + yp }, color, 10);

		float s = 0.6;
		canvas.CreateSolidColorBrush({ 0.95, 0.95, 0.95, 1 }, &color);
		canvas.FillEllipse({ x, y, w * s, h * s }, color);
		canvas.CreateSolidColorBrush({ 0.70, 0.70, 0.70, 1 }, &color);
		canvas.DrawEllipse({ x, y, w * s, h * s }, color);

		val = val * 0.8 + aval * 0.2;
	}

	bool WithinBounds(int x, int y)
	{
		return Bounds::Ellipse(x, y, this->x, this->y, w, h);
	}

	void Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void MousePressed(MouseEvent& e)
	{
		cursor = 0;
		xo = e.x;
		yo = e.y;
	}

	void MouseReleased(MouseEvent& e)
	{
		cursor = IDC_ARROW;
	}

	void MouseDragged(MouseEvent& e)
	{
		aval = std::fmax(std::fmin(val + 0.015*(yo - e.y), 1), 0);
		window->CursorPos(xo, yo);
	}
};

struct Sensor : Drawable
{

	float x = 0, y = 0, w = 50, h = 400;
	double val = 1;
	float xo = 0, yo = 0;

	Sensor() : Drawable(), color(0)
	{
	}

	SolidBrush* color;
	GradientBrush* gradient;
	void Draw(Canvas& canvas)
	{
		if (hovering) canvas.CreateSolidColorBrush({ 0.78, 0.78, 0.78, 1 }, &color);
		else canvas.CreateSolidColorBrush({ 0.75, 0.75, 0.75, 1 }, &color);
		canvas.FillRectangle({ x, y, x + w, y + h }, color);
		
		canvas.CreateSolidColorBrush({ 0.75, 0, 0, 1 }, &color);
		canvas.FillRectangle({ x, y + h - (float)val*h, x + w, y + h }, color);
	}

	bool WithinBounds(int x, int y)
	{
		return Bounds::Rectangle(x, y, this->x, this->y, w, h);
	}

	void Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void MouseMoved(MouseEvent& e) 
	{
		val = std::fmax(std::fmin(1 - (e.y - y)/h, 1), 0);
	}

	void MouseExited(MouseEvent& e)
	{
		val = 1;
	}
};
#endif
#endif