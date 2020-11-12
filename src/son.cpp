// The shift key overrides NumLock
// https://devblogs.microsoft.com/oldnewthing/20040906-00/?p=37953

#include "header.hpp"

namespace son
{
	DWORD shift_keydown_time;
	DWORD shift_keyup_time;
	DWORD insert_keyup_time;

	bool shift_key_is_down = false;
	bool numpad0_key_is_down = false;
	// bool numpad1_key_is_down = false;
	// bool numpad2_key_is_down = false;
	// bool numpad3_key_is_down = false;
	// bool numpad4_key_is_down = false;
	// bool numpad5_key_is_down = false;
	// ...

	bool start_hook();
	bool stop_hook();
	void proc(WPARAM message, PKBDLLHOOKSTRUCT event_info);
	void proc_key_down(PKBDLLHOOKSTRUCT event_info);
	void proc_key_up(PKBDLLHOOKSTRUCT event_info);

	bool start_hook()
	{
		hook::SetCallback(proc);
		bool result = hook::HookKeyboard();
		return result;
	}

	bool stop_hook()
	{
		hook::SetCallback(NULL);
		bool result = hook::UnhookKeyboard();
		return result;
	}

	void proc(WPARAM message, PKBDLLHOOKSTRUCT event_info)
	{
		switch (message)
		{
			case WM_KEYDOWN: proc_key_down(event_info); break;
			case WM_KEYUP:   proc_key_up(event_info);   break;
		}
	}

	void proc_key_down(PKBDLLHOOKSTRUCT event_info)
	{
		bool flag = false;
		switch (event_info->vkCode)
		{
			case VK_SHIFT: // 16
			case VK_LSHIFT: // 160
			case VK_RSHIFT: // 161
				logger::print_log("WM_KEYDOWN VK_LSHIFT");
				flag = true;
				shift_keydown_time = event_info->time;
				shift_key_is_down = true;
				if (event_info->time == insert_keyup_time)
				{
					numpad0_key_is_down = false;
				}
				break;

			case VK_NUMPAD0:
				logger::print_log("WM_KEYDOWN VK_NUMPAD0");
				flag = true;
				numpad0_key_is_down = true;
				break;

			case VK_INSERT:
				logger::print_log("WM_KEYDOWN VK_INSERT");
				flag = true;
				if (event_info->time == shift_keyup_time) // Shift+Numpad0
				{
					numpad0_key_is_down = true;
				}
				break;
		}
		if (flag)
		{
			logger::print_log("-> key_code: " + std::to_string(event_info->vkCode));
			logger::print_log("-> scan_code: " + std::to_string(event_info->scanCode));
			logger::print_log("-> is_extended: " + std::to_string((event_info->flags & LLKHF_EXTENDED)));
			logger::print_log("-> is_injected: " + std::to_string((event_info->flags & LLKHF_INJECTED)));
			logger::print_log("-> is_alt_down: " + std::to_string((event_info->flags & LLKHF_ALTDOWN)));
			logger::print_log("-> is_up: " + std::to_string((event_info->flags & LLKHF_UP)));
			logger::print_log("-> extra_info: " + std::to_string(GetMessageExtraInfo()));

			WCHAR wch_key_name[16];
			char ch_key_name[16];
			LONG lParam = (event_info->scanCode << 16);
			int result = GetKeyNameTextW(lParam, wch_key_name, 16);
			if (result > 0)
			{
				char defult_char = ' ';
				WideCharToMultiByte(CP_ACP, 0, wch_key_name, -1, ch_key_name, 16, &defult_char, NULL);
			}
			logger::print_log(std::string("-> key_name: ") + ch_key_name);
		}
	}

