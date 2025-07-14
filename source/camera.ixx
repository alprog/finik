export module Camera;

import Finik.Core;
import SceneSystemFwd;
import RenderSystemFwd;

// for intellisense

export class Camera
{
public:
    Camera();

    Vector3 getForward() const;

    void calcViewMatrix();
    void calcProjectionMatrix();

    Ray castRay(Vector2 ndcPoint) const;

public:
    Vector3 position;
    Vector3 lookAt;
    float FieldOfView;
    float OrthoSize;
    float FarPlane = 400.0f;
    float NearPlane = 0.1f;
    float AspectRatio = 1.0f;
    Vector2 Jitter;

    Matrix viewMatrix;
    Matrix projectionMatrix;
};
