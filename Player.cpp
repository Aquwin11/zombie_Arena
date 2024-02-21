#include "Player.h"
#include "TextureHolder.h"
Player::Player()
{
	m_Speed = START_SPEED;
	m_CurrentHealth = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	m_Texture = TextureHolder::GetTexture("graphics/playerpistol.png");
	m_Sprite.setTexture(m_Texture);

	m_Sprite.setOrigin(25, 25);
}
void Player::Spawn(IntRect area, Vector2f resolution, int tileSize)
{
	m_Position.x = area.width / 2;
	m_Position.y = area.height / 2;

	m_CurrentArea.left = area.left;
	m_CurrentArea.top = area.top;
	m_CurrentArea.width = area.width;
	m_CurrentArea.height = area.height;

	m_TitleSize = tileSize;
	m_Resolution = resolution;
}

void Player::ResetPlayerStats()
{
	m_Speed = START_SPEED;
	m_MaxHealth = START_HEALTH;
	m_CurrentHealth = START_HEALTH;
}

Time Player::getLastHit()
{
	return m_LastHit;
}

bool Player::Hit(Time timeHit,float dmg)
{
	if ((timeHit.asMilliseconds() - m_LastHit.asMilliseconds()) > 500)
	{
		m_LastHit = timeHit;
		m_CurrentHealth -= dmg;
		m_IsFlickering = true;
		m_FlickerStartTime = timeHit;
		m_LastToggleTime = timeHit - m_FlickerToggleInterval; // Ensure immediate toggle
		return true;
	}
	else
	{
		return false;
	}
}

FloatRect Player::getPosition() {
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

int Player::getHealth()
{
	return m_CurrentHealth;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}
void Player::moveRight()
{
	m_RightPressed = true;
}
void Player::moveUp()
{
	m_UpPressed = true;
}
void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::StopLeft()
{
	m_LeftPressed = false;
}
void Player::StopRight()
{
	m_RightPressed = false;
}
void Player::StopUp()
{
	m_UpPressed = false;
}
void Player::StopDown()
{
	m_DownPressed = false;
}

void Player::update(float timeElaspsed, Vector2i mousePos)
{
	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * timeElaspsed;
	}
	if (m_DownPressed)
	{
		m_Position.y += m_Speed * timeElaspsed;
	}
	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * timeElaspsed;
	}
	if (m_RightPressed)
	{
		m_Position.x += m_Speed * timeElaspsed;
	}
	m_Sprite.setPosition(m_Position);

	//Keep player in Area
	if (m_Position.x > m_CurrentArea.width - m_TitleSize)
	{
		m_Position.x = m_CurrentArea.width - m_TitleSize;
	}
	if (m_Position.x < m_CurrentArea.left + m_TitleSize)
	{
		m_Position.x = m_CurrentArea.left + m_TitleSize;
	}
	if (m_Position.y < m_CurrentArea.top + m_TitleSize)
	{
		m_Position.y = m_CurrentArea.top + m_TitleSize;
	}
	if (m_Position.y > m_CurrentArea.height - m_TitleSize)
	{
		m_Position.y = m_CurrentArea.height - m_TitleSize;
	}

	float angle = (atan2(mousePos.y - m_Resolution.y / 2,
		mousePos.x - m_Resolution.x / 2)
		* 180) / 3.141;

	m_Sprite.setRotation(angle);
}

void Player::UpgradeHealth()
{
	m_MaxHealth += (START_HEALTH * .2);
}
void Player::UpgradeSpeed()
{
	m_Speed += (START_SPEED * .2f);
}

void Player::IncreaseHealthLevel(int amount)
{
	m_CurrentHealth += amount;
	if (m_CurrentHealth > m_MaxHealth)
	{
		m_CurrentHealth = m_MaxHealth;
	}

}

void Player::ChangeTexture(const std::string& textureName)
{
	sf::Texture& newTexture = TextureHolder::GetTexture(textureName);
	m_Sprite.setTexture(newTexture);
}
void Player::updateFlickerEffect(sf::Time elapsedTime) {
	if (m_IsFlickering) {
		if (elapsedTime - m_FlickerStartTime > m_FlickerDuration) {
			m_IsFlickering = false;
			m_Sprite.setColor(sf::Color::White);
		}
		else {
			if (elapsedTime - m_LastToggleTime > m_FlickerToggleInterval) {
				m_FlickerColorToggle = !m_FlickerColorToggle;
				m_Sprite.setColor(m_FlickerColorToggle ? sf::Color::Red : sf::Color::White);
				m_LastToggleTime = elapsedTime;
			}
		}
	}
}

