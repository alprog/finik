module Grid;

import MeshBuilder;
import Images;
import Assets;
import EffectManager;

Grid::Grid()
{
    tileMap = new ::TileMap();
	mesh = CreateMesh();

	auto cellTexture = Assets::GetInstance().get<Texture>("textures/cell.png");

	material = new Material("grid_material");
	material->Textures.append(cellTexture);
	material->Textures.append(tileMap->texture);
	material->Effect = EffectManager::GetInstance().get("grid");
    material->ShadowEffect = EffectManager::GetInstance().getShadowEffect(*material->Effect);
	material->RefreshBuffer();
}

Mesh* Grid::CreateMesh()
{
	MeshBuilder builder;

	int width = tileMap->image->width;
    int heigth = tileMap->image->height;

	for (int y = 0; y < heigth; y++)
	{
		for (int x = 0; x < width; x++)
		{
			builder.AddTile(x, y);
		}
	}

	return builder.Build();
}
