
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

// TODO: Current game's timer


/// TODO: "Нажмите на холодильник, чтобы открыть его и посмотреть имеющиеся продукты."
///       Открывается интерфейс с холодильником, представляющий собой холодильник на заднем плане и ячейки инвентаря на переднем с едой, на которую можно тыкать, таким образом узнавая описание этой вещи. Ячейки будут занимать всё доступное пространство, а при нажатии будет открываться прямо из ячейки небольшое окошко с большим красным крестиком. Еду можно зажать и начать переносить в правую часть экрана, чтобы закрыть холодильник и начать передвигать еду в комнате. Еду можно просто так положить на пол, а можно дать кошечке, в случае чего она покушает. И так же с жаждой.

// DONE: Эмоции зависят от состояния неко, А ТАКЖЕ от активности, которая может перезаписывать элементы эмоции (брови, глаза, рот).
// DONE: Сон восстанавливает энергию В ПРОЦЕССЕ выполнения активности. Т.е. вместо ActivityTasks::Waiting нужен свой таск, который в Update будет также к needEnergy добавлять некоторые очки. И спать неко должна до тех пор, пока эта энерджи не станет полной (или её не разбудят, когда она уже в жёлтой зоне).
// TODO (?): Какой-нибудь список кулдаунов активностей. Если отказать кошечке в том, чтобы она покушала, то на 20-30 секунд она не будет пытаться кушать, занявшись чем-то другим.
// TODO: Если кошечка спит или занята, то наведение на неё предметом об этом говорит и возвращает предмет в инвентарь.
// TODO: Шторка в ванной. При тыкании по шторке приближение к ванной, где диалоговое окошечке кошечки из-за шторки может отвечать на вопросы, кнопки будут справа всякие. Можно "зайти к кошечке" в ванную, тогда открывается другой интерфейс, где будут другие кнопки итд.



#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <SFML/Main.hpp>
    #endif
#endif
#ifdef SFML_SYSTEM_ANDROID
    #include <jni.h>
    #include <android/native_activity.h>
    #include <android/log.h>
    #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sfml-activity", __VA_ARGS__))
    #include <SFML/System/NativeActivity.hpp>
#endif

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Essentials/Base.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/Initialization.hpp>
#include <minEH/Engine/Notifications.hpp>

#include <minEH/Components/Helpers/EssentialComponents.hpp>
#include <minEH/Components/Helpers/LanguageLibrary.hpp>

#include "Components/NekoUIComponents/Player.hpp"
#include "Components/NekoUIComponents/Neko/Brain.hpp"
#include "Components/NekoUIComponents/Apartment/Apartment.hpp"
#include "Components/NekoUIComponents/Misc/OffsetHelper.hpp"
#include "Components/NekoUIComponents/Interfaces/NekoInterfaceUI.hpp"
#include "Components/NekoUIComponents/Interfaces/JobInterfaceUI.hpp"
#include "Components/NekoUIComponents/Interfaces/RoomUI.hpp"
#include "Components/NekoUIComponents/Interfaces/InventoryUI.hpp"
#include "Components/NekoUIComponents/Interfaces/WardrobeUI.hpp"
#include "Components/NekoUIComponents/Interfaces/CalendarUI.hpp"
#include "Components/NekoUIComponents/Interfaces/MapUI.hpp"
#include "Components/NekoUIComponents/Interfaces/ItemDetailsUI.hpp"
#include "Components/NekoUIComponents/Places/PlacesInterfaceUI.hpp"
#include "Components/NekoUIComponents/Interfaces/FlowchartUI.hpp"

// #include "Components/VNLightComponents/Comp.hpp"

using std::cout;
using std::cin;
using std::endl;

using namespace ns;

#ifdef _MSC_VER
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
#ifdef _WIN32
    std::system("chcp 1251");
#endif
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
#if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "NekoUI", sf::Style::Default);
    gs::isParallaxEnabled = gs::buttonHovering = false;
