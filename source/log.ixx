export module log;

import Finik.Core;

export void log_impl(std::string text);

export template <typename... Args>
void log(std::format_string<Args...> fmt, Args&&... args)
{
    log_impl(std::format(fmt, std::forward<Args>(args)...));
}