export module Path;

import std;

export class Path
{
    friend class std::hash<Path>;

public:
    Path();

    Path(const char* pathCString);
    Path(const std::string& pathString);
    Path(const std::filesystem::path fs_path);

    static std::string getNormalized(std::string pathString);
    static bool isAbsolute(const std::string& pathString);

    static Path combine(const std::string lhs, const std::string rhs);

    void append(const std::string pathString);
    void cd(const Path pathString);
    void cdUp();

    friend Path operator+(const Path& lhs, const Path& rhs);
    Path& operator+=(const Path& rhs);

    friend Path operator/(const Path& lhs, const Path& rhs);
    Path& operator/=(const Path& rhs);

    Path getParentPath() const;

    std::string getName() const;
    std::string getExtension() const;
    std::string getNameWithoutExtension() const;

    bool isEmpty() const;
    bool isAbsolute() const;
    bool isRelative() const;

    Path getRelativeTo(Path basePath) const;

    static bool isEqual(const Path& path1, const Path& path2, bool caseSensitive = true);

    inline operator std::string&()
    {
        return normal_form;
    }

    inline operator const std::string&() const
    {
        return normal_form;
    }

    inline std::string str() const
    {
        return normal_form;
    }

    inline const char* c_str() const
    {
        return normal_form.c_str();
    }

    std::strong_ordering operator<=>(const Path& other) const = default;

private:
    static void fixSlashes(std::string& pathString);
    static void applyDots(std::string& pathString);

    std::string normal_form;
};

export template<>
struct std::hash<Path>
{
    size_t operator()(const Path& path) const
    {
        return std::hash<std::string>()(path.normal_form);
    }
};
