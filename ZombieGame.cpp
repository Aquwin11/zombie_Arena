#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Zombie.h"
#include "Bullet.h"
#include "PickUp.h"

using namespace sf;
using namespace std;

int main()
{
	srand((unsigned int)time(0));
	TextureHolder textHolder;
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	enum class GunTypes {Pistol, Rifle,Sniper,Shotgun};

	State currentState = State::GAME_OVER;
	GunTypes currentGun = GunTypes::Pistol;
	int gunIs = 1;
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));


	Clock clock;
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;


	Player player;
	IntRect arena;

	//player with different guns
	Texture playerPistolTexture= TextureHolder::GetTexture("graphics/playerpistol.png");
	Texture playerRifleTexture= TextureHolder::GetTexture("graphics/chaser.png");
	Texture playerSniperTexture= TextureHolder::GetTexture("graphics/playersniper.png");
	Texture playerShotgunTexture= TextureHolder::GetTexture("graphics/playershotgun.png");
	//Vertex Array
	VertexArray backGround;
	Texture backgroundTexture = TextureHolder::GetTexture("graphics/background_sheet.png");

	int numZombie;
	int numZombieAlive;
	Zombie* zombie = nullptr;

	int reloadCounter;
	bool isReloading=false;
	//Bullets
	Bullet bullets[100];
	Bullet enemyProjectile[10];
	int currentEnemyProjectile = 0;
	int maxEnemyProjectile = 9;
	int maxBullets = 99;
	int currentBullets=0;
	int bulletsInClip = 6;
	int totalBullets = 24;
	int clipSize = 6;
	float fireRate = 1;
	bool canSHoot = true;
	int reloadTime;

	//last fired
	Time lastFired;
	
	//CrossHair
	window.setMouseCursorVisible(false);
	Sprite mouseCrossHairSprite;
	Texture crossHairTextture = TextureHolder::GetTexture("graphics/crosshair.png");
	mouseCrossHairSprite.setTexture(crossHairTextture);
	mouseCrossHairSprite.setOrigin(25, 25);

	//PickUps
	PickUp healthPickUps(1);
	PickUp ammoPickUps(2);
	PickUp rifleammoPickUps(3);
	PickUp sniperammoPickUps(4);
	PickUp shotgunammoPickUps(5);
	
	//Score
	int score = 0;
	int highScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	Sprite spriteReloadIcon;
	Texture textureReloadIcon = TextureHolder::GetTexture("graphics/newreload_icon.png");
	spriteReloadIcon.setTexture(textureReloadIcon);
	spriteReloadIcon.setPosition(250, 990);


	// Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	// Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased fire rate of pistol " <<
		"\n2- Increased rifle clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups" <<
		"\n7- Decrease sniper reload time"<<
		"\n8- Decrease shotgun spread";
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(100, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	//load score file 
	std::fstream inputFile("gameData/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> highScore;
		inputFile.close();
	}
	
	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << highScore;
	hiScoreText.setString(s.str());



	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(resolution.x/3, 0);

	int frameSinceLastUpdate = 0;
	int fpsIntervals = 1000;

	//Gun
	//Pistol
	const int PISTOL_CLIP_SIZE = 6;
	float PISTOL_FIRE_RATE = 1;
	int Pistol_BulletsInClip= 6;
	int Pistol_TotalBullets= 12;
	bool pistol_CanSHoot = true;
	int pistol_ReloadTime=1000;

	//Assualt RIFLE
	int   RIFLE_CLIP_SIZE = 20;
	const float RIFLE_FIRE_RATE = 4;
	int rifle_BulletsInClip = 20;
	int rifle_TotalBullets = 30;
	bool rifle_CanSHoot = true;
	int  rifle_ReloadTime=2000;


	//SNIPER
	const int   SNIPER_CLIP_SIZE = 1;
	const float SNIPER_FIRE_RATE = 0.5;
	int sniper_BulletsInClip = 1;
	int sniper_TotalBullets = 2;
	bool sniper_CanSHoot = true;
	int  sniper_ReloadTime=4000;


	//SHOTGUN
	const int   SHOTGUN_CLIP_SIZE = 2;
	const float SHOTGUN_FIRE_RATE = 2;
	int shotgun_BulletsInClip = 2;
	int shotgun_TotalBullets = 6;
	bool shotgun_CanSHoot = true;
	int  shotgun_ReloadTime=1000;
	int const SHOTGUN_PELLETS = 4;
	float SHOTGUN_SPREAD_DEGREES = 40.0f; // 10 degrees spread
	const float SHOTGUN_SPREAD = SHOTGUN_SPREAD_DEGREES * 3.14159f / 180.0f; // Convert to radians


	//Gun HUD
	Sprite pistolIcon;
	Texture texturePistolIcon = TextureHolder::GetTexture("graphics/pistol.png");
	pistolIcon.setTexture(texturePistolIcon);
	pistolIcon.setPosition(500,960);
	Sprite rifleIcon;
	Texture textureRifleIcon = TextureHolder::GetTexture("graphics/rifle.png");
	rifleIcon.setTexture(textureRifleIcon);
	rifleIcon.setPosition(540, 980);
	Sprite sniperIcon;
	Texture textureSniperIcon = TextureHolder::GetTexture("graphics/sniper.png");
	sniperIcon.setTexture(textureSniperIcon);
	sniperIcon.setPosition(644, 980);
	Sprite shotgunIcon;
	Texture textureShotgunIcon = TextureHolder::GetTexture("graphics/shotgun.png");
	shotgunIcon.setTexture(textureShotgunIcon);
	shotgunIcon.setPosition(780, 980);


	//Sounds
	//hit
	SoundBuffer hitbuffer;
	hitbuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitbuffer);
	//splat
	SoundBuffer splatbuffer;
	splatbuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatbuffer);
	//shoot
	SoundBuffer shootbuffer;
	shootbuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootbuffer);

	//reload
	SoundBuffer reloadbuffer;
	reloadbuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadbuffer);
	//reload_failed
	SoundBuffer reloadFailedbuffer;
	reloadFailedbuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedbuffer);

	//powerup
	SoundBuffer powerupbuffer;
	powerupbuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupbuffer);
	//pickup
	SoundBuffer pickupbuffer;
	pickupbuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupbuffer);

	// The main game loop
	while (window.isOpen())
	{
		/*Handle input*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{

				if (event.key.code == Keyboard::Return &&
					currentState == State::PLAYING)
				{
					currentState = State::PAUSED;
				}


				else if (event.key.code == Keyboard::Return &&
					currentState == State::PAUSED)
				{
					currentState = State::PLAYING;

					clock.restart();
				}

				else if (event.key.code == Keyboard::Return &&
					currentState == State::GAME_OVER)
				{
					currentState = State::LEVELING_UP;
					wave = 0;
					score = 0;
					currentBullets = 0;
					totalBullets = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					//reset player
					player.ResetPlayerStats();
				}
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					mainView.rotate(.5f);
				}

				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					mainView.rotate(-.5f);
				}

				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					mainView.zoom(.99f);
				}

				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					mainView.zoom(1.01f);
				}

				if (currentState == State::PLAYING)
				{
					//Reload
					if (event.key.code == Keyboard::R)
					{
						if (bulletsInClip < clipSize)
						{
							isReloading = true;
						}
					}
				}

			}
		}



		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls
		if (currentState == State::PLAYING)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.StopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.StopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.StopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.StopRight();
			}
			//Fire Bullets
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastFired.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0 && !isReloading) {
					int numPellets = (gunIs == 4) ? SHOTGUN_PELLETS : 1;
					float dx = mouseWorldPosition.x - player.getCenter().x;
					float dy = mouseWorldPosition.y - player.getCenter().y;
					float baseAngle = atan2(dy, dx);
					float spreadAngle;
					float finalAngle;
					for (int i = 0; i < numPellets; ++i) {
						if (gunIs == 4) {
							 spreadAngle = ((float)rand() / (float)RAND_MAX - 0.5f) * 2 * SHOTGUN_SPREAD;
							 finalAngle = baseAngle + spreadAngle;
						}
						else
						{
							finalAngle = baseAngle;
						}

						float adjustedTargetX = player.getCenter().x + cos(finalAngle) * 1000;
						float adjustedTargetY = player.getCenter().y + sin(finalAngle) * 1000;

						// Fire bullet with adjusted trajectory
						bullets[currentBullets].shoot(player.getCenter().x, player.getCenter().y, adjustedTargetX, adjustedTargetY, gunIs);
						bullets[currentBullets].SetBulletSize(bullets[currentBullets].GetBulletSize());

						currentBullets++;
						if (currentBullets >= maxBullets)
						{
							currentBullets = 0;
						}
					}

					lastFired = gameTimeTotal;
					shoot.play();
					bulletsInClip--;
					switch (currentGun)
					{
					case GunTypes::Pistol:
						Pistol_BulletsInClip--;
						break;
					case GunTypes::Rifle:
						rifle_BulletsInClip--;
						break;
					case GunTypes::Sniper:
						sniper_BulletsInClip--;
						break;
					case GunTypes::Shotgun:
						shotgun_BulletsInClip--;
						break;
					}
				}

			}
			//Switching GUN
			if (event.key.code == Keyboard::Num1)
			{
				currentGun = GunTypes::Pistol;
				gunIs = 1;
				fireRate = PISTOL_FIRE_RATE;
				clipSize = PISTOL_CLIP_SIZE;
				player.ChangeTexture("graphics/playerpistol.png");
				reloadCounter = 0;
				bulletsInClip = Pistol_BulletsInClip;
				totalBullets = Pistol_TotalBullets;
				reloadTime = pistol_ReloadTime;
				isReloading = false;
				pistolIcon.setPosition(500, 960);
				rifleIcon.setPosition(540, 980);
				sniperIcon.setPosition(644, 980);
				shotgunIcon.setPosition(780, 980);
			}

			if (event.key.code == Keyboard::Num2)
			{
				currentGun = GunTypes::Rifle;
				gunIs = 2;
				fireRate = RIFLE_FIRE_RATE;
				clipSize = RIFLE_CLIP_SIZE;
				bulletsInClip = rifle_BulletsInClip;
				totalBullets =  rifle_TotalBullets;
				player.ChangeTexture("graphics/playerrifle.png");
				reloadCounter = 0;
				reloadTime = rifle_ReloadTime;
				isReloading = false;
				pistolIcon.setPosition(500, 980);
				rifleIcon.setPosition(540, 960);
				sniperIcon.setPosition(644, 980);
				shotgunIcon.setPosition(780, 980);
			}

			if (event.key.code == Keyboard::Num3)
			{
				currentGun = GunTypes::Sniper;
				gunIs = 3;
				fireRate = SNIPER_FIRE_RATE;
				clipSize = SNIPER_CLIP_SIZE;
				player.ChangeTexture("graphics/playersniper.png");
				bulletsInClip = sniper_BulletsInClip;
				totalBullets =  sniper_TotalBullets;
				reloadTime = sniper_ReloadTime;
				reloadCounter = 0;
				isReloading = false;
				pistolIcon.setPosition(500, 980);
				rifleIcon.setPosition(540, 980);
				sniperIcon.setPosition(644, 960);
				shotgunIcon.setPosition(780, 980);

			}

			if (event.key.code == Keyboard::Num4)
			{
				currentGun = GunTypes::Shotgun;
				gunIs = 4;
				fireRate = SHOTGUN_FIRE_RATE;
				clipSize = SHOTGUN_CLIP_SIZE;
				bulletsInClip =shotgun_BulletsInClip;
				totalBullets = shotgun_TotalBullets;
				player.ChangeTexture("graphics/playershotgun.png");
				reloadTime = shotgun_ReloadTime;
				isReloading = false;
				reloadCounter = 0;
				pistolIcon.setPosition(500, 980);
				rifleIcon.setPosition(540, 980);
				sniperIcon.setPosition(644, 980);
				shotgunIcon.setPosition(780, 960);

			}

		}

		// Handle the levelling
		if (currentState == State::LEVELING_UP)
		{

			if (event.key.code == Keyboard::Num1)
			{
				// Increase fire rate
				PISTOL_FIRE_RATE++;
				currentState = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// Increase clip size
				RIFLE_CLIP_SIZE += clipSize;
				currentState = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// Increase health
				player.UpgradeHealth();
				currentState = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// Increase speed
				player.UpgradeSpeed();
				currentState = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				//health value increase
				healthPickUps.Upgrade();
				currentState = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				//ammo value increase
				ammoPickUps.Upgrade();
				rifleammoPickUps.Upgrade();
				sniperammoPickUps.Upgrade();
				shotgunammoPickUps.Upgrade();
				currentState = State::PLAYING;

			}
			if (event.key.code == Keyboard::Num7)
			{
				//decrease reload time#
				if (sniper_ReloadTime >= 1000)
				{
					sniper_ReloadTime -= 500;
				}
				currentState = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num8)
			{
				//decrease reload time#
				if (SHOTGUN_SPREAD_DEGREES >= 10)
				{
					SHOTGUN_SPREAD_DEGREES -= 5;
				}
				currentState = State::PLAYING;
			}

			if (currentState == State::PLAYING)
			{
				// Increase the wave number
				wave++;
				if (wave <= 5)
				{
					arena.width = 500 * wave;
					arena.height = 500 * wave;
				}
				else
				{
					arena.width = 500 * 5;
					arena.height = 500 * 5;
				}
				arena.left = 0;
				arena.top = 0;


				int tileSize = CreateBackground(backGround,arena);

				player.Spawn(arena, resolution, tileSize);
				healthPickUps.SetArena(arena);
				ammoPickUps.SetArena(arena);
				rifleammoPickUps.SetArena(arena);
				sniperammoPickUps.SetArena(arena);
				shotgunammoPickUps.SetArena(arena);
				numZombie = 5*wave;
				delete[] zombie;
				zombie = CreateZombieHorde(numZombie, arena);
				numZombieAlive = numZombie;
				powerup.play();
				clock.restart();
			}
		}

		/*UPDATE THE FRAME*/
		if (currentState == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();

			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();


			mouseScreenPosition = Mouse::getPosition();

			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());
			player.updateFlickerEffect(gameTimeTotal);
			Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());
			for (int i = 0; i < numZombie; i++)
			{
				if (zombie[i].IsAlive())
				{
					zombie[i].Update(dtAsSeconds, playerPosition);
					if (zombie[i].GetZ_Type() == 3 && zombie[i].CanShoot(gameTimeTotal.asSeconds(), playerPosition))
					{
						enemyProjectile[currentEnemyProjectile].ZomShoot(zombie[i].GetPosition().getPosition().x, zombie[i].GetPosition().getPosition().y 
							, player.getPosition().getPosition().x, player.getPosition().getPosition().y);
						enemyProjectile[currentEnemyProjectile].SetBulletSize(8);
						currentEnemyProjectile++;
						if (currentEnemyProjectile >= maxEnemyProjectile)
						{
							currentEnemyProjectile = 0;
						}
					}
				}
				else if (!zombie[i].IsAlive() && zombie[i].IsFading())
				{
					zombie[i].FadeSplat(dtAsSeconds);
				}
			}
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].IsInFlight())
				{
					bullets[i].Update(dtAsSeconds);
				}
			}
			for (int i = 0; i < 10; i++)
			{
				if (enemyProjectile[i].IsInFlight())
				{
					enemyProjectile[i].Update(dtAsSeconds);
				}
			}
			
			healthPickUps.Update(dtAsSeconds);
			ammoPickUps.Update(dtAsSeconds);
			rifleammoPickUps.Update(dtAsSeconds);
			sniperammoPickUps.Update(dtAsSeconds);
			shotgunammoPickUps.Update(dtAsSeconds);
			mouseCrossHairSprite.setPosition(mouseWorldPosition);

			//Collision Zombie and bullet
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombie; j++)
				{
					if (bullets[i].IsInFlight() && zombie[j].IsAlive())
					{
						if (bullets[i].GetPosition().intersects(zombie[j].GetPosition()))
						{
							bullets[i].stop();
							if (zombie[j].Hit(bullets[i].GetDamage()))
							{
								score += 10;
								float r = ((float)rand() / (RAND_MAX));
								if (r > 0.9)
								{
									healthPickUps.SpawnMeds(zombie[j].GetPosition());
								}
								if (score >= highScore)
								{
									highScore = score;
								}
								numZombieAlive--;
								if (numZombieAlive == 0)
								{
									currentState = State::LEVELING_UP;
								}
							}
							splat.play();
						}

					}
				}
			}

			//Collision Zombie and player
			for (int i = 0; i < numZombie; i++)
			{
				if (player.getPosition().intersects(zombie[i].GetPosition()) && zombie[i].IsAlive())
				{
					if (player.Hit(gameTimeTotal,zombie[i].GetZomDamage()))
					{
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						currentState = State::GAME_OVER;
						std::ofstream outFile("gameData/scores.txt");
						outFile << highScore;
						outFile.close();
					}
				}
			}

			//collision player enemyBullet
			for (int i = 0; i < 9; i++)
			{
				if (player.getPosition().intersects(enemyProjectile[i].GetPosition()) && enemyProjectile[i].IsInFlight())
				{
					if (player.Hit(gameTimeTotal,10))
					{
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						currentState = State::GAME_OVER;
						std::ofstream outFile("gameData/scores.txt");
						outFile << highScore;
						outFile.close();
					}
					enemyProjectile[i].stop();
				}
			}

			//Collision player and pickups
			if (player.getPosition().intersects(healthPickUps.GetPostion()) && healthPickUps.isSpawned())
			{
				pickup.play();
				player.IncreaseHealthLevel(healthPickUps.Gotit());
			}
			if (player.getPosition().intersects(ammoPickUps.GetPostion()) && ammoPickUps.isSpawned())
			{
				reload.play();
				Pistol_TotalBullets += ammoPickUps.Gotit();
				if (currentGun == GunTypes::Pistol)
				{
					totalBullets += ammoPickUps.Gotit();
				}
			}
			if (player.getPosition().intersects(rifleammoPickUps.GetPostion()) && rifleammoPickUps.isSpawned())
			{
				reload.play();
				rifle_TotalBullets += rifleammoPickUps.Gotit();
				if (currentGun == GunTypes::Rifle)
				{
					totalBullets += rifleammoPickUps.Gotit();
				}
			}
			if (player.getPosition().intersects(sniperammoPickUps.GetPostion()) && sniperammoPickUps.isSpawned())
			{
				reload.play();
				sniper_TotalBullets += sniperammoPickUps.Gotit();
				if (currentGun == GunTypes::Sniper)
				{
					totalBullets += sniperammoPickUps.Gotit();
				}
			}
			if (player.getPosition().intersects(shotgunammoPickUps.GetPostion()) && shotgunammoPickUps.isSpawned())
			{
				reload.play();
				shotgun_TotalBullets += shotgunammoPickUps.Gotit();
				if (currentGun == GunTypes::Shotgun)
				{
					totalBullets += shotgunammoPickUps.Gotit();
				}
			}

			//healthBar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			frameSinceLastUpdate++;

			//reload
			if (isReloading)
			{
				if (reloadCounter >= reloadTime)
				{
					int bulletsToReload = clipSize - bulletsInClip;
					if (totalBullets >= bulletsToReload)
					{
						bulletsInClip = clipSize;
						totalBullets -= bulletsToReload;
						reload.play();
					}
					else if (totalBullets > 0)
					{
						reload.play();
						bulletsInClip = totalBullets;
						totalBullets = 0;
					}
					else
					{
						reloadFailed.play();
					}
					switch (currentGun)
					{
					case GunTypes::Pistol:
						Pistol_BulletsInClip = bulletsInClip;
						Pistol_TotalBullets = totalBullets;
						break;
					case GunTypes::Rifle:
						rifle_BulletsInClip = bulletsInClip;
						rifle_TotalBullets = totalBullets;
						break;
					case GunTypes::Sniper:
						sniper_BulletsInClip = bulletsInClip;
						sniper_TotalBullets = totalBullets;
						break;
					case GunTypes::Shotgun:
						shotgun_BulletsInClip = bulletsInClip;
						Pistol_TotalBullets = totalBullets;
						break;
					}
					reloadCounter = 0;
					isReloading = false;
				}
				reloadCounter++;
			}
			else
			{
				reloadCounter = 0;
			}
			if (frameSinceLastUpdate > fpsIntervals)
			{
				//Update game HUD
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHighScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//Ammo
				ssAmmo << bulletsInClip << "/" << totalBullets;
				ammoText.setString(ssAmmo.str());

				//score
				ssScore <<"Score:"<< score;
				scoreText.setString(ssScore.str());

				//Highscore
				ssHighScore << "HighScore:" << highScore;
				hiScoreText.setString(ssHighScore.str());

				//Waves
				ssWave<< "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				//Zombies
				ssZombiesAlive<< "Zombies:" << numZombieAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				frameSinceLastUpdate = 0;
			}
		}

		/*Draw the scene*/

		if (currentState == State::PLAYING)
		{
			window.clear();


			window.setView(mainView);
			window.draw(backGround, &backgroundTexture);
			for (int i = 0; i < numZombie; i++)
			{
				window.draw(zombie[i].GetSprite());
			}
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].IsInFlight())
				{
					window.draw(bullets[i].GetShape());
				}
			}
			for (int i = 0; i < 10; i++)
			{
				if (enemyProjectile[i].IsInFlight()) {
					window.draw(enemyProjectile[i].GetShape());
				}
			}
			window.draw(mouseCrossHairSprite);
			if (healthPickUps.isSpawned())
			{
				window.draw(healthPickUps.GetSprite());
			}
			if (ammoPickUps.isSpawned())
			{
				window.draw(ammoPickUps.GetSprite());
			}
			if (rifleammoPickUps.isSpawned())
			{
				window.draw(rifleammoPickUps.GetSprite());
			}
			if (sniperammoPickUps.isSpawned())
			{
				window.draw(sniperammoPickUps.GetSprite());
			}
			if (shotgunammoPickUps.isSpawned())
			{
				window.draw(shotgunammoPickUps.GetSprite());
			}
			window.draw(player.getSprite());
			window.setView(hudView);
			window.draw(spriteAmmoIcon);
			if (isReloading)
			{
				window.draw(spriteReloadIcon);
			}
			window.draw(scoreText);
			window.draw(healthBar);
			window.draw(hiScoreText);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
			window.draw(ammoText);
			window.draw(pistolIcon);
			window.draw(rifleIcon);
			window.draw(sniperIcon);
			window.draw(shotgunIcon);
		}

		if (currentState == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (currentState == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (currentState == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(hiScoreText);
			window.draw(scoreText);

		}

		window.display();

	}
	delete[] zombie;
	return 0;
}