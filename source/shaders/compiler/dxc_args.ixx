export module dxc_args;

import Finik.Core;

export struct DxcArgs
{
    int32 count() const { return arguments.count(); }

    DxcArgs& operator<<(const String& str)
    {
        arguments.append(std::wstring(str.begin(), str.end()));
        return *this;
    }

    auto getPtrToPtrs()
    {
        pointers.clear();
        for (auto& argument : arguments)
        {
            pointers.append(argument.data());
        }
        return &pointers[0];
    }

private:
    Array<std::wstring> arguments;
    Array<const wchar_t*> pointers;
};