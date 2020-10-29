#include "header.hpp"

sf::RenderWindow window;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // loading configs
    while (!data::init()) {
        continue;
    }

    int window_width = data::cfg["window"]["width"].asInt();
    int window_height = data::cfg["window"]["height"].asInt();
    window.create(sf::VideoMode(window_width, window_height), "Bongo Cat for osu!", sf::Style::Titlebar | sf::Style::Close);

    // get refresh rate and set the frame limit
    DISPLAY_DEVICE device;
    ZeroMemory(&device, sizeof(device));
    device.cb = sizeof(device);
    EnumDisplayDevices(NULL, (DWORD)0, &device, 0);
    DEVMODE devmode;
    ZeroMemory(&devmode, sizeof(DEVMODE));
    devmode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings((LPSTR)device.DeviceName, ENUM_REGISTRY_SETTINGS, &devmode);
    window.setFramerateLimit(devmode.dmDisplayFrequency);

    bool is_reload = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            // get reload config prompt
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R && event.key.control) {
                    if (!is_reload) {
                        while (!data::init()) {
                            continue;
                        }
                    }
                    is_reload = true;
                    break;
                }
            default:
                is_reload = false;
            }
        }

        int mode = data::cfg["mode"].asInt();
        int red_value = data::cfg["decoration"]["rgb"][0].asInt();
        int green_value = data::cfg["decoration"]["rgb"][1].asInt();
        int blue_value = data::cfg["decoration"]["rgb"][2].asInt();

        window.clear(sf::Color(red_value, green_value, blue_value));
        switch (mode) {
        case 1:
            osu::draw();
            break;
        case 2:
            taiko::draw();
            break;
        case 3:
            ctb::draw();
            break;
        case 4:
            mania::draw();
            break;
        case 5:
            kb::draw();
            break;
        }

        window.display();
    }

    return 0;
}
