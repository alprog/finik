export module console_view;

import view;

export class ConsoleView : public View
{
public:
    ConsoleView(const char* name);

protected:
    void draw_content() override;
};