#else
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width >= 1280 ? 1280 : sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height >= 880 ? 800 : sf::VideoMode::getDesktopMode().height - 80), "NekoUI", sf::Style::Default, settings);
#endif
    GameDocumentsPath::path = "/Melancholy Hill/NekoUI/";
    gs::window = &window;
    gs::width = window.getSize().x;
    gs::height = window.getSize().y;
    
    ExecuteInitalization();
    
    if (!gs::isVerticalSyncEnabled) window.setFramerateLimit(gs::framerateLimit);
    window.setVerticalSyncEnabled(gs::isVerticalSyncEnabled);
    // window.setFramerateLimit(120);
    gs::isParallaxEnabled = false;
    
    EntitySystem system;
    srand((unsigned int)time(nullptr));
    
    ic::globalRequestSender = &system;
    Language::Default(); lang::currentLanguage = "ru"; Languages::LoadData();
    
#pragma mark -
    ///----------------------------------------------------------
    /// \brief Entity to hold NekoUI components
    ///
    /// Entity holds components like NekoUI room, its interfaces etc.
    ///
    ///----------------------------------------------------------
    Entity* Vanilla = system.AddEntity();
    {
        /*for (int i = 0; i < 1; ++i)
        {
            Vanilla->AddComponent<Mesh>();
        }*/
        Vanilla->AddComponent<NotificationsUI>();
        
        NekoUI::Player::Init();
        Vanilla->AddComponent<NekoUI::Neko::BrainComponent>();
        Vanilla->AddComponent<NekoUI::CalendarUI>();
        Vanilla->AddComponent<NekoUI::NekoInterfaceUI>();
        Vanilla->AddComponent<NekoUI::JobInterfaceUI>();
        Vanilla->AddComponent<NekoUI::InventoryUI>();
        Vanilla->AddComponent<NekoUI::ItemDetailsUI>();
        Vanilla->AddComponent<NekoUI::WardrobeUI>();
        Vanilla->AddComponent<NekoUI::MapUI>();
        Vanilla->AddComponent<NekoUI::FlowchartUI>();
        Vanilla->AddComponent<NekoUI::PlacesInterfaceUI>();
        Vanilla->AddComponent<NekoUI::Apartment>();
        // Vanilla->AddComponent<NekoUI::OffsetHelper>(L"mouth.png", true, 1); // scale = 0.66
    }
#pragma mark -
    Entity* Lana = system.AddEntity();
    {
        Lana->AddComponent<NekoUI::RoomUI>();
    }
    // Lana->SendMessage({"NotUI :: Popup", new NotificationHolder(L"Greetings!", L"Привет! Я текст! Я очень-очень длинный текст! Невероятно длиннющий! Ого! Ня! Няняня! Кошкодевочки рулят! Я Никита и я Никита! Даааааа! Невероятные слова невероятного человека, невероято делающего всё это. СМЫСОЛ жызни.")});
    
    ///----------------------------------------------------------
    /// \brief Entity to hold essential components
    ///
    /// Entity holds components like always-on debug UI layer, system's components and other essential stuff.
    /// It also may hold components like NovelComponent that runs the novel, cuz it has to be essential component.
    ///
    ///----------------------------------------------------------
    Entity* Shimakaze = system.AddEntity();
    {
        Shimakaze->AddComponent<EssentialComponents::DebugComponent>("Update 0 build 5");
        Shimakaze->AddComponent<EssentialComponents::FadingFromBlackScreen>();
    }
    gs::lastMousePos = { sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y };
    
    bool active{ true };
    sf::Clock clock;
    sf::Time clockTime;
    float elapsedSecond{ 0 };
    
#if defined(SFML_SYSTEM_IOS)
    window.setActive();
