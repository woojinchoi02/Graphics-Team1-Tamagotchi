#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <thread> // �߰�

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;


#define GAME_WINDOW_WIDTH 400
#define GAME_WINDOW_HEIGHT 600


class Block {
public:
    sf::RectangleShape shape;
    bool isVisible;

    Block(float startX, float startY) {
        shape.setSize(sf::Vector2f(50, 20));
        shape.setPosition(startX, startY);
        shape.setFillColor(sf::Color::Red);
        isVisible = true;
    }
};

class BlockBreakerGame {
public:
    BlockBreakerGame() : window(sf::VideoMode(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), "Block Breaker Game", sf::Style::None) {
        // Set up paddle
        paddle.setSize(sf::Vector2f(100, 20));
        paddle.setPosition(GAME_WINDOW_WIDTH / 2 - paddle.getSize().x / 2, GAME_WINDOW_HEIGHT - 50);
        paddle.setFillColor(sf::Color::Green);

        // Set up ball
        ball.setRadius(10);
        ball.setPosition(GAME_WINDOW_WIDTH / 2 - ball.getRadius(), GAME_WINDOW_HEIGHT / 2 - ball.getRadius());
        ball.setFillColor(sf::Color::Blue);
        ballSpeed = sf::Vector2f(0.2f, 0.2f); // �ӵ� ����

        // Generate blocks
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 4; ++j) {
                blocks.push_back(Block(i * 60 + 20, j * 30 + 50));
            }
        }

        setupText();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::RectangleShape paddle;
    sf::CircleShape ball;
    sf::Vector2f ballSpeed;
    std::vector<Block> blocks;
    bool isGameOver = false;
    bool isSuccess = false;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text successText;

    void setupText() {
        if (!font.loadFromFile("arial.ttf")) {
            // ��Ʈ�� �ε��� �� ���� ���� ó��
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Game Over �ؽ�Ʈ ����
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("Game Over! Ball missed.");
        gameOverText.setPosition(GAME_WINDOW_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2,
            GAME_WINDOW_HEIGHT / 2 - gameOverText.getGlobalBounds().height / 2);

        // Success �ؽ�Ʈ ����
        successText.setFont(font);
        successText.setCharacterSize(30);
        successText.setFillColor(sf::Color::Green);
        successText.setString("Game Success!");
        successText.setPosition(GAME_WINDOW_WIDTH / 2 - successText.getGlobalBounds().width / 2,
            GAME_WINDOW_HEIGHT / 2 - successText.getGlobalBounds().height / 2);
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void update() {
        // Move paddle with mouse
        paddle.setPosition(sf::Mouse::getPosition(window).x - paddle.getSize().x / 2, paddle.getPosition().y);

        // Move ball
        ball.move(ballSpeed);

        // Ball collision with window edges
        if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= GAME_WINDOW_WIDTH)
            ballSpeed.x *= -1;
        if (ball.getPosition().y <= 0)
            ballSpeed.y *= -1;

        // Ball collision with paddle
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballSpeed.y *= -1;
        }

        // Ball collision with blocks
        for (auto& block : blocks) {
            if (block.isVisible && ball.getGlobalBounds().intersects(block.shape.getGlobalBounds())) {
                block.isVisible = false;
                ballSpeed.y *= -1;
            }
        }

        // Check if all blocks are destroyed
        bool allBlocksDestroyed = true;
        for (const auto& block : blocks) {
            if (block.isVisible) {
                allBlocksDestroyed = false;
                break;
            }
        }

        // If all blocks are destroyed, game success
        if (allBlocksDestroyed) {
            isSuccess = true;
        }

        // If ball reaches bottom without hitting paddle, game over
        if (ball.getPosition().y > GAME_WINDOW_HEIGHT) {
            isGameOver = true;
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        // Render paddle
        window.draw(paddle);

        // Render ball
        window.draw(ball);

        // Render blocks
        for (auto& block : blocks) {
            if (block.isVisible)
                window.draw(block.shape);
        }

        // Render game over text if game over
        if (isGameOver) {
            window.draw(gameOverText);
        }

        // Render success text if game success
        if (isSuccess) {
            window.draw(successText);
        }

        window.display();

        // If game over or success, wait for some time before closing
        if (isGameOver || isSuccess) {
            sf::sleep(sf::seconds(2)); // Wait for 3 seconds
            window.close();
        }
    }
};

