//
//  RoomLibrary.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 18/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef RoomLibrary_hpp
#define RoomLibrary_hpp

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors.hpp>

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct RoomLibrary
    {
        static float x, y, xWidth, yHeight, scale, roomScale;
        static int width, height;
        static sf::Image* mask;
        
        static bool scrolldownMenuOpened, drawDatePanel, drawScrolldownMenu, drawNeeds, requestCloseButton;
        static bool canPressScrolldownMenu, canPressDatePanel, canOpenNekoUI;
        static bool allowDTSaving;
        
        enum class Location{ Unknown, Apartment, Grocery, Shopkeeper, Park, TrainStation, NekoScience, Job }; // Больница, Аптека, Рыбалка, Лес, СалонКрасоты, МагазинОдежды, Рынок, Ресторан, ТЦ, Кино, Театр
        static Location location;
        
        static sf::FloatRect scrolldownMenuBounds;
        
        static bool Collision(float x, float y);
    };
    
    std::wstring GetMonthStringFromItsNumber(const int& num);
    
    typedef RoomLibrary Room;
    typedef RoomLibrary rm;
}

#endif /* RoomLibrary_hpp */
