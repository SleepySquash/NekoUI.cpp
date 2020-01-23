//
//  GUIInterface.hpp
//  AlternativeTransport-MLaToA2018
//
//  Created by Никита Исаенко on 22/11/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "SomeButtons.hpp"

namespace ns
{
    namespace GUI
    {
        TextButton::TextButton()
        {
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
        }
        void TextButton::Draw(sf::RenderTarget* window) { if (loaded && visible) window->draw(text); }
        void TextButton::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
            }
            else
            {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
            if (gs::width < text.getGlobalBounds().width)
                text.setScale((float)gs::width/(text.getGlobalBounds().width), 1);
        }
        bool TextButton::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (wasPressed || ignoreWasPressed)
                {
                    if (!noColorChanging) {
                        text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
                        text.setOutlineColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, alpha)); }
                    bool constains = text.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
                    
                    wasPressed = false;
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                wasPressed = text.getGlobalBounds().contains(dot.x, dot.y);
                if (onPress) return wasPressed;
                if (wasPressed && !noColorChanging) {
                    text.setFillColor(sf::Color(pressColor.r, pressColor.g, pressColor.b, alpha));
                    text.setOutlineColor(sf::Color(opressColor.r, opressColor.g, opressColor.b, alpha)); }
            }
            else if (gs::buttonHovering && event.type == sf::Event::MouseMoved && !wasPressed && !noColorChanging)
            {
                if (text.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    if (!wasHovered) { text.setFillColor(sf::Color(hoverColor.r, hoverColor.g, hoverColor.b, alpha));
                        text.setOutlineColor(sf::Color(ohoverColor.r, ohoverColor.g, ohoverColor.b, alpha)); wasHovered = true; } }
                else if (wasHovered) { text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
                    text.setOutlineColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, alpha)); wasHovered = false; }
            }
            return false;
        }
        void TextButton::setAlpha(const sf::Uint8& alpha)
        {
            this->alpha = sf::Uint8(alpha * ((float)maxAlpha/255));
            text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, this->alpha));
            text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, this->alpha));
        }
        sf::Uint8 TextButton::getAlpha() { return text.getOutlineColor().a; }
        void TextButton::setPosition(float x, float y)
        {
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); break;
                case Halign::Center: text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y); break;
                case Halign::Right: text.setOrigin(text.getLocalBounds().width, text.getOrigin().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); break;
                case Valign::Center: text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom: text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
            text.setPosition(x, y);
        }
        void TextButton::setFont(const std::wstring& fontname)
        {
            if ((loaded = fc::GetFont(fontname)))
                text.setFont(*fc::GetFont(fontname));
        }
        void TextButton::setString(const std::wstring& string) { text.setString(string); }
        void TextButton::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
        void TextButton::setColor(const sf::Color& fillColour) { normalColor = fillColour; text.setFillColor(sf::Color(fillColour.r, fillColour.g, fillColour.b, text.getFillColor().a)); }
        void TextButton::updateColor() { text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, text.getFillColor().a)); text.setOutlineColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, text.getFillColor().a)); }
        
        
        
        TextButtons::TextButtons()
        {
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
        }
        void TextButtons::Draw(sf::RenderTarget* window)
        {
            if (loaded && visible)
            {
                if (!noColorChanging)
                {
                    if (anyButtonPressed && index == pressedIndex) {
                        text.setFillColor(sf::Color(pressColor.r, pressColor.g, pressColor.b, alpha));
                        text.setOutlineColor(sf::Color(opressColor.r, opressColor.g, opressColor.b, alpha)); }
                    else if (anyButtonHovered && index == pressedIndex) {
                        text.setFillColor(sf::Color(hoverColor.r, hoverColor.g, hoverColor.b, alpha));
                        text.setOutlineColor(sf::Color(ohoverColor.r, ohoverColor.g, ohoverColor.b, alpha)); }
                }
                window->draw(text);
                if (!noColorChanging && (anyButtonPressed || anyButtonHovered) && index == pressedIndex) {
                    text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
                    text.setOutlineColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, alpha)); }
                text.setScale(1, 1);
            }
        }
        void TextButtons::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
            }
            else
            {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
            if (gs::width < text.getGlobalBounds().width)
                text.setScale((float)gs::width/(text.getGlobalBounds().width), 1);
        }
        bool TextButtons::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (pressedIndex == index && (anyButtonPressed || ignoreWasPressed))
                {
                    bool constains = text.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) { anyButtonPressed = anyButtonHovered = false; event = sf::Event(); }
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                bool wasPressed = text.getGlobalBounds().contains(dot.x, dot.y);
                if (wasPressed) { pressedIndex = index; anyButtonPressed = true; }
                if (onPress) return wasPressed;
            }
            else if (gs::buttonHovering && event.type == sf::Event::MouseMoved && !anyButtonPressed && !noColorChanging && text.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) { anyButtonHovered = true; pressedIndex = index; }
            return false;
        }
        void TextButtons::eventPolled(sf::Event& event) { if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) anyButtonPressed = anyButtonHovered = false; else if (event.type == sf::Event::MouseMoved) anyButtonHovered = false; }
        void TextButtons::setAlpha(const sf::Uint8& alpha)
        {
            this->alpha = sf::Uint8(alpha * ((float)maxAlpha/255));
            text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, this->alpha));
            text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, this->alpha));
        }
        sf::Uint8 TextButtons::getAlpha() { return text.getOutlineColor().a; }
        void TextButtons::setPosition(float x, float y)
        {
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); break;
                case Halign::Center: text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y); break;
                case Halign::Right: text.setOrigin(text.getLocalBounds().width, text.getOrigin().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); break;
                case Valign::Center: text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom: text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
            text.setPosition(x, y);
        }
        void TextButtons::setFont(const std::wstring& fontname)
        {
            if ((loaded = fc::GetFont(fontname)))
                text.setFont(*fc::GetFont(fontname));
        }
        void TextButtons::setString(const std::wstring& string) { text.setString(string); }
        void TextButtons::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
        void TextButtons::setColor(const sf::Color& fillColour) { normalColor = fillColour; text.setFillColor(sf::Color(fillColour.r, fillColour.g, fillColour.b, text.getFillColor().a)); }
        void TextButtons::updateColor() { text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, text.getFillColor().a)); text.setOutlineColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, text.getFillColor().a)); }
        
        
        
        
        SpriteButton::~SpriteButton() { if (textureName != L"") ic::DeleteImage(textureName); }
        void SpriteButton::Draw(sf::RenderTarget* window) { if (loaded && visible) window->draw(sprite); }
        void SpriteButton::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale) sprite.setScale(scale * gs::scScale, scale * gs::scScale);
            else sprite.setScale(scale * gs::scale, scale * gs::scale);
            if (invertXAxes) sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
        }
        bool SpriteButton::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (wasPressed || ignoreWasPressed)
                {
                    if (!noColorChanging) sprite.setColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
                    bool constains = sprite.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
                    
                    wasPressed = false;
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                wasPressed = sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (onPress) return wasPressed;
                if (wasPressed && !noColorChanging) sprite.setColor(sf::Color(pressColor.r, pressColor.g, pressColor.b, alpha));
            }
            else if (gs::buttonHovering && event.type == sf::Event::MouseMoved && !wasPressed && !noColorChanging)
            {
                if (sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    if (!wasHovered) { sprite.setColor(sf::Color(hoverColor.r, hoverColor.g, hoverColor.b, alpha)); wasHovered = true; } }
                else if (wasHovered) { sprite.setColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha)); wasHovered = false; }
            }
            return false;
        }
        void SpriteButton::ReceiveMessage(MessageHolder &message)
        {
            if (nss::Command(message.info, "Request") && message.additional == textureName)
            {
                sf::Texture* texture = ic::LoadTexture(textureName);
                if ((loaded = texture)) sprite.setTexture(*texture, true);
            }
        }
        void SpriteButton::setAlpha(const sf::Uint8& alpha)
        {
            this->alpha = sf::Uint8(alpha * ((float)maxAlpha/255));
            sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, this->alpha));
        }
        sf::Uint8 SpriteButton::getAlpha() { return sprite.getColor().a; }
        void SpriteButton::setPosition(float x, float y)
        {
            switch (halign)
            {
                case Halign::Left: sprite.setOrigin(0, sprite.getOrigin().y); break;
                case Halign::Center: sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getOrigin().y); break;
                case Halign::Right: sprite.setOrigin(sprite.getLocalBounds().width, sprite.getOrigin().y); break;
            }
            switch (valign)
            {
                case Valign::Top: sprite.setOrigin(sprite.getOrigin().x, 0); break;
                case Valign::Center: sprite.setOrigin(sprite.getOrigin().x, sprite.getLocalBounds().height/2); break;
                case Valign::Bottom: sprite.setOrigin(sprite.getOrigin().x, sprite.getLocalBounds().height); break;
            }
            sprite.setPosition(x, y);
        }
        void SpriteButton::setTexture(const std::wstring& imagePath, MessageSender* sender)
        {
            if (textureName.length() != 0 && textureName != L"") ic::DeleteImage(textureName);
            textureName = imagePath; loaded = false; sf::Texture* texture;
            if (sender) texture = ic::RequestHigherTexture(imagePath, sender); else texture = ic::LoadTexture(imagePath);
            if ((loaded = texture)) sprite.setTexture(*texture, true);
        }
        void SpriteButton::setTexture(sf::Texture* texture) { if ((loaded = texture)) sprite.setTexture(*texture, true); }
        void SpriteButton::setScale(const float& scl) { scale = scl; Resize(gs::width, gs::height); }
        void SpriteButton::setColor(const sf::Color& fillColour) { normalColor = fillColour; sprite.setColor(sf::Color(fillColour.r, fillColour.g, fillColour.b, sprite.getColor().a)); }
        
        
        
        
        
        SpriteButtons::~SpriteButtons() { if (textureName != L"") ic::DeleteImage(textureName); }
        void SpriteButtons::Draw(sf::RenderTarget* window)
        {
            if (loaded && visible)
            {
                if (anyButtonPressed && index == pressedIndex) sprite.setColor(sf::Color::Yellow);
                window->draw(sprite);
                if (anyButtonPressed && index == pressedIndex) sprite.setColor(sf::Color::White);
            }
        }
        void SpriteButtons::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale) sprite.setScale(scale * gs::scScale, scale * gs::scScale);
            else sprite.setScale(scale * gs::scale, scale * gs::scale);
        }
        bool SpriteButtons::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (pressedIndex == index && (anyButtonPressed || ignoreWasPressed))
                {
                    sprite.setColor(sf::Color::White);
                    bool constains = sprite.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) { pressedIndex = -1; anyButtonPressed = false; event = sf::Event(); }
                    
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                bool wasPressed = sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (wasPressed) { pressedIndex = index; anyButtonPressed = true; }
            }
            return false;
        }
        void SpriteButtons::eventPolled(sf::Event& event) { if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) anyButtonPressed = false; }
        void SpriteButtons::setAlpha(const sf::Uint8& alpha)
        {
            if (loaded)
            {
                unsigned char realAlpha = sf::Uint8((unsigned char)alpha * ((float)maxAlpha/255));
                sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, realAlpha));
            }
        }
        void SpriteButtons::setPosition(float x, float y)
        {
            switch (halign)
            {
                case Halign::Left: sprite.setOrigin(0, sprite.getOrigin().y); break;
                case Halign::Center: sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getOrigin().y); break;
                case Halign::Right: sprite.setOrigin(sprite.getLocalBounds().width, sprite.getOrigin().y); break;
            }
            switch (valign)
            {
                case Valign::Top: sprite.setOrigin(sprite.getOrigin().x, 0); break;
                case Valign::Center: sprite.setOrigin(sprite.getOrigin().x, sprite.getLocalBounds().height/2); break;
                case Valign::Bottom: sprite.setOrigin(sprite.getOrigin().x, sprite.getLocalBounds().height); break;
            }
            sprite.setPosition(x, y);
        }
        void SpriteButtons::setTexture(const std::wstring& texture) { textureName = texture; setTexture(ic::LoadTexture(texture)); }
        void SpriteButtons::setTexture(sf::Texture* texture) { if ((loaded = texture)) sprite.setTexture(*texture, true); }
        void SpriteButtons::setScale(const float& scl) { scale = scl; Resize(gs::width, gs::height); }
        
        
        
        
        
        
        
        RectangleButton::RectangleButton()
        {
            text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
            shape.setFillColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, ralpha));
        }
        void RectangleButton::Draw(sf::RenderTarget* window) { if (loaded && visible) { window->draw(shape); window->draw(text); } }
        void RectangleButton::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
            } else {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
            if (shape.getSize().x < text.getGlobalBounds().width)
                text.setScale(shape.getSize().x/(text.getGlobalBounds().width), 1);
        }
        bool RectangleButton::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (wasPressed || ignoreWasPressed)
                {
                    text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
                    shape.setFillColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, ralpha));
                    bool constains = shape.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
                    
                    wasPressed = false;
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                wasPressed = shape.getGlobalBounds().contains(dot.x, dot.y);
                if (onPress) return wasPressed;
                if (wasPressed) { text.setFillColor(sf::Color(pressColor.r, pressColor.g, pressColor.b, alpha));
                                  shape.setFillColor(sf::Color(opressColor.r, opressColor.g, opressColor.b, ralpha)); }
            }
            else if (gs::buttonHovering && event.type == sf::Event::MouseMoved && !wasPressed && !noColorChanging)
            {
                if (shape.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    if (!wasHovered) { text.setFillColor(sf::Color(hoverColor.r, hoverColor.g, hoverColor.b, alpha));
                                       shape.setFillColor(sf::Color(ohoverColor.r, ohoverColor.g, ohoverColor.b, ralpha)); wasHovered = true; } }
                else if (wasHovered) { text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, alpha));
                                       shape.setFillColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, ralpha)); wasHovered = false; }
            }
            return false;
        }
        void RectangleButton::setAlpha(const sf::Uint8& alpha)
        {
            if (loaded)
            {
                this->alpha = ralpha = sf::Uint8((unsigned char)alpha * ((float)maxAlpha/255));
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, ralpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, ralpha));
                shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, ralpha));
                shape.setOutlineColor(sf::Color(shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, ralpha));
            }
        }void RectangleButton::setAlphaEx(const sf::Uint8 &alpha, const sf::Uint8 &ralpha)
        {
            if (loaded)
            {
                this->alpha = sf::Uint8((unsigned char)alpha * ((float)maxAlpha/255));
                this->ralpha = sf::Uint8((unsigned char)ralpha * ((float)maxAlpha/255));
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, this->alpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, this->alpha));
                shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, this->ralpha));
                shape.setOutlineColor(sf::Color(shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, this->ralpha));
            }
        }
        void RectangleButton::setPosition(float x, float y)
        {
            shape.setPosition(x, y);
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); text.setPosition(x, text.getPosition().y); break;
                case Halign::Center:
                    text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x/2, text.getPosition().y); break;
                case Halign::Right:
                    text.setOrigin(text.getLocalBounds().width, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x, text.getPosition().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); text.setPosition(text.getPosition().x, y); break;
                case Valign::Center:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y/2);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
        }
        void RectangleButton::setSize(const sf::Vector2f& vec) { shape.setSize(vec); }
        void RectangleButton::setFont(const std::wstring& fontname)
        {
            if ((loaded = fc::GetFont(fontname)))
                text.setFont(*fc::GetFont(fontname));
        }
        void RectangleButton::setString(const std::wstring& string) { text.setString(string); }
        void RectangleButton::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
    void RectangleButton::setColor(const sf::Color& fillColour) { normalColor = fillColour; text.setFillColor(sf::Color(fillColour.r, fillColour.g, fillColour.b, text.getFillColor().a)); }
    void RectangleButton::updateColor() { text.setFillColor(sf::Color(normalColor.r, normalColor.g, normalColor.b, text.getFillColor().a)); shape.setFillColor(sf::Color(onormalColor.r, onormalColor.g, onormalColor.b, text.getFillColor().a)); }
        
        
        
        
        
        
        RectangleButtons::RectangleButtons()
        {
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            shape.setFillColor(shapeFillColor);
            shape.setOutlineColor(sf::Color::White);
        }
        void RectangleButtons::Draw(sf::RenderTarget* window)
        {
            if (loaded && visible)
            {
                if (shape.getSize().x < text.getGlobalBounds().width) { text.setScale(shape.getSize().x/(text.getGlobalBounds().width), 1); }
                if (anyButtonPressed && index == pressedIndex) {
                    shape.setFillColor(sf::Color::Yellow); text.setFillColor(sf::Color::Yellow); }
                window->draw(shape); window->draw(text);
                if (anyButtonPressed && index == pressedIndex) {
                    shape.setFillColor(shapeFillColor); text.setFillColor(sf::Color::White); }
                text.setScale(1, 1);
            }
        }
        void RectangleButtons::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
                shape.setOutlineThickness(thickness * gs::scScale);
            } else {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
                shape.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
        }
        bool RectangleButtons::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (pressedIndex == index && (anyButtonPressed || ignoreWasPressed))
                {
                    text.setFillColor(sf::Color::White); shape.setFillColor(shapeFillColor);
                    bool constains = shape.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) { pressedIndex = -1; anyButtonPressed = false; event = sf::Event(); }
                    
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                bool wasPressed = shape.getGlobalBounds().contains(dot.x, dot.y);
                if (wasPressed) { pressedIndex = index; anyButtonPressed = true; }
            }
            return false;
        }
        void RectangleButtons::eventPolled(sf::Event& event) { if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) anyButtonPressed = false; }
        void RectangleButtons::setAlpha(const sf::Uint8& alpha)
        {
            if (loaded)
            {
                unsigned char realAlpha = sf::Uint8((unsigned char)alpha * ((float)maxAlpha/255));
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, realAlpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, realAlpha));
                shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, realAlpha));
                shape.setOutlineColor(sf::Color(shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, realAlpha));
            }
        }
        void RectangleButtons::setPosition(float x, float y)
        {
            shape.setPosition(x, y);
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); text.setPosition(x, text.getPosition().y); break;
                case Halign::Center:
                    text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x/2, text.getPosition().y); break;
                case Halign::Right:
                    text.setOrigin(text.getLocalBounds().width, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x, text.getPosition().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); text.setPosition(text.getPosition().x, y); break;
                case Valign::Center:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y/2);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
        }
        void RectangleButtons::setSize(const sf::Vector2f& vec) { shape.setSize(vec); }
        void RectangleButtons::setFont(const std::wstring& fontname)
        {
            if ((loaded = fc::GetFont(fontname)))
                text.setFont(*fc::GetFont(fontname));
        }
        void RectangleButtons::setString(const std::wstring& string) { text.setString(string); }
        void RectangleButtons::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
    }
}