class ShootingGame {
public:
    ShootingGame() : window(sf::VideoMode(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), "Shooting Game", sf::Style::Close) {
        // Load player texture
        if (!playerTexture.loadFromFile("pngwing.com.png")) {
            // Handle error if player texture loading fails
            std::cerr << "Failed to load player texture!" << std::endl;
            // You can add more sophisticated error handling here
        }

        // Set player sprite texture
        player.setTexture(playerTexture);

        // Set player initial position and scale
        player.setScale(0.5f, 0.5f); // Scale down the player sprite to fit the window better
        player.setPosition(GAME_WINDOW_WIDTH / 2 - player.getGlobalBounds().width / 2, GAME_WINDOW_HEIGHT - player.getGlobalBounds().height - 20);

        // Initialize bullets
        bullet.setRadius(5);
        bullet.setFillColor(sf::Color::White);

        // Initialize enemies
        for (int i = 0; i < 5; ++i) {
            enemies.push_back(sf::RectangleShape(sf::Vector2f(30, 30)));
            enemies[i].setPosition(i * 80 + 50, 50);
            enemies[i].setFillColor(sf::Color::Red);
        }
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Sprite player;
    sf::Texture playerTexture;
    sf::CircleShape bullet;
    std::vector<sf::RectangleShape> enemies;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                shoot();
            }
        }
    }

    void update() {
        // Move player left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move(-0.2f, 0);
        }
        // Move player right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move(0.2f, 0);
        }

        // Move bullet
        bullet.move(0, -1);

        // Check bullet collision with enemies
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (bullet.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
                enemies.erase(enemies.begin() + i);
                bullet.setPosition(player.getPosition().x + player.getGlobalBounds().width / 2, player.getPosition().y);
                break;
            }
        }

        // Check game over
        if (enemies.empty()) {
            window.close();
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        // Render player
        window.draw(player);

        // Render bullet
        window.draw(bullet);

        // Render enemies
        for (auto& enemy : enemies) {
            window.draw(enemy);
        }

        window.display();
    }

    void shoot() {
        // Only shoot if bullet is not already on screen
        if (bullet.getPosition().y < 0) {
            bullet.setPosition(player.getPosition().x + player.getGlobalBounds().width / 2, player.getPosition().y);
        }
    }
};


