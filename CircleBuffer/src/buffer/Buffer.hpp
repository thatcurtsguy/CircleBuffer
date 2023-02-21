#pragma once
#include <SFML/Graphics.hpp>

/*
 * TODO:
 * - apon generation, turn all of the vertecies into circle molds so they can be processed
 * - custom "overflow" error message
 */

struct Object
/* An Object is a class that manipulates Vertices inside of the VertexBuffer */
{
	std::vector<unsigned int> indexes;
	sf::Vector2f position;

	void setPosition(std::vector<sf::Vertex>& vertices, const sf::Vector2f newPosition)
	{
		const sf::Vector2f deltaPosition = newPosition - position;

		for (const unsigned int index : indexes)
		{
			vertices[index].position += deltaPosition;
		}

		position = newPosition;
	}


	void setColor()
	{
		
	}
};


class Buffer
{
	const unsigned int m_maxObjects;
	const unsigned int m_ObjectPoints;
	const unsigned int m_verticesMultiplier;
	unsigned int m_totalExpectedVertices;
	const double PI = 3.14159265358979323846;

	sf::VertexBuffer m_VertexBuffer;
	std::vector<sf::Vertex> m_vertices;
	std::vector<unsigned int> m_verticesIndexes;


public:
	// constructor and detructor
	explicit Buffer(unsigned int maxObjects, unsigned int objectPoints);
	~Buffer() = default;

	[[nodiscard]] Object add(sf::Vector2f position, double radius, sf::Color color);
	void remove(const Object* object);
	void render(sf::RenderTarget* renderTarget) const;
	void update();


private:
	[[nodiscard]] std::vector<sf::Vertex> createTriangleVertices(double radius, sf::Vector2f position) const;
	[[nodiscard]] sf::Vector2f idxToCoords(unsigned int idx, double radius) const;
	[[nodiscard]] unsigned int scaleIndex(unsigned int index, bool scaleUp) const;
	[[nodiscard]] static sf::PrimitiveType getPrimitiveType(unsigned int objectPoints);
	[[nodiscard]] static unsigned int getMultiplier(unsigned int objectPoints);
	[[nodiscard]] unsigned int getNextIndex();
	//[[nodiscard]] static Object makeObject(sf::Vector2f position, const std::vector<unsigned>& indexes);
};

