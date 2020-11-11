// The shift key overrides NumLock
// https://devblogs.microsoft.com/oldnewthing/20040906-00/?p=37953

#include "header.hpp"

namespace son
{
	bool shift_key_is_down = false;
	bool numpad0_key_is_down = false;
	int shift_key_pressed_frame_count = -1;
	int shift_key_released_frame_count = -1;
	int insert_key_released_frame_count = -1;

	void on_sf_event_key_pressed(int frame_count, sf::Event event)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::LShift:
				logger::print_log("[" + std::to_string(frame_count) + "] LShift KeyPressed");
				shift_key_pressed_frame_count = frame_count;
				shift_key_is_down = true;
				if (frame_count == insert_key_released_frame_count)
				{
					numpad0_key_is_down = false;
				}
				break;

			case sf::Keyboard::Numpad0:
				logger::print_log("[" + std::to_string(frame_count) + "] Numpad0 KeyPressed");
				numpad0_key_is_down = true;
				break;

			case sf::Keyboard::Insert: // Numpad0
				logger::print_log("[" + std::to_string(frame_count) + "] Insert KeyPressed");
				if (frame_count == shift_key_released_frame_count)
				{
					shift_key_is_down = true;
					numpad0_key_is_down = true;
				}
				break;
			// case sf::Keyboard::End:  // Numpad1
			// case sf::Keyboard::Down: // Numpad2
			// ...
		}
	}

	void on_sf_event_key_released(int frame_count, sf::Event event)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::LShift:
				logger::print_log("[" + std::to_string(frame_count) + "] LShift KeyReleased");
				shift_key_released_frame_count = frame_count;
				shift_key_is_down = false;
				break;

			case sf::Keyboard::Numpad0:
				logger::print_log("[" + std::to_string(frame_count) + "] Numpad0 KeyReleased");
				numpad0_key_is_down = false;
				break;

			case sf::Keyboard::Insert: // Numpad0
				logger::print_log("[" + std::to_string(frame_count) + "] Insert KeyReleased");
				insert_key_released_frame_count = frame_count;
				if (frame_count == shift_key_released_frame_count)
				{
					shift_key_is_down = true;
					numpad0_key_is_down = false;
				}
				break;
		}
	}

	void update(int frame_count)
	{
		// shift_key_is_down = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || shift_overrides_numlock;
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
}