class Tamagotchi {
public:
    Tamagotchi() : level(1), experience(0), hunger(100), happiness(100), cleanliness(100), lastUpdateTime(sf::Clock().getElapsedTime().asMilliseconds()), evolved(false) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 3);
        a = dis(gen); // ������ ���� �� ����
        if (!tamagotchiTexture.loadFromFile(std::to_string(a) + std::to_string(1) + ".png")) {
            std::cerr << "Failed to load tamagotchi image!" << std::endl;
        }
        tamagotchiSprite.setTexture(tamagotchiTexture);
        tamagotchiSprite.setPosition((WINDOW_WIDTH - tamagotchiSprite.getGlobalBounds().width) / 2, (WINDOW_HEIGHT - tamagotchiSprite.getGlobalBounds().height) / 2);
       
        playButton = sf::RectangleShape(sf::Vector2f(200, 50));
        playButton.setPosition(100, 450);
        feedButton = sf::RectangleShape(sf::Vector2f(200, 50));
        feedButton.setPosition(350, 450);
        cleanButton = sf::RectangleShape(sf::Vector2f(200, 50));
        cleanButton.setPosition(600, 450);
        game1Button = sf::RectangleShape(sf::Vector2f(100, 30)); // �̴� ���� 1 ��
        game1Button.setPosition(600, 50);
        game2Button = sf::RectangleShape(sf::Vector2f(100, 30)); // �̴� ���� 2 ��
        game2Button.setPosition(600, 100);
        // ��ư �ؽ�Ʈ ����
        font.loadFromFile("arial.ttf"); // ��Ʈ �ε�
        playText.setFont(font);
        playText.setString("  Play");
        playText.setCharacterSize(20);
        playText.setFillColor(sf::Color::Black);
        playText.setPosition(playButton.getPosition().x + 60, playButton.getPosition().y + 10);
        feedText.setFont(font);
        feedText.setString("  Feed");
        feedText.setCharacterSize(20);
        feedText.setFillColor(sf::Color::Black);
        feedText.setPosition(feedButton.getPosition().x + 60, feedButton.getPosition().y + 10);
        cleanText.setFont(font);
        cleanText.setString("  Clean");
        cleanText.setCharacterSize(20);
        cleanText.setFillColor(sf::Color::Black);
        cleanText.setPosition(cleanButton.getPosition().x + 50, cleanButton.getPosition().y + 10);
        game1Text.setFont(font);
        game1Text.setString("Mini Game 1");
        game1Text.setCharacterSize(15);
        game1Text.setFillColor(sf::Color::Black);
        game1Text.setPosition(game1Button.getPosition().x + 10, game1Button.getPosition().y + 5);
        game2Text.setFont(font);
        game2Text.setString("Mini Game 2");
        game2Text.setCharacterSize(15);
        game2Text.setFillColor(sf::Color::Black);
        game2Text.setPosition(game2Button.getPosition().x + 10, game2Button.getPosition().y + 5);

        // ��ư Ŭ�� ���� �ε�
        if (!buttonClickBuffer.loadFromFile("button.wav")) {
            std::cerr << "Failed to load button click sound!" << std::endl;
        }
        buttonClickSound.setBuffer(buttonClickBuffer);

        if (!evolveSoundBuffer.loadFromFile("evolve.wav")) {//��ȭ ����
            std::cerr << "Failed to load evolve sound!" << std::endl;
        }
        evolveSound.setBuffer(evolveSoundBuffer);

        if (!levelSoundBuffer.loadFromFile("levelup.wav")) {//level ����
            std::cerr << "Failed to load level sound!" << std::endl;
        }
        levelSound.setBuffer(levelSoundBuffer);

        if (!foodSoundBuffer.loadFromFile("food.wav")) {//food ����
            std::cerr << "Failed to load food sound!" << std::endl;
        }
        foodSound.setBuffer(foodSoundBuffer);

        if (!cleanSoundBuffer.loadFromFile("clean.wav")) {//clean ����
            std::cerr << "Failed to load clean sound!" << std::endl;
        }
        cleanSound.setBuffer(cleanSoundBuffer);

        if (!playSoundBuffer.loadFromFile("play.wav")) {//��� ����
            std::cerr << "Failed to load play sound!" << std::endl;
        }
        playSound.setBuffer(playSoundBuffer);
        // Initialize percentage texts
        hungerText.setFont(font);
        happinessText.setFont(font);
        cleanlinessText.setFont(font);
        hungerText.setCharacterSize(15);
        happinessText.setCharacterSize(15);
        cleanlinessText.setCharacterSize(15);
        hungerText.setFillColor(sf::Color::Black);
        happinessText.setFillColor(sf::Color::Black);
        cleanlinessText.setFillColor(sf::Color::Black);

        // Initialize level text
        levelText.setFont(font);
        levelText.setCharacterSize(15);
        levelText.setFillColor(sf::Color::Black);
        levelText.setString("Level " + std::to_string(level));
        levelText.setPosition(50, 175); // ��ġ�� ����ġ �� ���ʿ� ����
    }
    void startMiniGame1() {
        // Start Mini Game 1 (Block Breaker Game) in a separate thread
        std::thread gameThread(&Tamagotchi::runMiniGame1, this);
        gameThread.detach(); // detach the thread to allow it to run independently
    }

    void runMiniGame1() {
        BlockBreakerGame game;
        game.run();
    }

    void startMiniGame2() {
        std::thread gameThread(&Tamagotchi::runMiniGame2, this);
        gameThread.detach();
    }

    void runMiniGame2() {
        ShootingGame game;
        game.run();
    }

    void handleEvent(sf::Event& e) {
        if (e.type == sf::Event::MouseButtonPressed) {
            int x = e.mouseButton.x;
            int y = e.mouseButton.y;
            if (x >= playButton.getPosition().x && x < playButton.getPosition().x + playButton.getSize().x &&
                y >= playButton.getPosition().y && y < playButton.getPosition().y + playButton.getSize().y) {
                happiness = std::min(100, happiness + 10);
                hunger = std::max(0, hunger - 10); // ����ָ� ������� ����
                cleanliness = std::max(0, cleanliness - 2); // ��� �� û�� ����
                increaseExperience(10); // ����ġ ����
                playSound.play(); // ��ư Ŭ�� ���� ���
            }
            if (x >= feedButton.getPosition().x && x < feedButton.getPosition().x + feedButton.getSize().x &&
                y >= feedButton.getPosition().y && y < feedButton.getPosition().y + feedButton.getSize().y) {
                hunger = std::min(100, hunger + 10);
                increaseExperience(10); // ����ġ ����
                foodSound.play(); // ��ư Ŭ�� ���� ���
            }
            if (x >= cleanButton.getPosition().x && x < cleanButton.getPosition().x + cleanButton.getSize().x &&
                y >= cleanButton.getPosition().y && y < cleanButton.getPosition().y + cleanButton.getSize().y) {
                cleanliness = std::min(100, cleanliness + 10);
                happiness = std::max(0, happiness - 3);
                increaseExperience(10); // ����ġ ����
                cleanSound.play(); // ��ư Ŭ�� ���� ���
            }

            if (x >= game1Button.getPosition().x && x < game1Button.getPosition().x + game1Button.getSize().x &&
                y >= game1Button.getPosition().y && y < game1Button.getPosition().y + game1Button.getSize().y) {
                std::cout << "Entering Mini Game 1..." << std::endl;
                buttonClickSound.play(); // ��ư Ŭ�� ���� ���
                startMiniGame1(); // �̴� ���� 1 ���� �Լ� ȣ��
                // ���⿡ �̴� ���� 1�� �̵��ϴ� �ڵ� �߰�
            }



            if (x >= game2Button.getPosition().x && x < game2Button.getPosition().x + game2Button.getSize().x &&
                y >= game2Button.getPosition().y && y < game2Button.getPosition().y + game2Button.getSize().y) {
                std::cout << "Entering Mini Game 2..." << std::endl;
                buttonClickSound.play(); // ��ư Ŭ�� ���� ���
                startMiniGame2();
                // ���⿡ �̴� ���� 2�� �̵��ϴ� �ڵ� �߰�
            }
        }
    }

    void update() {
        int currentTime = sf::Clock().getElapsedTime().asMilliseconds();
        if (currentTime > lastUpdateTime + 30000) { // 30�ʸ��� ����
            hunger = std::max(0, hunger - 5);
            happiness = std::max(0, happiness - 5);
            cleanliness = std::max(0, cleanliness - 5);
            lastUpdateTime = currentTime;
        }
    }

    void render(sf::RenderWindow& window) {
        sf::Text hungerLabelText;
        hungerLabelText.setFont(font);
        hungerLabelText.setCharacterSize(15);
        hungerLabelText.setFillColor(sf::Color::Black);
        hungerLabelText.setString("Hunger");
        hungerLabelText.setPosition(50, 30);

        sf::Text happinessLabelText;
        happinessLabelText.setFont(font);
        happinessLabelText.setCharacterSize(15);
        happinessLabelText.setFillColor(sf::Color::Black);
        happinessLabelText.setString("Happiness");
        happinessLabelText.setPosition(50, 80);

        sf::Text cleanlinessLabelText;
        cleanlinessLabelText.setFont(font);
        cleanlinessLabelText.setCharacterSize(15);
        cleanlinessLabelText.setFillColor(sf::Color::Black);
        cleanlinessLabelText.setString("Cleanliness");
        cleanlinessLabelText.setPosition(50, 130);

        window.draw(hungerLabelText);
        window.draw(happinessLabelText);
        window.draw(cleanlinessLabelText);

        // Render happiness bar
        sf::RectangleShape happinessBar(sf::Vector2f(happiness * 2, 20));
        happinessBar.setPosition(50, 50);
        happinessBar.setFillColor(sf::Color::Red);
        window.draw(happinessBar);

        // Render hunger bar
        sf::RectangleShape hungerBar(sf::Vector2f(hunger * 2, 20));
        hungerBar.setPosition(50, 100);
        hungerBar.setFillColor(sf::Color::Green);
        window.draw(hungerBar);

        // Render cleanliness bar
        sf::RectangleShape cleanlinessBar(sf::Vector2f(cleanliness * 2, 20));
        cleanlinessBar.setPosition(50, 150);
        cleanlinessBar.setFillColor(sf::Color::Blue);
        window.draw(cleanlinessBar);

        // Render experience bar
        sf::RectangleShape experienceBar(sf::Vector2f(experience * 2, 20));
        experienceBar.setPosition(50, 200);
        experienceBar.setFillColor(sf::Color::Yellow);
        window.draw(experienceBar);

        // Render percentage texts
        hungerText.setString(std::to_string(hunger) + "%");
        happinessText.setString(std::to_string(happiness) + "%");
        cleanlinessText.setString(std::to_string(cleanliness) + "%");

        hungerText.setPosition(hungerBar.getPosition().x + hungerBar.getSize().x / 2 - hungerText.getGlobalBounds().width / 2, hungerBar.getPosition().y + hungerBar.getSize().y / 2 - hungerText.getGlobalBounds().height / 2);
        happinessText.setPosition(happinessBar.getPosition().x + happinessBar.getSize().x / 2 - happinessText.getGlobalBounds().width / 2, happinessBar.getPosition().y + happinessBar.getSize().y / 2 - happinessText.getGlobalBounds().height / 2);
        cleanlinessText.setPosition(cleanlinessBar.getPosition().x + cleanlinessBar.getSize().x / 2 - cleanlinessText.getGlobalBounds().width / 2, cleanlinessBar.getPosition().y + cleanlinessBar.getSize().y / 2 - cleanlinessText.getGlobalBounds().height / 2);
        // �ٸ���ġ ��������Ʈ ������
        window.draw(tamagotchiSprite);
        window.draw(hungerText);
        window.draw(happinessText);
        window.draw(cleanlinessText);

        // Render level text
        levelText.setString("Level " + std::to_string(level));
        window.draw(levelText);

        // Render tamagotchi
        tamagotchiRect.setFillColor(sf::Color::Black);
        window.draw(tamagotchiRect);

        // Render buttons
        playButton.setFillColor(sf::Color(128, 128, 128));
        feedButton.setFillColor(sf::Color(128, 128, 128));
        cleanButton.setFillColor(sf::Color(128, 128, 128));
        window.draw(playButton);
        window.draw(feedButton);
        window.draw(cleanButton);

        // Render mini game buttons
        game1Button.setFillColor(sf::Color(128, 128, 128));
        game2Button.setFillColor(sf::Color(128, 128, 128));
        window.draw(game1Button);
        window.draw(game2Button);

        // Render button texts
        window.draw(playText);
        window.draw(feedText);
        window.draw(cleanText);
        window.draw(game1Text);
        window.draw(game2Text);
    }

    bool isGameOver() {
        return (hunger == 0 || happiness == 0 || cleanliness == 0);
    }

