#pragma once
#include <SFML/Graphics.hpp>

/*
 * TODO:
 * - apon generation, turn all of the vertecies into circle molds so they can be processed
 * - custom "overflow" error message
 * - fix the removal bug
 *
 * Update optimisation:
 * - keep track of what verticies indexes have been updated up untill the user calles the update function
 * - then clear the container
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


	void setColor(std::vector<sf::Vertex>& vertices, sf::Color newColor)
	{
		for (const unsigned int index : indexes)
		{
			vertices[index].color = newColor;
		}
	}
};


class Buffer
{
	const unsigned int m_maxObjects;
	const unsigned int m_ObjectPoints;
	const unsigned int m_verticesMultiplier;
	unsigned int m_totalExpectedVertices;
	const double PI = 3.14159265358979;

	sf::VertexBuffer m_VertexBuffer;
	std::vector<sf::Vertex> m_vertices;
	std::vector<unsigned int> m_verticesIndexes;


public:
	// constructor and detructor
	explicit Buffer(unsigned int maxObjects, unsigned int objectPoints);
	Object handleOnePointPrimitive(sf::Vector2f position, sf::Color color);
	Object handleFourPointPrimitive(const sf::Vector2f position, const sf::Color color, const double size);
	Object handleThreePointPrimitive(sf::Vector2f position, sf::Color color, double size);
	~Buffer() = default;

	[[nodiscard]] Object add(sf::Vector2f position, double radius, sf::Color color);
	void addToVertexVector(Object& object, sf::Color color, const std::vector<sf::Vertex>& vertices);
	std::vector<sf::Vertex>* getVertices() { return &m_vertices; }
	sf::VertexBuffer* getBuffer() { return &m_VertexBuffer; }

	void remove(const Object* object);
	void render(sf::RenderTarget* renderTarget) const;
	void update();


private:
	[[nodiscard]] std::vector<sf::Vertex> createTriangleVertices(double radius, sf::Vector2f position) const;
	[[nodiscard]] std::vector<sf::Vertex> createSquare(const sf::Vector2f position, const double size) const;
	[[nodiscard]] std::vector<sf::Vertex> createTriangleAroundPoint(sf::Vector2f position, double size) const;
	[[nodiscard]] sf::Vector2f idxToCoords(unsigned int idx, double radius) const;
	[[nodiscard]] unsigned int scaleIndex(unsigned int index, bool scaleUp) const;
	[[nodiscard]] static sf::PrimitiveType getPrimitiveType(unsigned int objectPoints);
	[[nodiscard]] static unsigned int getMultiplier(unsigned int objectPoints);
	[[nodiscard]] unsigned int getNextIndex();
	//[[nodiscard]] static Object makeObject(sf::Vector2f position, const std::vector<unsigned>& indexes);
};

