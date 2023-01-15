#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

const int WIDTH = 1600;
const int HEIGHT = 900;

int main()
{
    int frame_count = 0;
    int current_fps = 60;
    float current_time1;
    float current_time2;

    // create the window
    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Raymarching on GPU", sf::Style::Default);
    window.setFramerateLimit(60);
    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    sf::Shader shaderF;
    shaderF.loadFromFile("example.frag", sf::Shader::Fragment);
    sf::Glsl::Vec2 res(WIDTH, HEIGHT);
    shaderF.setUniform("iRes", res);

    sf::Clock clock1;
    sf::Clock clock2;

    sf::Image image;
    image.create(1600, 900, sf::Color::Transparent);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    // run the main loop
    bool running = true;
    while (running)
    {
        window.clear(sf::Color::Transparent);

        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                sf::Glsl::Vec2 res(event.size.width, event.size.height);

                shaderF.setUniform("iRes", res);
            }

        }

        current_time2 = clock2.getElapsedTime().asSeconds();

        shaderF.setUniform("iTime", current_time2);

        // draw...
        window.draw(sprite, &shaderF);


        // Show FPS
        current_time1 = clock1.getElapsedTime().asSeconds();

        frame_count += 1;
        if (current_time1 >= 1.0)
        {
            current_fps = frame_count;
            frame_count = 0;
            clock1.restart();
        }

        std::ostringstream display_fps;
        display_fps << "FPS: " << current_fps;
        sf::Font font;
        font.loadFromFile("brohoney.ttf");
        sf::Text text(display_fps.str(), font, 40);
        text.setFillColor(sf::Color::White);
        text.setPosition(37.f, 27.f);
        text.setOrigin(0.f, 0.f);
        window.draw(text);

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}