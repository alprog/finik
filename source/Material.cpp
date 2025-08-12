module Material;

import MaterialManager;

Material::Material(String name)
    : Name{name}
{
    Index = MaterialManager::GetInstance().Register(this);
}

Material::~Material()
{
    MaterialManager::GetInstance().Unregister(this);
}

void Material::RefreshBuffer()
{
    MaterialManager::GetInstance().WriteToBuffer(this);
}
