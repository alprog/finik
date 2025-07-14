export module MaterialManager;

import Finik.Core;
import ConstantBuffer;
import Material;

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
