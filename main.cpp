#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>

int main(){
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    
    // Set values
    const float pi = 3.1415f;
    const int windowWidth = 900;
    const int windowHeight = 600;
    sf::Vector2f paddleSize(25, 100);
    float ballRad = 10.f;
    
    // Create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Ping Pong");
    window.setVerticalSyncEnabled(true);
    
    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRad - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color::White);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRad / 2, ballRad / 2);
    
    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color::White);
    rightPaddle.setFillColor(sf::Color::Red);
    rightPaddle.setOrigin(paddleSize / 2.f);
    
    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color::White);
    leftPaddle.setFillColor(sf::Color::Blue);
    leftPaddle.setOrigin(paddleSize / 2.f);
    
    // Paddles Properties
    const float paddleSpeed = 400.f;
    const float ballSpeed   = 400.f;
    float ballAngle         = 0.f;//will adjust
    
    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("/Users/ammarrangoonwala/documents/pingPongGame/pingPongGame/sansation.ttf"))
        return EXIT_FAILURE;
    
    // Start Menu
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(170.f, 150.f);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("Press the space bar to begin");
    
    sf::Clock clock;
    bool isPlaying = false;
    while (window.isOpen()){
       
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)){
            
            // When window is closed or escape key pressed -> exit
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))){
                window.close();
                break;
            }
            
            // When space key is pressed -> play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)){
                if (!isPlaying){
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();
                    
                    // Reset the position of the paddles and ball
                    leftPaddle.setPosition(10 + paddleSize.x / 2, windowHeight / 2);
                    rightPaddle.setPosition(windowWidth - 10 - paddleSize.x / 2, windowHeight / 2);
                    ball.setPosition(windowWidth / 2, windowHeight / 2);
                    
                    // Reset the ball
                    do{
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    }
                    while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
            }
        }
        
        if (isPlaying){
            float deltaTime = clock.restart().asSeconds();
            
            // Move Player 1 (Red)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f)){
                leftPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (leftPaddle.getPosition().y + paddleSize.y / 2 < windowHeight - 5.f)){
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
            }
            
            //Move Player 2 (Blue)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)){
                rightPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (rightPaddle.getPosition().y + paddleSize.y / 2 < windowHeight - 5.f)){
                rightPaddle.move(0.f, paddleSpeed * deltaTime);
            }
            
            // Move the ball
            float factor = ballSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
            
            // Check for winner
            if (ball.getPosition().x - ballRad < 0.f){
                isPlaying = false;
                pauseMessage.setString("Red Won!\nPress the space bar to start over \nor press escape to quit");
            }
            if (ball.getPosition().x + ballRad > windowWidth){
                isPlaying = false;
                pauseMessage.setString("Blue won!\nPress the space bar to start over \nor press escape to quit");
            }
            if (ball.getPosition().y - ballRad < 0.f){
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRad + 0.1f);
            }
            if (ball.getPosition().y + ballRad > windowHeight){
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, windowHeight - ballRad - 0.1f);
            }
            
            // Paddle and ball contact
            
            // Left Paddle
            if (ball.getPosition().x - ballRad < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRad > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRad >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRad <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
                
                ball.setPosition(leftPaddle.getPosition().x + ballRad + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }
            
            // Right Paddle
            if (ball.getPosition().x + ballRad > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRad < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRad >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRad <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
                
                ball.setPosition(rightPaddle.getPosition().x - ballRad - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
        }
        
        // Clear the window
        window.clear(sf::Color::Black);
        
        if (isPlaying){
            // Draw the paddles and the ball
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        } else{
            // Draw the menu
            window.draw(pauseMessage);
        }
        // Display screen
        window.display();
    }
    
    return EXIT_SUCCESS;
}