private:
    int a;
    int level;
    int experience;
    int hunger;
    int happiness;
    int cleanliness;
    int lastUpdateTime;
    bool evolved; // �ٸ���ġ�� ��ȭ�ߴ��� ����
    sf::RectangleShape tamagotchiRect;
    sf::RectangleShape playButton;
    sf::RectangleShape feedButton;
    sf::RectangleShape cleanButton;
    sf::RectangleShape game1Button;
    sf::RectangleShape game2Button;
    sf::Font font;
    sf::Text playText;
    sf::Text feedText;
    sf::Text cleanText;
    sf::Text game1Text;
    sf::Text game2Text;
    sf::SoundBuffer buttonClickBuffer;
    sf::Sound buttonClickSound;
    sf::Texture tamagotchiTexture;
    sf::Sprite tamagotchiSprite;
    sf::SoundBuffer evolveSoundBuffer;
    sf::Sound evolveSound;
    sf::SoundBuffer levelSoundBuffer;
    sf::Sound levelSound;
    sf::SoundBuffer foodSoundBuffer;
    sf::Sound foodSound;
    sf::SoundBuffer cleanSoundBuffer;
    sf::Sound cleanSound;
    sf::SoundBuffer playSoundBuffer;
    sf::Sound playSound;
    // Percentage texts
    sf::Text hungerText;
    sf::Text happinessText;
    sf::Text cleanlinessText;

    // Level text
    sf::Text levelText;

    void increaseExperience(int amount) {
        experience += amount;
        if (experience >= 100) {
            experience -= 100;
            level++;
            std::cout << "Tamagotchi leveled up! Current level: " << level << std::endl;
            if (level == 3 || level == 5) {
                evolveSound.play(); // ���� 3 �Ǵ� 5�� �����ϸ� ��ȭ ���� ���
            }
            else {
                levelSound.play(); // ������ ������ �� ȿ���� ���
            }
        }
        if (level == 3) {
            if (!tamagotchiTexture.loadFromFile(std::to_string(a) + std::to_string(2) + ".png")) {
                std::cerr << "Failed to load tamagotchi1 image!" << std::endl;
            }
            tamagotchiSprite.setTexture(tamagotchiTexture);
        }
        if (level == 5) {
            // �� ��° ��ȭ�� ��� tamagotchi2.png�� �̹��� ����
            if (!tamagotchiTexture.loadFromFile(std::to_string(a) + std::to_string(3) + ".png")) {
                std::cerr << "Failed to load tamagotchi2 image!" << std::endl;
            }
            tamagotchiSprite.setTexture(tamagotchiTexture);
        }
    }
};

int main() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tamagotchi");
    Tamagotchi tamagotchi;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            tamagotchi.handleEvent(event);
        }

        tamagotchi.update();

        window.clear(sf::Color::White);
        tamagotchi.render(window);
        window.display();

        if (tamagotchi.isGameOver()) {
            std::cout << "Game Over!" << std::endl;
            window.close();
        }
    }

    return 0;
}