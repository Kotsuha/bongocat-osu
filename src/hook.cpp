#include "header.hpp"

namespace hook
{

	HHOOK hHook = NULL;
	LRESULT CALLBACK HookProc(int nCode, WPARAM wp, LPARAM lp);

	typedef void (*Callback)(WPARAM message, PKBDLLHOOKSTRUCT eventInfo);
	Callback callback = NULL;

	void SetCallback(Callback function) // pass null to unset
	{
		callback = function;
	}

	BOOL HookKeyboard()
	{
		if (hHook != NULL)
		{
			return FALSE;
		}

		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, 0, 0);
		if (hHook == NULL)
		{
			DWORD dw = GetLastError();
			logger::print_log(std::to_string(dw));
		}
		return hHook != NULL;
	}

	BOOL UnhookKeyboard()
	{
		if (hHook == NULL)
		{
			return FALSE;
		}
		BOOL ret = UnhookWindowsHookEx(hHook);
		if (ret)
		{
			hHook = NULL;
		}
		return ret;
	}

	LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode < 0)
		{
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		}

		if (nCode == HC_ACTION)
		{
			// Do things here

			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			// DWORD vkCode = p->vkCode; // A virtual-key code.
			// DWORD scanCode = p->scanCode;
			// DWORD flags = p->flags;
			// DWORD time = p->time;
			// switch (wParam)
			// {
			// 	case WM_KEYDOWN:
			// 		logger::print_log("[" + std::to_string(get_frame_count()) + "] WM_KEYDOWN: " + std::to_string(vkCode));
			// 		logger::print_log("-> scanCode: " + std::to_string(vkCode));
			// 		logger::print_log("-> time: " + std::to_string(time));
			// 		break;

			// 	case WM_KEYUP:
			// 		logger::print_log("[" + std::to_string(get_frame_count()) + "] WM_KEYUP: " + std::to_string(vkCode));
			// 		logger::print_log("-> scanCode: " + std::to_string(vkCode));
			// 		logger::print_log("-> time: " + std::to_string(time));
			// 		break;

			// 	// case WM_SYSKEYDOWN:
			// 	// 	break;
			// 	// case WM_SYSKEYUP:
			// 	// 	break;
			// }

			if (callback != NULL)
			{
				(*callback)(wParam, p);
			}
		}

		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}

}

// Reference:
// https://stackoverflow.com/questions/22975916/global-keyboard-hook-with-wh-keyboard-ll-and-keybd-event-windows
// https://blog.csdn.net/qq_29020861/article/details/54865332

// https://stackoverflow.com/questions/22975916/global-keyboard-hook-with-wh-keyboard-ll-and-keybd-event-windows#comment85433110_24276756
// > All global hooks must be in a DLL unless it is a low-level hook. The documentation is lacking on this.
// Seems like I need to use WH_KEYBOARD_LL instead of WH_KEYBOARD to avoid DLL

// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644985(v=vs.85)

/*

Shift Down -> Numpad0 Down -> Numpad0 Up -> Shift Up
[1845] WM_KEYDOWN: 160
[1856] WM_KEYUP: 160
[1857] WM_KEYDOWN: 45
[1866] WM_KEYUP: 45
[1867] WM_KEYDOWN: 160
[1880] WM_KEYUP: 160

Shift Down -> Numpad0 Down -> Shift Up -> Numpad0 Up
[8494] WM_KEYDOWN: 160
[8503] WM_KEYUP: 160
[8504] WM_KEYDOWN: 45
[8513] WM_KEYDOWN: 160
[8514] WM_KEYUP: 160
[8523] WM_KEYUP: 96

Numpad0 Down -> Shift Down -> Shift Up -> Numpad0 Up
[22672] WM_KEYDOWN: 96
[22684] WM_KEYDOWN: 160
[22690] WM_KEYUP: 160
[22702] WM_KEYUP: 96

Numpad0 Down -> Shift Down -> Numpad0 Up -> Shift Up
[24366] WM_KEYDOWN: 96
[24384] WM_KEYDOWN: 160
[24401] WM_KEYUP: 160
[24402] WM_KEYUP: 45
[24413] WM_KEYDOWN: 160
[24414] WM_KEYDOWN: 160
[24415] WM_KEYDOWN: 160
[24417] WM_KEYDOWN: 160
[24419] WM_KEYDOWN: 160
[24421] WM_KEYDOWN: 160
[24422] WM_KEYUP: 160
*/

/*
[5942] WM_KEYUP: 160
-> scanCode: 160
-> time: 458379687
[5943] WM_KEYDOWN: 45
-> scanCode: 45
-> time: 458379703
[5950] WM_KEYUP: 45
-> scanCode: 45
-> time: 458379812
[6762] WM_KEYDOWN: 160
-> scanCode: 160
-> time: 458393593
[6774] WM_KEYUP: 160
-> scanCode: 160
-> time: 458393796
[6775] WM_KEYDOWN: 45
-> scanCode: 45
-> time: 458393796
[6781] WM_KEYUP: 45
-> scanCode: 45
-> time: 458393906
[6782] WM_KEYDOWN: 160
-> scanCode: 160
-> time: 458393906
[6795] WM_KEYUP: 160
-> scanCode: 160
-> time: 458394156

The 'Same Frame' strategy does work anymore. But 'Same Time' seems good.
*/