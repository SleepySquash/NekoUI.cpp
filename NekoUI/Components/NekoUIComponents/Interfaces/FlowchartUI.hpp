//
//  FlowchartUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 16.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef FlowchartUI_hpp
#define FlowchartUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors/Font.hpp>
#include <minEH/Engine/Collectors/Image.hpp>
#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>
#include <minEH/Engine/Language.hpp>

#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::lang;

namespace NekoUI
{
    struct FlowchartUI : Component
    {
        bool active{ false };
        
        sf::Sprite background; bool loaded{ false };
        GUI::TextButton skillsButton, interestsButton, biographyButton;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        enum class Screen { Skill, Interest, Biography } screen{ Screen::Skill };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
        void ReceiveMessage(MessageHolder& message) override;
        void SwitchScreen(const Screen& scr);
        void Switch(const bool& on);
        void UpdateAlpha();
    };
}

#endif /* FlowchartUI_hpp */
