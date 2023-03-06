#include "Buffer.hpp"

#include <numeric> // needed for iota()



Buffer::Buffer(const unsigned maxObjects, const unsigned objectPoints, const sf::VertexBuffer::Usage usage)
	: m_maxObjects(maxObjects), m_ObjectPoints(objectPoints), m_verticesMultiplier(getMultiplier(objectPoints))
{
	// the total expected vertecies (m_maxObjects * m_ObjectPoints) is multiplied by three as we add a point every 3
	// indexes to represent the circle center. vertices
	m_totalExpectedVertices = m_maxObjects * m_ObjectPoints * m_verticesMultiplier;

	// preparing containers for oncoming objects
	m_vertices.resize(m_totalExpectedVertices, sf::Vertex());

	// vertices indexes are relative to the actual Object, meaning 1 index will hold info for (objectPoints * 3) Vertices
	m_verticesIndexes = std::vector<unsigned int>(m_maxObjects);
	std::iota(m_verticesIndexes.begin(), m_verticesIndexes.end(), 0);
	

	m_VertexBuffer = sf::VertexBuffer(getPrimitiveType(objectPoints), usage);
	m_VertexBuffer.create(m_totalExpectedVertices);
}


Object Buffer::handleOnePointPrimitive(const sf::Vector2f position, const sf::Color color)
{
	const unsigned int index = getNextIndex();
	m_vertices[index].position = position;
	m_vertices[index].color = color;
	return Object{ {index}, position };
}


Object Buffer::handleThreePointPrimitive(const sf::Vector2f position, const sf::Color color, const double size)
{
	const std::vector<sf::Vertex> triangle = createTriangleAroundPoint(position, size);

	Object object;
	object.position = position;
	addToVertexVector(object, color, triangle);
	return object;
}


Object Buffer::handleFourPointPrimitive(const sf::Vector2f position, const sf::Color color, const double size)
{
	const std::vector<sf::Vertex> rectangle = createSquare(position, size);

	// creating the object which will describe the Vertecies
	Object object;
	object.position = position;
	addToVertexVector(object, color, rectangle);
	return object;
}




Object Buffer::add(const sf::Vector2f position, const double radius, const sf::Color color)
{
	// if there is only one Vertex describing an object we can take a shortcut
	if (m_ObjectPoints == 1)
	{
		return handleOnePointPrimitive(position, color);
	}

	else if (m_ObjectPoints == 2)
	{
		// TODO
	}

	else if (m_ObjectPoints == 3)
	{
		return handleThreePointPrimitive(position, color, radius);
	}

	else if (m_ObjectPoints == 4)
	{
		return handleFourPointPrimitive(position, color, radius);
	}

	// Create the vertex data for the circle
	const std::vector<sf::Vertex> triangles = createTriangleVertices(radius, position);

	// creating the object which will describe the Vertecies
	Object object;
	object.position = position;

	addToVertexVector(object, color, triangles);

	return object;
}

void Buffer::addToVertexVector(Object& object, const sf::Color color, const std::vector<sf::Vertex>& vertices)
{
	const unsigned int startIndex = getNextIndex();
	object.indexes.resize(vertices.size());

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		m_vertices[startIndex + i].position = vertices[i].position;
		m_vertices[startIndex + i].color = color;
		object.indexes[i] = startIndex + i;
	}
}


void Buffer::remove(const Object* object)
{
	const unsigned int objectMinIndex = object->indexes[0];

	// freeing up a new index to be used
	m_verticesIndexes.push_back(scaleIndex(objectMinIndex, false));

	// "removing" the indexes from the Buffer by making it invisible
	for (const unsigned int index : object->indexes)
	{
		m_vertices.at(index).color = sf::Color(0, 0, 0, 0);
	}
}


void Buffer::render(sf::RenderTarget* renderTarget) const
{
	renderTarget->draw(m_VertexBuffer, sf::BlendAdd);
}


void Buffer::update()
{
	m_VertexBuffer.update(m_vertices.data(), m_vertices.size(), 0);
}


