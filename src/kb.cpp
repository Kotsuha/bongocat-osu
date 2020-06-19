#include "header.hpp"
#include <fstream>
#include <vector>

namespace kb
{
const int NUM = 16;
sf::Sprite bg, left_hand_up, right_hand_up;
sf::Sprite left_hand[NUM], right_hand[NUM];
sf::Sprite left_hand_key[NUM], right_hand_key[NUM], no_hand_key[NUM];
int left_hand_key_value[NUM], right_hand_key_value[NUM], no_hand_key_value[NUM];
int left_hand_pos = -1, right_hand_pos = -1;
bool left_hand_key_is_down[NUM], right_hand_key_is_down[NUM];
bool left_hand_key_was_down[NUM], right_hand_key_was_down[NUM];
std::vector<int> left_hand_key_down_order, right_hand_key_down_order;

std::ofstream log_file;
bool is_debug = false;

bool init()
{
    for (int i = 0; i < NUM; i++)
    {
        left_hand_key_value[i] = data::cfg["kb"]["left_hand_keys"][i].asInt();
        right_hand_key_value[i] = data::cfg["kb"]["right_hand_keys"][i].asInt();
        no_hand_key_value[i] = data::cfg["kb"]["no_hand_keys"][i].asInt();
    }

    // importing sprites
    bg.setTexture(data::load_texture("img/kb/bg.png"));
    left_hand_up.setTexture(data::load_texture("img/kb/left_hand_up.png"));
    right_hand_up.setTexture(data::load_texture("img/kb/right_hand_up.png"));
    for (int i = 0; i < NUM; i++)
    {
        if (left_hand_key_value[i] >= 0)
        {
            left_hand_key[i].setTexture(data::load_texture("img/kb/left_hand_key_" + std::to_string(i) + ".png"));
            left_hand[i].setTexture(data::load_texture("img/kb/left_hand_" + std::to_string(i) + ".png"));
        }
        if (right_hand_key_value[i] >= 0)
        {
            right_hand_key[i].setTexture(data::load_texture("img/kb/right_hand_key_" + std::to_string(i) + ".png"));
            right_hand[i].setTexture(data::load_texture("img/kb/right_hand_" + std::to_string(i) + ".png"));
        }
        if (no_hand_key_value[i] >= 0)
        {
            no_hand_key[i].setTexture(data::load_texture("img/kb/no_hand_key_" + std::to_string(i) + ".png"));
        }
    }

    if (is_debug)
    {
        log_file.open("debug.log");
    }

    return true;
}

void print_log(const std::string& text)
{
    if (!is_debug)
    {
        return;
    }
    log_file << text << std::endl;
}

void draw_7K()
{
    // Draw bg
    window.draw(bg);

    // Get key down info & Draw keys
    for (int i = 0; i < NUM; i++)
    {
        left_hand_key_is_down[i] = (left_hand_key_value[i] > 0 && GetKeyState(left_hand_key_value[i]) & WM_KEYDOWN);
        right_hand_key_is_down[i] = (right_hand_key_value[i] > 0 && GetKeyState(right_hand_key_value[i]) & WM_KEYDOWN);

        if (!left_hand_key_was_down[i] && left_hand_key_is_down[i])
        {
            left_hand_key_down_order.insert(left_hand_key_down_order.begin(), i);
        }
        else if (left_hand_key_was_down[i] && !left_hand_key_is_down[i])
        {
            left_hand_key_down_order.erase(std::find(left_hand_key_down_order.begin(), left_hand_key_down_order.end(), i));
        }

        if (!right_hand_key_was_down[i] && right_hand_key_is_down[i])
        {
            right_hand_key_down_order.insert(right_hand_key_down_order.begin(), i);
        }
        else if (right_hand_key_was_down[i] && !right_hand_key_is_down[i])
        {
            right_hand_key_down_order.erase(std::find(right_hand_key_down_order.begin(), right_hand_key_down_order.end(), i));
        }

        if (left_hand_key_is_down[i])
        {
            window.draw(left_hand_key[i]);
        }
        if (right_hand_key_is_down[i])
        {
            window.draw(right_hand_key[i]); 
        }
        if (no_hand_key_value[i] > 0 && GetKeyState(no_hand_key_value[i]) & WM_KEYDOWN)
        {
            window.draw(no_hand_key[i]); 
        }
    }

    // Determine hand positions
    left_hand_pos = left_hand_key_down_order.empty() ? -1 : *left_hand_key_down_order.begin();
    right_hand_pos = right_hand_key_down_order.empty() ? -1 : *right_hand_key_down_order.begin();

    // Draw hands
    window.draw(left_hand_pos == -1 ? left_hand_up : left_hand[left_hand_pos]);
    window.draw(right_hand_pos == -1 ? right_hand_up : right_hand[right_hand_pos]);

    // Update key down
    for (int i = 0; i < NUM; i++)
    {
        left_hand_key_was_down[i] = left_hand_key_is_down[i];
        right_hand_key_was_down[i] = right_hand_key_is_down[i];
    }

    return;
}

void draw()
{
    draw_7K();
}
}; // namespace kb
