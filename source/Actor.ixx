export module Actor;

import Finik.Core;
import Mesh;
import Material;
import RenderContext;
import Model;
import RenderPass;

export class Actor
{
public:
    Actor();

    Matrix transformMatrix;
    Matrix oldTransformMatrix;
    Ptr<Model> model;

    void render(RenderContext& context, RenderPass pass)
    {
        context.setModelMatrix(transformMatrix);
     
        if (model)
        {
            for (int i = 0; i < model->meshes.count(); i++)
            {
                context.setMaterial(*model->materials[i], pass);
                context.drawMesh(model->meshes[i]);
           }
        }
    }
};