std::vector<sf::Vertex> Buffer::createTriangleVertices(const double radius, const sf::Vector2f position) const
{
	std::vector<sf::Vertex> triangles(static_cast<int>(m_ObjectPoints * 3));

	unsigned int index = 0;
	for (unsigned int i = 0; i < m_ObjectPoints; i++)
	{
		triangles[index + 0] = sf::Vertex({position + idxToCoords(i + 0, radius)}); // vertex 1
		triangles[index + 1] = sf::Vertex({ position + idxToCoords(i + 1, radius) }); // vertex 2
		triangles[index + 2] = position; // vertex center

		index += 3;
	}
	return triangles;
}

std::vector<sf::Vertex> Buffer::createSquare(const sf::Vector2f position, const double size) const
{
	// Calculate the half-size of the square
	const float halfSize = size / 2.0f;

	// Calculate the position of the top-left corner of the square
	const auto topLeft = sf::Vector2f(position.x - halfSize, position.y - halfSize);

	// Create an array of vertices for the square
	sf::Vertex vertices[] = {
		{topLeft, sf::Color::White},
		{sf::Vector2f(topLeft.x + size, topLeft.y), sf::Color::White},
		{sf::Vector2f(topLeft.x + size, topLeft.y + size), sf::Color::White},
		{sf::Vector2f(topLeft.x, topLeft.y + size), sf::Color::White}
	};

	// Convert the vertex array to a vector and return it
	std::vector result(vertices, vertices + std::size(vertices));
	return result;
}


std::vector<sf::Vertex> Buffer::createTriangleAroundPoint(const sf::Vector2f position, const double size) const
{
	// Calculate the height of the equilateral triangle
	const double height = size * sqrt(3) / 2.0f;

	// Calculate the position of the top corner of the triangle
	const sf::Vector2f top(position.x, position.y - height / 2.0f);

	// Calculate the positions of the other two corners of the triangle
	const sf::Vector2f left(top.x - size / 2.0f, top.y + height);
	const sf::Vector2f right(top.x + size / 2.0f, top.y + height);

	// Create an array of vertices for the triangle
	sf::Vertex vertices[] = {
		{top, sf::Color::White},
		{left, sf::Color::White},
		{right, sf::Color::White}
	};

	// Convert the vertex array to a vector and return it
	std::vector result(vertices, vertices + std::size(vertices));
	return result;
}



sf::Vector2f Buffer::idxToCoords(const unsigned idx, const double radius) const
{
	static const double angleIncrement = 2 * PI / static_cast<double>(m_ObjectPoints);

	const double angle = static_cast<double>(idx) * angleIncrement;
	const double cosAngle = std::cos(angle);
	const double sinAngle = std::sin(angle);

	const double x = cosAngle * radius;
	const double y = sinAngle * radius;

	return { static_cast<float>(x), static_cast<float>(y) };
}


unsigned Buffer::scaleIndex(const unsigned index, const bool scaleUp) const
{
	// false = scale down, true = scale up.
	// this function is used to convert indexes from the m_vertices and m_verticesIndexes containers
	const unsigned int scaleFactor = m_ObjectPoints * m_verticesMultiplier;
	if (scaleUp)
		return index * scaleFactor;
	return index / scaleFactor;
}

sf::PrimitiveType Buffer::getPrimitiveType(const unsigned int objectPoints)
{
	if (objectPoints == 1)
		 return sf::PrimitiveType::Points;

	if (objectPoints == 2)
		 return sf::PrimitiveType::Lines;

	if (objectPoints == 4)
		return sf::PrimitiveType::Quads;

	return sf::PrimitiveType::Triangles;
}

unsigned Buffer::getMultiplier(const unsigned int objectPoints)
{
	if (objectPoints == 1 || objectPoints == 2 || objectPoints == 4)
		return 1;
	return 3;
}

unsigned int Buffer::getNextIndex()
{
	const unsigned int index = scaleIndex(m_verticesIndexes.back(), true);
	m_verticesIndexes.pop_back();
	return index;
}

/*
Object Buffer::makeObject(
	const sf::Vector2f position, const sf::Color color, const std::vector<sf::Vertex>& vertices, 
	const std::vector<unsigned>& indexes)
{
	Object object{ indexes, position };

	for (const unsigned int index : object.indexes)
	{
		m_vertices[index].position = triangles[i].position;
		m_vertices[index].color = color;
	}
}
*/
