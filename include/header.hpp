#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include <time.h>
#include <windows.h>
#include <math.h>
#include <string.h>

#include <SFML/Graphics.hpp>
#include "json/json.h"

extern sf::RenderWindow window;

namespace logger {
void init();

void print_log(const std::string& text);
}; // namespace logger

namespace son {
extern bool shift_key_is_down;
extern bool numpad0_key_is_down;
void on_sf_event_key_pressed(int frame_count, sf::Event event);
void on_sf_event_key_released(int frame_count, sf::Event event);
void update(int frame_count);
}; // namespace son

namespace data {
extern Json::Value cfg;

void error_msg(std::string error, std::string title);

bool init();

sf::Texture &load_texture(std::string path);
}; // namespace data

namespace osu {
bool init();

void draw();
}; // namespace osu

namespace taiko {
bool init();

void draw();
}; // namespace taiko

namespace ctb {
bool init();

void draw();
}; // namespace ctb

namespace mania {
bool init();

void draw();
}; // namespace mania

namespace kb {
bool init();

void draw();
}; // namespace mania
