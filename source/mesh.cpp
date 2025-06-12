module mesh;

import mesh_builder;

Mesh* createCubeMesh()
{
    MeshBuilder builder;

    auto add = [&builder](Vector3 of1, Vector3 of2, Vector3 normal, Vector2 uv) {
        StandardVertex a{-of1 - of2 + normal, normal, uv};
        StandardVertex b{of1 - of2 + normal, normal, uv + Vector2(0.33f, 0.0f)};
        StandardVertex c{of1 + of2 + normal, normal, uv + Vector2(0.33f, 0.5f)};
        StandardVertex d{-of1 + of2 + normal, normal, uv + Vector2(0, 0.5f)};
        builder.addQuad(a, b, c, d);
    };

    add(Vector3::Left, Vector3::Down, Vector3::Forward, Vector2(0, 0));
    add(Vector3::Forward, Vector3::Down, Vector3::Right, Vector2(0.33f, 0));
    add(Vector3::Left, Vector3::Forward, Vector3::Up, Vector2(0.66f, 0));

    add(Vector3::Right, Vector3::Down, Vector3::Backward, Vector2(0, 0.5f));
    add(Vector3::Backward, Vector3::Down, Vector3::Left, Vector2(0.33f, 0.5f));
    add(Vector3::Right, Vector3::Forward, Vector3::Down, Vector2(0.66f, 0.5f));


    return builder.Build();
}

Mesh* createFullScreenQuad()
{
    MeshBuilder builder;

    // a       b
    //  +-----+
    //  | \   |
    //  |   \ |
    //  +-----+
    // d       c

    auto a = StandardVertex{Vector3(0, 0, 0), Vector3::Backward, Vector2(0, 0)};
    auto b = StandardVertex{Vector3(1, 0, 0), Vector3::Backward, Vector2(1, 0)};
    auto c = StandardVertex{Vector3(1, 1, 0), Vector3::Backward, Vector2(1, 1)};
    auto d = StandardVertex{Vector3(0, 1, 0), Vector3::Backward, Vector2(0, 1)};
    builder.addQuad(a, b, c, d);

    return builder.Build();
}
