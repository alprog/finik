export module View;

export class View
{
public:
    View(const char* name);

    virtual void update(float deltaTime) {}
    void draw();

protected:
    virtual void draw_content() = 0;

    const char* name;
};
