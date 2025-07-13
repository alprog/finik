export module material_manager;

import Finik.Core;
import constant_buffer;
import material;

export using GpuMaterialsConstantBuffer = GpuBuffer<MaterialsConstantBuffer>;

export class MaterialManager
{
public:
    static MaterialManager& GetInstance()
    {
        static MaterialManager instance;
        return instance;
    }

    MaterialManager();

    int32 Register(Material* material);
    void Unregister(Material* material);

    void WriteToBuffer(Material* material);

    Array<Material*> Materials;
    GpuMaterialsConstantBuffer* ConstantBuffer;
};