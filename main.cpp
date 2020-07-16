#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <math.h>

static size_t WINDOW_WIDTH = 400;
static size_t WINDOW_HEIGHT = 400;
static const size_t NUM_PARTICLES = 100;
static const float MAX_SPEED = 100.0f;

constexpr double PI = 3.14159265358979323846;

struct Particle
{
   sf::Vector2f pos;
   sf::Vector2f speed;
   sf::Color color;
   size_t radius;

   struct Explosion
   {
   sf::Time lifetime;
   sf::Time startLifetime;
};

float getRandomSpeed()
{
   return static_cast<float>( rand() / static_cast<float>(RAND_MAX) * MAX_SPEED );
}

struct Explosion
{
   public:
      Explosion( sf::Vector2i origin )
      {
         const uint8_t red = 255 * ( rand() % 2 );
         for( size_t i = 0; i < NUM_PARTICLES; ++i )

             struct Explosion
             {
         {
             Particle p;
             p.pos.x = origin.x;
             p.pos.y = origin.y;
             const float angle = rand();

             p.speed.x = -MAX_SPEED/2.0f + ( static_cast<float>( rand() / static_cast<float>(RAND_MAX) ) * MAX_SPEED );
             p.speed.y = -MAX_SPEED/2.0f + ( static_cast<float>( rand() / static_cast<float>(RAND_MAX) ) * MAX_SPEED );

             p.speed.x = getRandomSpeed() * std::sin( angle ); p.speed.y = getRandomSpeed() * std::cos( angle );

             p.radius = 1 + rand() % 5;
             p.lifetime = sf::seconds( 2 + rand() % 3 );
             p.startLifetime = p.lifetime;
             p.color = sf::Color( red, rand() % 256, rand() % 256 );
             particles.push_back( p );
         }
      }

      void draw( sf::RenderWindow& window )
      {
         for( const auto& particle : particles )
         {
           if( particle.lifetime == sf::Time::Zero )
           {
               continue;
           }

           sf::CircleShape c( particle.radius, 10 );
           c.setPosition( particle.pos.x - particle.radius, particle.pos.y - particle.radius );
           c.setFillColor( particle.color );
           window.draw( c );
         }
      }

      void update( const sf::Time& elapsedTime )
      {
         for( auto& particle : particles )
         {
            const float percent = ( particle.startLifetime - particle.lifetime ).asMilliseconds() / particle.startLifetime.asMilliseconds();
            const float t = ( 1. / ( 0.8 + percent ) ) * elapsedTime.asSeconds();
            if( false )
            {
               // Increasing speed
            }
            else
            {
               // Decreasing speed
            }

            particle.pos += particle.speed * t;
            particle.lifetime = elapsedTime > particle.lifetime ? sf::Time::Zero : particle.lifetime - elapsedTime;
            particle.color.a = particle.lifetime.asMilliseconds() / static_cast<float>(particle.startLifetime.asMilliseconds()) * 255.0f;
         }
      }

      bool isEmpty()
      {
         for( const auto& particle : particles )
         {
            if( particle.lifetime != sf::Time::Zero )
                return false;
         }

         return true;
      }

      std::vector<Particle> particles;
};

bool appendRandomExplosion( std::vector<Explosion>& explosions )
{
    static Explosion* lastExplosion = nullptr;
    if( explosions.size() > 0 )
    {
        size_t explosionIndex = 0;
        for( size_t i = 0; i < explosions.size(); ++i )
        {
            if( lastExplosion != &explosions[i])
            {
               explosionIndex = i;
               break;
            }
        }

        size_t index = rand() % explosions[explosionIndex].particles.size();
        const Particle& p = explosions[explosionIndex].particles[index];
        if( p.lifetime > sf::Time::Zero &&
            ( p.startLifetime - p.lifetime ) > sf::seconds( 1 ) &&
            ( p.startLifetime - p.lifetime ) < sf::seconds( 3 ) )
        {
           Explosion s( sf::Vector2i( p.pos.x, p.pos.y ) );
           explosions.push_back( s );
           lastExplosion = &explosions[0];
           return true;
        }
    }

    return false;
}

int main()
{
   sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_WIDTH), "Matrix!", sf::Style::Resize);

   std::vector<sf::Vector2i> circles;
   std::vector<Explosion> explosions;


   bool stop = false;
   sf::Clock clock;
   sf::Vector2i lastMousePos;
   sf::Clock randomExplosion;
   while (window.isOpen())
   {
      sf::Vector2i mousePos = sf::Mouse::getPosition( window );
      sf::Event event;
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
         else if(event.type == sf::Event::KeyPressed)
             stop = true;
         else if(event.type == sf::Event::KeyReleased)
             stop = false;
         else if(event.type == sf::Event::Resized)
         {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
         }
         else if( event.type == sf::Event::MouseButtonPressed )
         {
             Explosion s( mousePos );
             explosions.push_back( s );
             std::cerr << "pos x: " << mousePos.x << " pos y: " << mousePos.y;
         }
      }

      if( explosions.size() > 0 && explosions[0].isEmpty() )
      {
          explosions.erase( explosions.begin(), explosions.begin()+1 );
      }

      if( randomExplosion.getElapsedTime() > sf::milliseconds( 200 ) )
      {
         if( appendRandomExplosion( explosions ) )
         {
            randomExplosion.restart();
         }
      }

      if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && lastMousePos != mousePos )
      {
           Explosion s( mousePos );
           explosions.push_back( s );
      }

      lastMousePos = mousePos;

      if( WINDOW_WIDTH != window.getSize().x )
      {
         WINDOW_WIDTH = window.getSize().x;
         WINDOW_HEIGHT = window.getSize().y;
         std::cerr << "window: " << window.getSize().x << "," << window.getSize().y << "\n";
      }

      window.clear();

      sf::Time elapsedTime = clock.restart();
      for( auto& explosion : explosions )
      {
          explosion.draw( window );
          explosion.update( elapsedTime );
      }

      window.display();

      usleep(1000 * 10);

   }

   return 0;
}