	void proc_key_up(PKBDLLHOOKSTRUCT event_info)
	{
		bool flag = false;
		switch (event_info->vkCode)
		{
			case VK_SHIFT:
			case VK_LSHIFT: // ?
			case VK_RSHIFT: // ?
				logger::print_log("WM_KEYUP   VK_LSHIFT");
				flag = true;
				shift_keyup_time = event_info->time;
				if (event_info->scanCode != 554)
				{
					shift_key_is_down = false;
				}
				break;

			case VK_NUMPAD0:
				logger::print_log("WM_KEYUP   VK_NUMPAD0");
				flag = true;
				numpad0_key_is_down = false;
				break;

			case VK_INSERT:
				logger::print_log("WM_KEYUP   VK_INSERT");
				flag = true;
				insert_keyup_time = event_info->time;
				if (event_info->time == shift_keyup_time)
				{
					numpad0_key_is_down = false;
				}
				break;
		}
		if (flag)
		{
			logger::print_log("-> key_code: " + std::to_string(event_info->vkCode));
			logger::print_log("-> scan_code: " + std::to_string(event_info->scanCode));
			logger::print_log("-> is_extended: " + std::to_string((event_info->flags & LLKHF_EXTENDED)));
			logger::print_log("-> is_injected: " + std::to_string((event_info->flags & LLKHF_INJECTED)));
			logger::print_log("-> is_alt_down: " + std::to_string((event_info->flags & LLKHF_ALTDOWN)));
			logger::print_log("-> is_up: " + std::to_string((event_info->flags & LLKHF_UP)));
			logger::print_log("-> extra_info: " + std::to_string(GetMessageExtraInfo()));

			WCHAR wch_key_name[16];
			char ch_key_name[16];
			LONG lParam = (event_info->scanCode << 16);
			int result = GetKeyNameTextW(lParam, wch_key_name, 16);
			if (result > 0)
			{
				char defult_char = ' ';
				WideCharToMultiByte(CP_ACP, 0, wch_key_name, -1, ch_key_name, 16, &defult_char, NULL);
			}
			logger::print_log(std::string("-> key_name: ") + ch_key_name);
		}
	}

	/*
	bool shift_key_is_down = false;
	bool shift_key_was_down = false;
	bool numpad0_key_is_down = false;
	bool numpad0_key_was_down = false;
	int shift_key_pressed_frame_count = -1;
	int shift_key_released_frame_count = -1;
	int insert_key_released_frame_count = -1;

	void on_sf_event_key_pressed(int frame_count, sf::Event event)
	{
		// switch (event.key.code)
		// {
		// 	case sf::Keyboard::LShift:
		// 		logger::print_log("[" + std::to_string(frame_count) + "] LShift KeyPressed");
		// 		shift_key_pressed_frame_count = frame_count;
		// 		shift_key_is_down = true;
		// 		if (frame_count == insert_key_released_frame_count)
		// 		{
		// 			numpad0_key_is_down = false;
		// 		}
		// 		break;

		// 	case sf::Keyboard::Numpad0:
		// 		logger::print_log("[" + std::to_string(frame_count) + "] Numpad0 KeyPressed");
		// 		numpad0_key_is_down = true;
		// 		break;

		// 	case sf::Keyboard::Insert: // Numpad0
		// 		logger::print_log("[" + std::to_string(frame_count) + "] Insert KeyPressed");
		// 		if (frame_count == shift_key_released_frame_count)
		// 		{
		// 			shift_key_is_down = true;
		// 			numpad0_key_is_down = true;
		// 		}
		// 		break;
		// 	// case sf::Keyboard::End:  // Numpad1
		// 	// case sf::Keyboard::Down: // Numpad2
		// 	// ...
		// }
	}

	void on_sf_event_key_released(int frame_count, sf::Event event)
	{
		// switch (event.key.code)
		// {
		// 	case sf::Keyboard::LShift:
		// 		logger::print_log("[" + std::to_string(frame_count) + "] LShift KeyReleased");
		// 		shift_key_released_frame_count = frame_count;
		// 		shift_key_is_down = false;
		// 		break;

		// 	case sf::Keyboard::Numpad0:
		// 		logger::print_log("[" + std::to_string(frame_count) + "] Numpad0 KeyReleased");
		// 		numpad0_key_is_down = false;
		// 		break;

		// 	case sf::Keyboard::Insert: // Numpad0
		// 		logger::print_log("[" + std::to_string(frame_count) + "] Insert KeyReleased");
		// 		insert_key_released_frame_count = frame_count;
		// 		if (frame_count == shift_key_released_frame_count)
		// 		{
		// 			shift_key_is_down = true;
		// 			numpad0_key_is_down = false;
		// 		}
		// 		break;
		// }
	}

	void update(int frame_count)
	{
		// // shift_key_is_down = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || shift_overrides_numlock;
		// shift_key_is_down = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
		// numpad0_key_is_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0);
		// bool shift_key_pressed = !shift_key_was_down && shift_key_is_down;
		// bool shift_key_released = shift_key_was_down && !shift_key_is_down;
		// bool numpad0_key_pressed = !numpad0_key_was_down && numpad0_key_is_down;
		// bool numpad0_key_released = numpad0_key_was_down && !numpad0_key_is_down;
		// if (shift_key_pressed)
		// {

		// }
		
		shift_key_was_down = shift_key_is_down;
		numpad0_key_was_down = numpad0_key_is_down;
	}

	int nk_code_to_nak_code(int nk_code)
	{
		switch (nk_code)
		{
			case VK_NUMPAD0: return VK_INSERT;
			case VK_NUMPAD1: return VK_END;
			case VK_NUMPAD2: return VK_DOWN;
			// ...
			default: return -1;
		}
	}
	*/
}
