export module ConsoleView;

import View;

export class ConsoleView : public View
{
public:
    ConsoleView(const char* name);

protected:
    void draw_content() override;
};
