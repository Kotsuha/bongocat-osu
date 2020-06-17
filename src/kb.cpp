#include "header.hpp"

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
int curr_left_hand_key_down_order = 0, curr_right_hand_key_down_order = 0, left_hand_key_down_order[NUM] = {0}, right_hand_key_down_order[NUM] = {0};

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

    return true;
}

void draw_7K()
{
    // Draw bg
    window.draw(bg);

    // Draw keys
    for (int i = 0; i < NUM; i++)
    {
        left_hand_key_is_down[i] = (left_hand_key_value[i] > 0 && GetKeyState(left_hand_key_value[i]) & WM_KEYDOWN);
        right_hand_key_is_down[i]= (right_hand_key_value[i] > 0 && GetKeyState(right_hand_key_value[i]) & WM_KEYDOWN);

        if (!left_hand_key_was_down[i] && left_hand_key_is_down[i])
        {
            curr_left_hand_key_down_order++;
            if (curr_left_hand_key_down_order == INT_MIN)
            {
                for (int j = 0; j < NUM; j++)
                {
                    left_hand_key_down_order[j] += NUM;
                }
                curr_left_hand_key_down_order += NUM;
            }
            left_hand_key_down_order[i] = curr_left_hand_key_down_order;
        }
        if (!right_hand_key_was_down[i] && right_hand_key_is_down[i])
        {
            curr_right_hand_key_down_order++;
            if (curr_right_hand_key_down_order == INT_MIN)
            {
                for (int j = 0; j < NUM; j++)
                {
                    right_hand_key_down_order[j] += NUM;
                }
                curr_right_hand_key_down_order += NUM;
            }
            right_hand_key_down_order[i] = curr_right_hand_key_down_order;
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
    int tmp_most_recent_order = INT_MIN;
    left_hand_pos = -1;
    for (int i = 0; i < NUM; i++)
    {
        if (left_hand_key_is_down[i] && left_hand_key_down_order[i] >= tmp_most_recent_order)
        {
            tmp_most_recent_order = left_hand_key_down_order[i];
            left_hand_pos = i;
        }
    }
    right_hand_pos = -1;
    for (int i = 0; i < NUM; i++)
    {
        if (right_hand_key_is_down[i] && right_hand_key_down_order[i] >= tmp_most_recent_order)
        {
            tmp_most_recent_order = right_hand_key_down_order[i];
            right_hand_pos = i;
        }
    }

    // Draw hands
    window.draw( left_hand_pos == -1 ?  left_hand_up :  left_hand[ left_hand_pos]);
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
