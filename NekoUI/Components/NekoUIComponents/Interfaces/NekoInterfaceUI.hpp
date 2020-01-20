//
//  NekoInterfaceUI.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoInterfaceUI_hpp
#define NekoInterfaceUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors/Font.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/GUI/Button/SomeButtons.hpp"

#include "../Player.hpp"
#include "../Apartment/NekoEntity.hpp"
#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    struct NekoInterfaceUI : Component
    {
        bool active{ false };
        
        sf::RectangleShape blackScreenShape;
        NekoEntity* neko{ nullptr };
        bool nekoIsSleeping{ false };
        
        bool nintDontDrawPersonNeko{ false };
        sf::Text nameText, affectionStatusText;
        sf::Sprite heartSprite;
        GUI::TextButton talkButton, requestButton, engageButton, actionButton;
        
        enum class Screen { main, talk, talkAboutNeko, talkAboutYourself, request, engage, action } screen{ Screen::main };
        
        sf::RectangleShape choiceBackShape;
        GUI::TextButton talkNekoButton, talkSelfButton, talkInterestButton, talkSkillButton;
        GUI::SpriteButton talkChooseCircle;
        GUI::TextButtons choiceButtons;
        
        bool drawSpeechBubble{ true };
        float elapsedSpeechBubble{ 0.f };
        std::wstring speechBubbleLine{ L"..." };
        sf::Sprite speechBubbleSprite;
        sf::Text speechBubbleText;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void OpenNekoInterface(NekoEntity* entity);
        void UpdateAlpha();
    };
}

#endif /* NekoInterfaceUI_hpp */
