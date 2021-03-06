//
//  Background.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 11/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "Background.hpp"

namespace ns
{
    namespace NovelComponents
    {
        Background::Background(const std::wstring& folderPath) : folder(folderPath) { }
        void Background::LoadImage(const std::wstring& path)
        {
            sf::Texture* texture = ic::RequestHigherTexture(folder + path, /*novelSystem*/ic::globalRequestSender, 1);
            if ((loaded = texture))
            {
                this->path = folder + path; image = path;
                sprite.setTexture(*texture); Resize(gs::width, gs::height);
            }
            else
            {
                if (messageBack != MessageBack::No)
                    novelSystem->SendMessage({"UnHold", this});
                novelSystem->PopComponent(this);
            }
        }
        void Background::Resize(const unsigned int& width, const unsigned int& height)
        {
            CalculateScale(width, height);
            if (doParallax && !(gs::isPauseEnabled && gs::isPause))
                CalculateParallax(sf::Mouse::getPosition(*gs::window).x, sf::Mouse::getPosition(*gs::window).y);
        }
        void Background::Update(const sf::Time& elapsedTime)
        {
            switch (mode)
            {
                case Mode::Appear: gs::requestWindowRefresh = true;
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime)
                    {
                        alpha = maxAlpha; currentTime = disappearTime; mode = switchTo;
                        novelSystem->SendMessage({"Background :: Appeared", this});
                        if (messageBack == MessageBack::AtAppearance) novelSystem->SendMessage({"UnHold", this});
                    }
                    else alpha = (sf::Uint8)(maxAlpha * (currentTime / appearTime));
                    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
                    break;
                    
                case Mode::Disapper: gs::requestWindowRefresh = true;
                    if (currentTime > 0) currentTime -= elapsedTime.asSeconds();
                    if (currentTime <= 0)
                    {
                        alpha = 0; currentTime = 0.f;
                        mode = Mode::Deprecate;
                        if (messageBack == MessageBack::AtDeprecated)
                            novelSystem->SendMessage({"UnHold", this});
                    }
                    else alpha = (sf::Uint8)(maxAlpha * (currentTime / disappearTime));
                    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
                    break;

                case Mode::Deprecate: gs::requestWindowRefresh = true; novelSystem->PopComponent(this); break;
                default: break;
            }
        }
        void Background::Draw(sf::RenderWindow* window) { if (loaded && visible) window->draw(sprite); }
        void Background::Destroy() { if (path.length() != 0) ic::DeleteImage(path); novelSystem->SendMessage({"Destroy", this}); }
        void Background::PollEvent(sf::Event& event)
        {
            if (event.type == sf::Event::MouseMoved && visible && doParallax && parallaxPower > 0)
                CalculateParallax(event.mouseMove.x, event.mouseMove.y);
        }
        void Background::ReceiveMessage(MessageHolder &message)
        {
            if (nss::Command(message.info, "Request") && message.additional == path)
            {
                sf::Texture* texture = ic::LoadTexture(path);
                if (texture) { sprite.setTexture(*texture, true); CalculateScale(gs::width, gs::height);
                    if (doParallax && !(gs::isPauseEnabled && gs::isPause))
                        CalculateParallax(sf::Mouse::getPosition(*gs::window).x, sf::Mouse::getPosition(*gs::window).y);
                }
            }
            else if (message.address == hideAfter && message.info == "Background :: Appeared") { messageBack = MessageBack::No; mode = Mode::Disapper; }
        }
        void Background::CalculateParallax(int mouseX, int mouseY)
        {
            if (mouseX >= 0 && mouseY >= 0 && mouseX <= gs::width && mouseY <= gs::height)
            {
                float posX = defaultPositionX + (int)(mouseX - gs::width/2) * parallaxPower;
                float posY = defaultPositionY + (int)(mouseY - gs::height/2) * parallaxPower;
                sprite.setPosition(posX, posY);
            }
        }
        void Background::CalculateScale(unsigned int width, unsigned int height)
        {
            if (loaded)
            {
                float scaleFactorX, scaleFactorY, scaleFactor;
                scaleFactorX = (float)width / (sprite.getLocalBounds().width) * (doParallax? 1 + parallaxPower : 1) * scaleX;
                scaleFactorY = (float)height / (sprite.getLocalBounds().height) * (doParallax? 1 + parallaxPower : 1) * scaleY;
                switch (fitMode)
                {
                    case defaultFit:
                        scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
                        sprite.setScale(scaleFactor, scaleFactor);
                        break;
                    case fillCentre:
                        scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
                        sprite.setScale(scaleFactor, scaleFactor);
                        defaultPositionX = (float)ns::gs::width/2 - sprite.getLocalBounds().width/2*sprite.getScale().x - sprite.getOrigin().x*sprite.getScale().x;
                        defaultPositionY = (float)ns::gs::height/2 - sprite.getLocalBounds().height/2*sprite.getScale().y - sprite.getOrigin().y*sprite.getScale().y;
                        sprite.setPosition(defaultPositionX, defaultPositionY);
                        break;
                    case stretch: sprite.setScale(scaleFactorX, scaleFactorY); break;
                    default: break;
                }
            }
        }
        void Background::SetStateMode(const Mode& newMode)
        {
            if (mode != newMode)
            {
                if (newMode == Mode::Disapper) currentTime = disappearTime; else currentTime = 0.f; mode = newMode;
                if ((newMode == Mode::Disapper && messageBack == MessageBack::AtDisappearance) ||
                    (newMode == Mode::Deprecate && messageBack == MessageBack::AtDeprecated))
                    novelSystem->SendMessage({"UnHold", this});
            }
        }
    }
}
