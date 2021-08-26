#include "Physics4D.h"
#include <iostream>

VisiblesImage::VisiblesImage(const std::vector<Visible*> visibles, Player player)
{
	this->player = player;
	for (Visible* pVisible : visibles)
	{
		switch (pVisible->id)
		{
		case Space3D::id:
			this->visibles.push_back(new Space3D{ *static_cast<Space3D*>(pVisible) });
			break;
		case Tetrahedron::id:
			this->visibles.push_back(new Tetrahedron{ *static_cast<Tetrahedron*>(pVisible) });
			break;
		case Mesh::id:
			this->visibles.push_back(new Mesh{ *static_cast<Mesh*>(pVisible) });
			break;
		default:
			throw std::exception{ "Unknown id" };
			break;
		}
	}
}
VisiblesImage::~VisiblesImage()
{
	for (Visible* pVisible : visibles)
	{
		switch (pVisible->id)
		{
		case Space3D::id:
			delete static_cast<Space3D*>(pVisible);
			break;
		case Tetrahedron::id:
			delete static_cast<Tetrahedron*>(pVisible);
			break;
		case Mesh::id:
			delete static_cast<Mesh*>(pVisible);
			break;
		}
	}
}
