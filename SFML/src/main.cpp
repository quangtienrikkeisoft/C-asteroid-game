#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>

// dont using namespace

const float DEGTORAD = 0.017453f;
const int width = 1600;
const int height = 1000;
const int FPS = 60;
// define the class
class Animation{
     public:
     float Frame, speed;
     sf::Sprite sprite;
     std::vector<sf::IntRect> frames;
     // default constructor
     Animation(){}
     Animation(sf::Texture &t,int x, int y, int w,int h, int count, float Speed){
          Frame = 0;
          speed = Speed;
          for(int i=0;i<count;i++){
               frames.push_back(sf::IntRect(x+i*w, y, w,h));
          }
          sprite.setTexture(t);
          sprite.setOrigin(w/2, h/2);
          sprite.setTextureRect(frames[0]);
     }
     //function update the animation frame
     void update(){
          Frame += speed;
          int n= frames.size();
          if(Frame >= n) Frame -=n;
          if(n>0) sprite.setTextureRect(frames[int(Frame)]);
     }
     //end of the animation
     bool isEnd(){
          return Frame +speed >= frames.size();
     }
};

class Entity{
     public:
     float x,y,dx,dy,R, angle;
     bool life;
     std::string name;
     Animation animation;

     Entity(){
          life = 1;
     }
     void settings(Animation &a, int X, int Y , float Angle= 0, int radius=1){
          animation = a;
          x = X;
          y = Y;
          R= radius;
     }
     virtual void update(){}
     void draw(sf::RenderWindow &window){
          animation.sprite.setPosition(x,y);
          animation.sprite.setRotation(angle + 90);
          window.draw(animation.sprite);

          sf::CircleShape circle(R);
          circle.setFillColor(sf::Color(255,0,0,170));
          circle.setPosition(x,y);
          circle.setOrigin(R,R);
     }
     virtual ~Entity(){};
};

class Asteroid: public Entity{
     public:
     Asteroid(){
          dx = rand() % 8 -4;
          dy = rand() %8 -4;
          name = "asteroid";
     }
     // update the positon of the asteroid
     void update(){
          x +=dx;y +=dy;
          if(x>width) x= 0; if (x <0) x= width;
          if(y>height) y = 0; if(y< 0)   y = height;
     }
};

class Player: public Entity{
     public:
     bool thrust;
     Player(){
          name ="player";
     }
     void update(){
          if(thrust){
               dx+=cos(angle*DEGTORAD)*1;
               dy+=sin(angle*DEGTORAD)*1;
          }
          else{
               dx*=0.99;
               dx*=0.99;
          }
          int maxSpeed = 15;
          float speed = sqrt(dx*dx + dy*dy);
          if(speed>maxSpeed){
               dx *= maxSpeed/speed;
               dy *= maxSpeed/speed;
               x+=dx;
               y+=dy;

               if (x>width) x=0; if (x<0) x=width;
               if (y>height) y=0; if (y<0) y=height;
          }
     }
};


int main(){
     srand(time(0));
     sf::RenderWindow window(sf::VideoMode(width, height), "The game!");
     window.setFramerateLimit(FPS);

     // loading the texture of the game
     sf::Texture background_text , player_text, big_rock_text ,small_rock_text;
     background_text.loadFromFile("/home/tientq/Downloads/16_Games/16 Asteroids/images/background.jpg");
     player_text.loadFromFile("/home/tientq/Downloads/16_Games/16 Asteroids/images/spaceship.png");
     big_rock_text.loadFromFile("/home/tientq/Downloads/16_Games/16 Asteroids/images/rock.png");
     small_rock_text.loadFromFile("/home/tientq/Downloads/16_Games/16 Asteroids/images/rock_small.png");
     // texture, x,y,width , height , count_frame , speed
     Animation sSmall_rock(big_rock_text, 0,0,64,64,16,0.2);
     Animation sPlayer(player_text, 40 ,0,40,40,1,0);
     Animation sPlayer_go(player_text, 40,40,40,40,1,0);
     // main loop
     sf::Sprite sBackground(background_text);
     std::list<Entity* > entities;
     for(int i = 0;i<10;i++){
          Asteroid* a = new Asteroid();
          a->settings(sSmall_rock, rand()%width , rand()%height, rand()%360, 25);
          entities.push_back(a);
     }
     Player* p = new Player();
     p->settings(sPlayer, 400,400,0,20);
     entities.push_back(p);
     while(window.isOpen()){
          sf::Event event;
          while(window.pollEvent(event)){
               if(event.type == sf::Event::Closed) window.close();
               if(event.type == sf::Event::KeyPressed){
               }
               if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p->angle +=9;
               if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) p->angle -=9;
               if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) p->thrust = true;
               else p->thrust = false;
          }
          // draw
          window.clear();
          window.draw(sBackground);
          for(auto i=entities.begin(); i != entities.end();){
               Entity* e = * i;
               e->update();
               e->animation.update();
               i++;
          }

          for(auto i:entities){
               i -> draw(window);
          }
          window.display();
     }
     return 0;
}
