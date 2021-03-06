//
//  NekoHelpers.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoHelpers_hpp
#define NekoHelpers_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/minEH.hpp>
#include "EventBase.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

using namespace ns;

namespace NekoNinja
{
    struct HeartsShape : NovelObject
    {
        sf::Sprite sprite;
        int alpha{ 255 };
        float elapsed{ 0.f };
        bool reverse;
        
        HeartsShape(float x, float y, float scale, bool reverse);
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Draw(sf::RenderWindow* window) override;
    };
    
    struct ComboText : NovelObject
    {
        sf::Text text;
        int alpha{ 255 };
        float elapsed{ 0.f };
        bool fontLoaded{ false };
        
        ComboText(int count, float x, float y);
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Draw(sf::RenderWindow* window) override;
    };
    
    struct CriticalHitText : NovelObject
    {
        sf::Text text;
        int alpha{ 255 };
        float elapsed{ 0.f };
        bool fontLoaded{ false };
        
        CriticalHitText(float x, float y);
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Draw(sf::RenderWindow* window) override;
    };
    
    struct ScratchScratch : NovelObject
    {
        sf::Text text;
        int alpha{ 255 };
        float elapsed{ 0.f };
        bool fontLoaded{ false };
        
        ScratchScratch();
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
    };
}

#endif /* NekoHelpers_hpp */
