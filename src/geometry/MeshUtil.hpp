#pragma once
#include <vector>

#include <rply/rply.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToMesh.h>

inline int vertexCallback(p_ply_argument argument) {
	long coordIndex;
	void* userData;
	ply_get_argument_user_data(argument, &userData, &coordIndex);

	auto* points = reinterpret_cast<std::vector<openvdb::Vec3s>*>(userData);
	double value = ply_get_argument_value(argument);

	if (coordIndex % 3 == 0) {
		points->emplace_back();
	}

	(*points).back()[coordIndex % 3] = static_cast<float>(value);

	return 1;
}

// Callback for face
inline int faceCallback(p_ply_argument argument) {
	long dummy;
	void* userData;
	ply_get_argument_user_data(argument, &userData, &dummy);

	auto* triangles = reinterpret_cast<std::vector<openvdb::Vec3I>*>(userData);
	long length, valueIndex;
	ply_get_argument_property(argument, NULL, &length, &valueIndex);

	if (length == 3) { // Assuming each face is a triangle
		if (valueIndex == 0) {
			triangles->emplace_back();
		}
		if (valueIndex >= 0) {
			int value = static_cast<int>(ply_get_argument_value(argument));
			(*triangles).back()[valueIndex] = value;
		}
	}

	return 1;
}


// read ply file as openvdb::Vec3s and openvdb::Vec3I
inline std::pair< std::vector<openvdb::Vec3s>, std::vector<openvdb::Vec3I>> ReadPLYasOpenvdb(const std::string& path)
{
	std::pair< std::vector<openvdb::Vec3s>, std::vector<openvdb::Vec3I>> mesh;


	p_ply ply = ply_open(path.c_str(), NULL, 0, NULL);
	if (!ply)
	{
		std::cerr << "Failed to open " << path << std::endl;
		return mesh;
	}

	if (!ply_read_header(ply))
	{
		std::cerr << "Failed to read header of " << path << std::endl;
		return mesh;
	}

	long nvertices = ply_set_read_cb(ply, "vertex", "x", vertexCallback, &mesh.first, 0);
	ply_set_read_cb(ply, "vertex", "y", vertexCallback, &mesh.first, 1);
	ply_set_read_cb(ply, "vertex", "z", vertexCallback, &mesh.first, 2);

	long ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", faceCallback, &mesh.second, 0);

	if (!ply_read(ply))
	{
		std::cerr << "Failed to read " << path << std::endl;
		return mesh;
	}

	ply_close(ply);

	std::cout << "Read " << nvertices << " vertices and " << ntriangles << " triangles from " << path << std::endl;

	return mesh;
}

inline openvdb::FloatGrid ConvertMeshToLevelSet(const std::pair< std::vector<openvdb::Vec3s>, std::vector<openvdb::Vec3I>>& mesh, float voxelSize)
{

	// get vector of points and triangles from mesh
	const std::vector<openvdb::Vec3s> points = mesh.first;
	const std::vector<openvdb::Vec3I> triangles = mesh.second;

	// Create a linear transform.
	auto transform = openvdb::math::Transform::createLinearTransform(voxelSize);

	// convert mesh to volume
	openvdb::FloatGrid::Ptr grid = openvdb::tools::meshToLevelSet<openvdb::FloatGrid>(*transform, points, triangles, voxelSize);

	return *grid;
}


