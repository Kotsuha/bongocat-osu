#include "header.hpp"

sf::RenderWindow window;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // open log file if needed
    logger::init();

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

    int frame_count = -1; // Unity also uses int

    while (window.isOpen()) {
        frame_count++;
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
                son::on_sf_event_key_pressed(frame_count, event);
                break;

            case sf::Event::KeyReleased:
                son::on_sf_event_key_released(frame_count, event);
                break;

            default:
                is_reload = false;
            }
        }

        son::update(frame_count);


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

        // bool insert_is_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Insert);
        // logger::print_log("Insert is down: " + std::to_string(insert_is_down));
        // bool insert_is_down = GetKeyState(VK_INSERT) & WM_KEYDOWN;
        // logger::print_log("Insert is down: " + std::to_string(insert_is_down));
        // bool insert_is_down = GetAsyncKeyState(VK_INSERT) & 0x8000;
        // logger::print_log("Insert is down: " + std::to_string(insert_is_down));

        window.display();
    }

    return 0;
}
