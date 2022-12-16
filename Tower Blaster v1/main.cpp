#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>


using namespace sf;

RenderWindow window(VideoMode(1200, 800), "Tower Blaster", Style::Close | Style::Titlebar);
Font font;
Vector2i mousePos;

String playerInput;
Text playerText("",font,20);
int gamestate;
Image icon;
Texture title;
Texture background; //will change to a rolling one
bool rando;

int score, better, oldbetter, j;

int StringToInt (String a) //utility function to convert strings that have numbers in them to int
{
    std::string a1 = a;
    if(a1!="NULL"){
    int x = 0;
    for(int i=0; i<a1.length(); i++)
    {
        x = x*10+(a1[i]-'0');
    }
    return x;
    }
    else return 0;
}

class Button
{
    public:
        int x, y, wid, hig, butToGS;
        bool hoverOK, fhover;
        String txt;
        int ret = 0;

    void init(int xin, int yin, int widin, int higin, String txtin, int BGS, bool hov)
    {
        x = xin-widin/2;
        y = yin-higin/2;
        wid = widin;
        hig = higin;
        txt = txtin;
        butToGS = BGS;
        hoverOK = hov;
    }
    void bd()
    {
        RectangleShape but(Vector2f(wid, hig));
        Text butDis("", font, 20);
        butDis.setString(txt);
        butDis.setPosition(x+wid/2-butDis.getGlobalBounds().width/2, y+hig/2-butDis.getGlobalBounds().height/1.2);
        but.setPosition(x, y);
        but.setFillColor(Color(0,0,0));
        but.setOutlineThickness(2);
        but.setOutlineColor(Color(255,255,255));
        window.draw(but);
        if(mousePos.x>but.getGlobalBounds().left &&
           mousePos.x<but.getGlobalBounds().left+but.getGlobalBounds().width &&
           mousePos.y>but.getGlobalBounds().top &&
           mousePos.y<but.getGlobalBounds().top+but.getGlobalBounds().height && hoverOK)
        {
            RectangleShape hover(Vector2f(wid-10, hig-10));
            hover.setPosition(x+5, y+5);
            hover.setFillColor(Color(0,0,0));
            hover.setOutlineThickness(1);
            hover.setOutlineColor(Color(255,255,255));
            window.draw(hover);
            if(Mouse::isButtonPressed(Mouse::Left)) gamestate = butToGS;
        }
        window.draw(butDis);
    }
    void forcedHover()
        {
        RectangleShape hover(Vector2f(wid-10, hig-10));
        hover.setPosition(x+5, y+5);
        hover.setFillColor(Color(0,0,0));
        hover.setOutlineThickness(1);
        hover.setOutlineColor(Color(255,255,255));
        window.draw(hover);
        Text butDis("", font, 20);
        butDis.setString(txt);
        butDis.setPosition(x+wid/2-butDis.getGlobalBounds().width/2, y+hig/2-butDis.getGlobalBounds().height/1.2);
        window.draw(butDis);
        }
};

Button meTower[11], enTower[11], prev, mys;
int occup[51] = {0};
bool urTurn = true, choose = true;
Button winScreen;
Button loseScreen;

int Check(bool who) //checks whether the vector given is in order or not; 0 for enemy, 1 for you
{
    /*if(who) //score part, might add later
        for(int i=2; i<=10; i++)
        {
            if(StringToInt(meTower[i].txt)==StringToInt(meTower[i-1].txt)+1) score = StringToInt(meTower[i].txt) + StringToInt(meTower[i-1].txt);
        }*/
    if(who)
        for(int i=2; i<=10; i++)
        {
            if(StringToInt(meTower[i].txt)<StringToInt(meTower[i-1].txt)) return 0;
        }
    else
        for(int i=2; i<=10; i++)
        {
            if(StringToInt(enTower[i].txt)<StringToInt(enTower[i-1].txt)) return 0;
        }

    return 1;
}

void init()
{
    gamestate = 1;
    title.loadFromFile("title.png");
    background.loadFromFile("BG.png");
    icon.loadFromFile("icon.png");
    window.setIcon(32, 32, icon.getPixelsPtr());
    font.loadFromFile("font\\LiberationSans-Regular.ttf");
    window.setFramerateLimit(60);
}

void textIO(Event evnt)
{
    if(evnt.text.unicode == '\b'&& playerInput.getSize()>=1) {playerInput.erase(playerInput.getSize()-1, 1);}
    else {playerInput +=evnt.text.unicode;}
    playerText.setString(playerInput);
}
void backgroundDraw()
{
    RectangleShape backgroundScene(Vector2f(1200.0f, 800.0f));
    backgroundScene.setTexture(&background);
    backgroundScene.setPosition(0, 0);
    window.draw(backgroundScene);
}
void menu(Event evnt)
{
    RectangleShape titleScene(Vector2f(500.0f, 200.0f));
    titleScene.setTexture(&title);
    titleScene.setPosition(350, 100);
    window.draw(titleScene);

    Button play;
    Button rules;
    Button exit;

    play.init(window.getSize().x/2, window.getSize().y/2, 150, 50, "Play", 3, true);
    rules.init(window.getSize().x/2, window.getSize().y/2+70, 150, 50, "Rules", 2, true);
    exit.init(window.getSize().x/2, window.getSize().y/2+200, 100, 40, "Exit", 9, true);
    play.bd();
    rules.bd();
    exit.bd();
}

