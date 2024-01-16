#include <SFML/Graphics.hpp>
#include <functional> // For std::function
#include <iostream>

// Button class to handle button properties and actions
class Button
{
  public:
    Button() = default;
    Button(const sf::Font &font, const std::string &text, const sf::Vector2f &position, const sf::Vector2f &size)
        : font_(font), text_(text), position_(position), size_(size)
    {
        initialize();
    }

    void render(sf::RenderTarget &target)
    {
        target.draw(buttonBackground_);
        target.draw(buttonText_);
    }

    void onClick(std::function<void()> action)
    {
        onClickAction_ = std::move(action);
    }

    bool contains(const sf::Vector2f &point) const
    {
        return buttonBackground_.getGlobalBounds().contains(point);
    }

    void handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x),
                                       static_cast<float>(event.mouseButton.y));
            if (contains(mousePosition) && onClickAction_)
            {
                onClickAction_();
            }
        }
    }

  private:
    void initialize()
    {
        buttonBackground_.setSize(size_);
        buttonBackground_.setFillColor(sf::Color::White);
        buttonBackground_.setPosition(position_);

        buttonText_.setFont(font_);
        buttonText_.setString(text_);
        buttonText_.setCharacterSize(24);
        buttonText_.setFillColor(sf::Color::Blue);
        sf::FloatRect textBounds = buttonText_.getLocalBounds();
        buttonText_.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        buttonText_.setPosition(position_ + size_ / 2.0f);
    }

    sf::RectangleShape buttonBackground_;
    sf::Text buttonText_;
    sf::Font font_;
    std::string text_;
    sf::Vector2f position_;
    sf::Vector2f size_;
    std::function<void()> onClickAction_;
};

// Menu class to handle a collection of buttons
class Menu
{
  public:
    Menu() = default;
    Menu(sf::RenderWindow &window) : window_(window)
    {
        // Other initialization logic
    }

    void addButton(Button button)
    {
        buttons_.push_back(std::move(button));
    }

    void render()
    {
        for (auto &button : buttons_)
        {
            button.render(window_);
        }
    }

    void handleEvent(const sf::Event &event)
    {
        for (auto &button : buttons_)
        {
            button.handleEvent(event);
        }
    }

  private:
    sf::RenderWindow &window_;
    std::vector<Button> buttons_;
};

class ModalWindow
{
  public:
    ModalWindow(sf::RenderWindow &window, const sf::Font &font)
        : window_(window), font_(font), isModalVisible_(false), result_(false)
    {
        initialize();
    }

    void show(const std::string &message)
    {
        isModalVisible_ = true;
        modalText_.setString(message);
        centerText();
    }

    bool getResult() const
    {
        return result_;
    }

    void handleEvent(const sf::Event &event)
    {
        if (!isModalVisible_)
            return;

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x),
                                       static_cast<float>(event.mouseButton.y));

            if (quitButton_.contains(mousePosition))
            {
                result_ = true;
                isModalVisible_ = false;
            }
            else if (cancelButton_.contains(mousePosition))
            {
                result_ = false;
                isModalVisible_ = false;
            }
        }
    }

    bool isModalVisible() const
    {
        return isModalVisible_;
    }

    void render()
    {
        if (!isModalVisible_)
            return;

        window_.draw(modalBackground_);
        window_.draw(modalText_);
        quitButton_.render(window_);
        cancelButton_.render(window_);
    }

  private:
    void initialize()
    {
        modalBackground_.setSize(sf::Vector2f(400.f, 200.f));
        modalBackground_.setFillColor(sf::Color(0, 0, 0, 200));
        centerModalBackground();

        modalText_.setFont(font_);
        modalText_.setCharacterSize(24);
        modalText_.setFillColor(sf::Color::White);

        quitButton_ = createButton("Quit", sf::Vector2f(150.f, 120.f), sf::Vector2f(100.f, 40.f));
        cancelButton_ = createButton("Cancel", sf::Vector2f(250.f, 120.f), sf::Vector2f(100.f, 40.f));
    }

    void centerModalBackground()
    {
        sf::Vector2u windowSize = window_.getSize();
        modalBackground_.setPosition((windowSize.x - modalBackground_.getSize().x) / 2.f,
                                     (windowSize.y - modalBackground_.getSize().y) / 2.f);
    }

    void centerText()
    {
        sf::FloatRect textBounds = modalText_.getLocalBounds();
        modalText_.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
        sf::Vector2u windowSize = window_.getSize();
        modalText_.setPosition(windowSize.x / 2.f, (windowSize.y - modalBackground_.getSize().y) / 2.f + 20.f);
    }

    Button createButton(const std::string &text, const sf::Vector2f &position, const sf::Vector2f &size)
    {
        Button button(font_, text, position, size);
        button.onClick([text]() { std::cout << "Button clicked: " << text << std::endl; });
        return button;
    }

    sf::RenderWindow &window_;
    sf::Font font_;
    sf::RectangleShape modalBackground_;
    sf::Text modalText_;
    Button quitButton_;
    Button cancelButton_;
    bool isModalVisible_;
    bool result_;
};