#endif
#pragma mark -
    while (window.isOpen())
    {
        sf::Event event;
        while (active ? window.pollEvent(event) : window.waitEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed: system.PollEvent(event); window.close(); break;
                case sf::Event::GainedFocus: active = true; clock.restart(); window.setFramerateLimit(gs::framerateLimit);
#ifdef SFML_SYSTEM_ANDROID
                    window.setActive();
#endif
                    break;
                case sf::Event::LostFocus: active = false; system.PollEvent(event); window.setFramerateLimit(gs::framerateNoFocus);
#ifdef SFML_SYSTEM_ANDROID
                    window.setActive(false);
#endif
                    break;
#if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
                case sf::Event::MouseEntered: active = true; clock.restart(); window.setFramerateLimit(gs::framerateLimit);
#ifdef SFML_SYSTEM_ANDROID
                    window.setActive();
#endif
                    break;
                case sf::Event::MouseLeft: active = false; system.PollEvent(event); window.setFramerateLimit(gs::framerateNoFocus);
#ifdef SFML_SYSTEM_ANDROID
                    window.setActive(false);
#endif
                    break;
#endif
                    
                case sf::Event::MouseWheelScrolled: system.PollEvent(event); break;
                case sf::Event::TouchEnded: case sf::Event::TouchBegan:
                    gs::lastMousePos = { event.touch.x, event.touch.y };
                    system.PollEvent(event);
                    break;
                case sf::Event::MouseButtonReleased: case sf::Event::MouseButtonPressed:
                    gs::lastMousePos = { event.mouseButton.x, event.mouseButton.y };
                    system.PollEvent(event);
                    break;
                case sf::Event::TouchMoved:
                    system.PollEvent(event);
                    gs::lastMousePos = { event.touch.x, event.touch.y };
                    break;
                case sf::Event::MouseMoved:
                    system.PollEvent(event);
                    gs::lastMousePos = { event.mouseMove.x, event.mouseMove.y };
                    break;
                    
                //case sf::Event::KeyPressed: case sf::Event::KeyReleased: system.PollEvent(event); break;
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Key::D:
                        case sf::Keyboard::Key::K:
                        case sf::Keyboard::Key::S:
                        case sf::Keyboard::Key::L:
                            system.PollEvent(event); break;
                        default: break;
                    }
                    break;
                    
                    case sf::Event::KeyReleased:
#ifdef SFML_SYSTEM_WINDOWS
                         if (event.key.code == sf::Keyboard::Key::F11)
                         {
                          if ((gs::fullscreen = !gs::fullscreen)) window.create(sf::VideoMode::getFullscreenModes()[0], "NekoUI", sf::Style::Fullscreen);
                          else window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width >= 1280 ? 1280 : sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height >= 880 ? 800 : sf::VideoMode::getDesktopMode().height - 80), "NovelSome");
                          gs::width = window.getSize().x;
                          gs::height = window.getSize().y;
                          CalculateScaleRatios(gs::width, gs::height);
                          system.Resize(gs::width, gs::height);
                          window.setView(sf::View(sf::FloatRect(0, 0, gs::width, gs::height)));
                         }
#endif
                         break;
                    
                case sf::Event::Resized:
                    gs::width = event.size.width;
                    gs::height = event.size.height;
                    CalculateScaleRatios(event.size.width, event.size.height);
                    system.Resize(event.size.width, event.size.height);
                    window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    break;
                    
                default: break;
            }
            gs::requestWindowRefresh = true;
        }
        
#if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
        if (active)
        {
            clockTime = clock.restart();
            elapsedSecond += clockTime.asSeconds();
            if (elapsedSecond > gs::fixedUpdate) { system.FixedUpdate(gs::fixedUpdate); elapsedSecond = 0.f; }
            
            system.Update(clockTime);
            
            if (gs::requestWindowRefresh)
            {
                window.clear();
                system.Draw(&window);
                window.display(); //TODO: Might crash here if app is not running
                
                gs::requestWindowRefresh = false;
            }
            else sf::sleep(sf::milliseconds(20));
        }
        else sf::sleep(sf::milliseconds(100));
#else
        clockTime = clock.restart();
        elapsedSecond += clockTime.asSeconds();
        if (elapsedSecond > gs::fixedUpdate) { system.FixedUpdate(gs::fixedUpdate); elapsedSecond = 0.f; }
        
        system.Update(clockTime);
        
        if (gs::requestWindowRefresh)
        {
            window.clear();
            system.Draw(&window);
            window.display();
            
            gs::requestWindowRefresh = false;
        }
        else sf::sleep(sf::milliseconds(10));
#endif
    }
#pragma mark -
    
    system.clear();
    
    return 0;
}