void rules(Event evnt)
{
    Text text("", font, 20);
    text.setPosition(Vector2f(150, 50));
    text.setString("How to play:\n\n\nWhen the game starts, you'll see two towers, yours and your enemy's.\n\nThe towers are in a certain order. Your job is to make every floor have a value smaller\nthan the one under it\n\nYou will achieve that by choosing between the floors presented to you in the middle of the\nscreen. You have two choices: the one on the left, discarded by the enemy, and the one on\nthe right which is a \"mystery\" block.\n\nWhichever of you finishes putting the tower in order wins. Good luck!");
    window.draw(text);

    Button menu;
    menu.init(200, window.getSize().y/2+250, 100, 40, "Menu", 1, true);
    menu.bd();
}

void gamemenu(Event evnt)
{
    RectangleShape titleScene(Vector2f(500.0f, 200.0f));
    titleScene.setTexture(&title);
    titleScene.setPosition(350, 100);
    window.draw(titleScene);
    Button multi;
    Button single;
    Button menu;
    menu.init(window.getSize().x/2, window.getSize().y/2+120, 100, 40, "Menu", 1, true);
    multi.init(window.getSize().x/2-185, window.getSize().y/2, 220, 66, "Multiplayer", 3, true);
    single.init(window.getSize().x/2+185, window.getSize().y/2, 220, 66, "Singleplayer", 4, true);
    single.bd();
    multi.bd();
    menu.bd();
}

void initSP(Event evnt)
{
    urTurn = true; choose = true;
    score = 0; better = 0; oldbetter = 0;
    for(int i=1; i<=50; i++) occup[i] = 0;
    for(int i=1, p; i<=10; i++)
    {
        do{
            p = rand()%50+1;
        }while(occup[p]==1);
        occup[p]=1;
        meTower[i].init(850, 100+50*i, 50+p*3, 40, std::to_string(p), 9+i, false);
        do{
            p = rand()%50+1;
        }while(occup[p]==1);
        occup[p]=1;
        enTower[i].init(350, 100+50*i, 50+p*3, 40, std::to_string(p), 19+i, false);
    }
    int p;
    do{
        p = rand()%50+1;
    }while(occup[p]==1);
    occup[p]=1;
    prev.init(600, 300, 70, 40, std::to_string(p), 51, false);
    do{
        p = rand()%50+1;
    }while(occup[p]==1);
    occup[p]=1;
    mys.init(600, 350, 70, 40, std::to_string(p), 52, false);
    gamestate = 5;

    winScreen.init(600, 350, 500, 150, "You won", 5, false);
    loseScreen.init(600, 350, 500, 150, "You lose", 5, false);
}

