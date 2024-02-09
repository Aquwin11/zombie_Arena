#include<SFML/Graphics.hpp>


using namespace sf;
class Player {

private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	Sprite m_Sprite;
	Texture m_Texture;
	Vector2f m_Position;

	Vector2f m_Resolution;
	IntRect m_CurrentArea;

	int m_TitleSize;

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_CurrentHealth;
	int m_MaxHealth;

	Time m_LastHit;

	float m_Speed;

public:
	Player();

	void Spawn(IntRect area, Vector2f resolution, int tileSize);

	void ResetPlayerStats();

	bool Hit(Time timeHit);

	Time getLastHit();

	FloatRect getPosition();

	Vector2f getCenter();

	float getRotation();

	Sprite getSprite();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void StopLeft();
	void StopRight();
	void StopUp();
	void StopDown();

	void update(float timeElaspsed, Vector2i mousePos);

	void UpgradeSpeed();

	void UpgradeHealth();

	void IncreaseHealthLevel(int amount);

	int getHealth();

	void ChangeTexture(Texture newTexture);
};
