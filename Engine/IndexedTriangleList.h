#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "tiny_obj_loader.h"
#include <stdexcept>
#include <sstream>
#include "Miniball.h"

template <typename T>
struct IndexedTriangleList
{
	IndexedTriangleList() = default;
	IndexedTriangleList( std::vector<T> vertices_in, std::vector<size_t> indices_in )
		:
		vertices( std::move(vertices_in) ),
		indices( std::move(indices_in) )
	{
		assert( vertices.size() > 2 );
		assert( indices.size() % 3 == 0 );
	}
	// load an indexed triangle list from an object file
	static IndexedTriangleList<T> LoadFromFile( const std::string& filename )
	{
		IndexedTriangleList<T> itlist;

		// check if vertices are arranged clockwise
		bool isCCW = false;
		{
			std::ifstream file;
			std::string line;
			std::getline( file, line );
			std::transform( line.begin(), line.end(), line.begin(), std::tolower );
			if ( line.find("ccw") != std::string::npos )
			{
				isCCW = true;
			}
		}

		// resources fille by the loader
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::string err;

		// load obj file and save return val
		const bool returnVal = tinyobj::LoadObj( &attrib, &shapes, nullptr, &err, filename.c_str() );

		// check for errors
		if ( !err.empty() && err.substr(0, 4) != "WARN" )
		{
			throw std::runtime_error( "LoadObj returned error: " + err + "\nFile: " + filename.c_str() );
		}
		else if ( !returnVal )
		{
			throw std::runtime_error( "LoadObj returned false\nFile: " + filename );
		}
		else if ( shapes.size() == 0u )
		{
			throw std::runtime_error( "File " + filename + " contains no shapes" );
		}

		// extract vertices
		itlist.vertices.reserve( attrib.vertices.size() / 3u );
		for ( size_t i = 0; i < attrib.vertices.size(); i += 3 )
		{
			itlist.vertices.emplace_back( Vec3{
				attrib.vertices[i],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
			} );
		}

		// extract indices
		const auto& mesh = shapes[0].mesh;

		itlist.indices.reserve( mesh.indices.size() );
		// add one face at a time
		for ( size_t f = 0; f < mesh.num_face_vertices.size(); f++  )
		{
			// check if face has the right number of faces
			if ( mesh.num_face_vertices[f] != 3u )
			{
				std::ostringstream oss;
				oss << "Face #" << f << " has " << mesh.num_face_vertices[f] << " faces";
				throw std::runtime_error( oss.str().c_str() );
			}
			// add indices
			for ( size_t i = 0; i < 3u; i++ )
			{
				const auto idx = mesh.indices[f * 3u + i];
				itlist.indices.push_back( size_t( idx.vertex_index ) );
			}

			if ( isCCW )
			{
				// correct the order
				std::swap( itlist.indices.back(), *std::prev( itlist.indices.end(), 2 ) );
			}

		}
		return itlist;
	}
	// load indexed triangle list containing normals from an object file
	static IndexedTriangleList<T> LoadNormals( const std::string& filename )
	{
		IndexedTriangleList<T> itlist;

		// check if vertices are arranged clockwise
		bool isCCW = false;
		{
			std::ifstream file;
			std::string line;
			std::getline( file, line );
			std::transform( line.begin(), line.end(), line.begin(), std::tolower );
			if ( line.find("ccw") != std::string::npos )
			{
				isCCW = true;
			}
		}

		// resources fille by the loader
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::string err;

		// load obj file and save return val
		const bool returnVal = tinyobj::LoadObj( &attrib, &shapes, nullptr, &err, filename.c_str() );

		// check for errors
		if ( !err.empty() && err.substr(0, 4) != "WARN" )
		{
			throw std::runtime_error( "LoadObj returned error: " + err + "\nFile: " + filename.c_str() );
		}
		else if ( !returnVal )
		{
			throw std::runtime_error( "LoadObj returned false\nFile: " + filename );
		}
		else if ( shapes.size() == 0u )
		{
			throw std::runtime_error( "File " + filename + " contains no shapes" );
		}

		// extract vertices
		itlist.vertices.reserve( attrib.vertices.size() / 3u );
		for ( size_t i = 0; i < attrib.vertices.size(); i += 3 )
		{
			itlist.vertices.emplace_back( Vec3{
				attrib.vertices[i],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
			} );
		}

		// extract indices
		const auto& mesh = shapes[0].mesh;

		itlist.indices.reserve( mesh.indices.size() );
		// add one face at a time
		for ( size_t f = 0; f < mesh.num_face_vertices.size(); f++  )
		{
			// check if face has the right number of faces
			if ( mesh.num_face_vertices[f] != 3u )
			{
				std::ostringstream oss;
				oss << "Face #" << f << " has " << mesh.num_face_vertices[f] << " faces";
				throw std::runtime_error( oss.str().c_str() );
			}
			// add indices and normals
			for ( size_t i = 0; i < 3u; i++ )
			{
				const auto idx = mesh.indices[f * 3u + i];
				itlist.indices.push_back( size_t( idx.vertex_index ) );

				itlist.vertices[(size_t)idx.vertex_index].n = Vec3{
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				};
			}

			if ( isCCW )
			{
				// correct the order
				std::swap( itlist.indices.back(), *std::prev( itlist.indices.end(), 2 ) );
			}

		}
		return itlist;
	}
	
	// 
	void AdjustToTrueCenter()
	{
		struct VertexAccessor
		{
			typedef std::vector<T>::const_iterator Pit;
			typedef const float* Cit;
			Cit operator()( Pit it ) const
			{
				return &(it->pos.x);
			}
		};

		Miniball::Miniball<VertexAccessor> mb( 3, vertices.cbegin(), vertices.cend() );
		
		const auto pc = mb.center();
		const Vec3 center = Vec3{ pc[0], pc[1], pc[2] };

		for ( auto& v : vertices )
		{
			v -= center;
		}
	}

	float GetRadius() const
	{
		// return the distance of the farthest vertex from origin
		return std::max_element( vertices.cbegin(), vertices.cend(), 
			[]( const T& v0, const T& v1  ) {
				return v0.pos.LenSq() < v1.pos.LenSq();
			}
		)->pos.Len();
	}

	std::vector<T> vertices;
	std::vector<size_t> indices;
};