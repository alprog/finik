module Mesh;

import MeshBuilder;

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

    add(Vector3::Left, Vector3::Up, Vector3::Forward, Vector2(0, 0.5f));
    add(Vector3::Forward, Vector3::Up, Vector3::Right, Vector2(0.33f, 0.5f));
    add(Vector3::Left, Vector3::Forward, Vector3::Up, Vector2(0.66f, 0.5f));

    add(Vector3::Right, Vector3::Up, Vector3::Backward, Vector2(0, 0));
    add(Vector3::Backward, Vector3::Up, Vector3::Left, Vector2(0.33f, 0));
    add(Vector3::Right, Vector3::Forward, Vector3::Down, Vector2(0.66f, 0));


    return builder.Build();
}

Mesh* createFullScreenQuad()
{
    MeshBuilder builder;

    // d       c
    //  +-----+
    //  |   / |
    //  | /   |
    //  +-----+
    // a       b

    auto a = StandardVertex{Vector3(0, 0, 0), Vector3::Backward, Vector2(0, 0)};
    auto b = StandardVertex{Vector3(1, 0, 0), Vector3::Backward, Vector2(1, 0)};
    auto c = StandardVertex{Vector3(1, 1, 0), Vector3::Backward, Vector2(1, 1)};
    auto d = StandardVertex{Vector3(0, 1, 0), Vector3::Backward, Vector2(0, 1)};
    builder.addQuad(a, b, c, d);

    return builder.Build();
}

Mesh* createBodyQuad()
{
    MeshBuilder builder;

    float bodyHeight = 2.0f;
    auto a = StandardVertex{Vector3(0, +0.5f, bodyHeight), Vector3::Forward, Vector2(0, 0)};
    auto b = StandardVertex{Vector3(0, -0.5f, bodyHeight), Vector3::Forward, Vector2(1, 0)};
    auto c = StandardVertex{Vector3(0, -0.5f, 0), Vector3::Forward, Vector2(1, 1)};
    auto d = StandardVertex{Vector3(0, +0.5f, 0), Vector3::Forward, Vector2(0, 1)};
    builder.addQuad(a, b, c, d);

    return builder.Build();
}

Mesh* createBodyMesh()
{
    MeshBuilder builder;

    float bodyHeight = 1.5f;
    int32 segmentCount = 5;

    std::vector<StandardVertex> vertices;
    vertices.reserve((segmentCount + 1) * 2);

    float baseWidth = 0.25f;
    float offsets[6] = {0.25f, 0.35f, 0.42f, 0.44f, 0.3f, 0.25f};
    //float offsets[6] = {0.25f, 0.25f, 0.25f, 0.25f, 0.3f, 0.25f};

    for (int32 i = 0; i <= segmentCount; i++)
    {
        auto portion = static_cast<float>(i) / segmentCount;

        auto altitude = Vector3::Up * bodyHeight * portion;

        auto offset = offsets[i];

        auto texOffset = 0.5f * offset / baseWidth; 

        float V = 1.0f - portion;

        vertices.emplace_back(altitude + Vector3::Right * offset, Vector3::Forward, Vector2(0.5f - texOffset, V));
        vertices.emplace_back(altitude - Vector3::Right * offset, Vector3::Forward, Vector2(0.5f + texOffset, V));
    }

    for (int32 i = 0; i < segmentCount; i++)
    {
        auto* v = &vertices[i * 2];
        builder.addTriangle(v[0], v[2], v[1]);
        builder.addTriangle(v[1], v[2], v[3]);
    }

    return builder.Build();
}
