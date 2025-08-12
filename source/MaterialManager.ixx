export module MaterialManager;

import Finik.Core;
import ConstantBuffer;
import Material;

export using GpuMaterialsConstantBuffer = GpuBuffer<MaterialsConstantBuffer>;

export class MaterialManager : public Singleton<MaterialManager>
{
public:
    MaterialManager();

    int32 Register(Material* material);
    void Unregister(Material* material);

    void WriteToBuffer(Material* material);

    Array<Material*> Materials;
    GpuMaterialsConstantBuffer* ConstantBuffer;
};
