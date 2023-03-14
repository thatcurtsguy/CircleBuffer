#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Rect<float> m_border;

	const float m_radius;

	void borderCollision()
	{
		const float buffer = m_radius;

		const bool x_out_of_bounds = m_position.x < m_border.left + buffer || m_position.x > m_border.left + m_border.width - buffer;
		const bool y_out_of_bounds = m_position.y < m_border.top + buffer || m_position.y > m_border.top + m_border.height - buffer;

		if (x_out_of_bounds) {
			m_velocity.x *= -1;
		}

		if (y_out_of_bounds) {
			m_velocity.y *= -1;
		}

		m_position.x = std::max(m_border.left + buffer, std::min(m_position.x, m_border.left + m_border.width - buffer));
		m_position.y = std::max(m_border.top + buffer, std::min(m_position.y, m_border.top + m_border.height - buffer));
	}


public:
	// constructor and destructor
	explicit Entity(const sf::Vector2f position, const sf::Vector2f velocity, const sf::Rect<float> border, const float radius)
		: m_position(position), m_velocity(velocity), m_border(border), m_radius(radius)
	{
		
	}
	~Entity() = default;


	void update()
	{
		m_position += m_velocity;
		m_velocity /= 1.1;
		borderCollision();
	}
};