void singleplayer(Event evnt)
{
    RectangleShape markbox(Vector2f(mys.wid*0.8, mys.hig*0.7));
    Text mark("?", font, 20);
    mark.setPosition(mys.x+mys.wid/2-mark.getGlobalBounds().width/2, mys.y+mys.hig/2-mark.getGlobalBounds().height/1.2);
    markbox.setPosition(mys.x+5, mys.y+5);
    markbox.setFillColor(Color(0,0,0));

    for(int i=1; i<=10; i++)
    {
        meTower[i].bd();
        enTower[i].bd();
        prev.bd();
        mys.bd();
    }
    //for(int i=1; i<=50; i++) if(occup[i]) std::cout<< i << " "; else std::cout << '-' << " ";
    //std::cout<<std::endl;
    CircleShape indicator(10);
    int p;
    if(urTurn)
    {
        indicator.setPosition(840, 50);
        window.draw(indicator);
        if(choose)
        {
            prev.hoverOK=true;
            mys.hoverOK=true;
            window.draw(markbox);
            window.draw(mark);
            if(Mouse::isButtonPressed(Mouse::Left)&&gamestate==51)
            {
                choose = false; gamestate = 5; prev.hoverOK=false; mys.hoverOK=false;
                rando = false;
            }
            if(Mouse::isButtonPressed(Mouse::Left)&&gamestate==52)
            {
                rando = true;
                choose = false; gamestate = 5; prev.hoverOK=false; mys.hoverOK=false;
            }
        }
        else
        {

            for(int i=1; i<=10; i++) meTower[i].hoverOK = true;
            if(rando) mys.forcedHover(); else {prev.forcedHover(); window.draw(markbox); window.draw(mark);}
            if(Mouse::isButtonPressed(Mouse::Left)&&gamestate/10 == 1)
            {
                if(rando)
                {
                    std::swap(mys.txt, meTower[gamestate-9].txt);
                    meTower[gamestate-9].wid=50+StringToInt(meTower[gamestate-9].txt)*3;
                    meTower[gamestate-9].x = meTower[gamestate-9].x + 25 + StringToInt(mys.txt)*3/2 - meTower[gamestate-9].wid/2;
                    urTurn=false; choose=true; gamestate = 5;
                    for(int i=1; i<=10; i++) meTower[i].hoverOK = false;

                    do{
                        p = rand()%50+1;
                    }while(occup[p]==1);
                    occup[StringToInt(mys.txt)] = 0;
                    occup[p]=1;
                    mys.txt=std::to_string(p);

                }
                else{
                    std::swap(prev.txt, meTower[gamestate-9].txt);
                    meTower[gamestate-9].wid=50+StringToInt(meTower[gamestate-9].txt)*3;
                    meTower[gamestate-9].x = meTower[gamestate-9].x + 25 + StringToInt(prev.txt)*3/2 - meTower[gamestate-9].wid/2;
                    urTurn=false; choose=true; gamestate = 5;
                    for(int i=1; i<=10; i++) meTower[i].hoverOK = false;
                }
            }
        }
    }
    else
    {

        indicator.setPosition(340, 50);
        window.draw(indicator);
        if(choose)
        {
            oldbetter=0;
            better = 0;
            for(int i=2; i<=10; i++)
            {
                if(StringToInt(enTower[i].txt)>StringToInt(enTower[i-1].txt)) oldbetter++;
            }
            window.draw(markbox);
            window.draw(mark);
            int aux;
            for(int i=1; i<=10; i++)
            {
                better = 0;
                aux = StringToInt(enTower[i].txt);
                enTower[i].txt = prev.txt;
                for(int i=2; i<=10; i++)
                {
                    if(StringToInt(enTower[i].txt)>StringToInt(enTower[i-1].txt)) better++;
                }
                //std::cout << oldbetter << " " << better << std::endl;
                enTower[i].txt = std::to_string(aux);

                if(better>oldbetter) {choose = false; gamestate = 5; rando = false; j=i; break;}
                rando = true; choose = false; gamestate = 5;
            }
            //std::cout<<std::endl;


        }
        else
        {

            if(rando)
            {
                int k = (StringToInt(mys.txt)-1)/5+1;

                std::swap(mys.txt, enTower[k].txt);
                enTower[k].wid=50+StringToInt(enTower[k].txt)*3;
                enTower[k].x = enTower[k].x + 25 + StringToInt(mys.txt)*3/2 - enTower[k].wid/2;

                urTurn=true; choose=true; gamestate = 5;

                do{
                    p = rand()%50+1;
                }while(occup[p]==1);
                occup[StringToInt(mys.txt)] = 0;
                occup[p]=1;
                mys.txt=std::to_string(p);

            }
            else
            {
                prev.forcedHover();
                window.draw(markbox);
                window.draw(mark);
                //std::cout<<j;

                std::swap(prev.txt, enTower[j].txt);
                enTower[j].wid=50+StringToInt(enTower[j].txt)*3;
                enTower[j].x = enTower[j].x + 25 + StringToInt(prev.txt)*3/2 - enTower[j].wid/2;

                urTurn=true; choose=true; gamestate = 5;
            }
            sleep(milliseconds(500));
        }
    }

    if(Check(1)) {winScreen.bd(); prev.hoverOK = mys.hoverOK = false;}
    if(Check(0)) {loseScreen.bd(); prev.hoverOK = mys.hoverOK = false;}

    Button menu;
    menu.init(200, window.getSize().y/2+350, 100, 40, "Menu", 1, true);
    menu.bd();


}

void debugIO(Event evnt) //function to test out text imput in order to see if the app still works or something i dunno
{
    switch (evnt.type)
    {
        case Event::TextEntered: textIO(evnt); break;
        default: break;
    }
    window.draw(playerText);
}

int main()
{

    init();
    srand (time(NULL));
    while (window.isOpen())
    {
        window.clear();
        Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
                case Event::Closed: window.close(); break;
                default: break;
            }
        }
        backgroundDraw();
        mousePos = Mouse::getPosition(window);
        switch (gamestate) {
            //gamestate switch 1 = menu, 2 = rules, 3 = gamemenu, 4 = initSP, 5 = singleplayer, 6 = debug input, 7 = initMP, 8 = multiplayer,
            case 1: menu(evnt); break;
            case 2: rules(evnt); break;
            case 3: gamemenu(evnt); break;
            case 4: initSP(evnt); break;
            case 5: singleplayer(evnt); break;
            //case 6: debugIO(evnt); break;
            //case 7:
            //case 8:
            case 9: window.close(); break;
            }
        window.display();
        window.setFramerateLimit(144);
        //std::cout << gamestate <<std::endl;
    }

    return 0;
}
