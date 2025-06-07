export module msaa;

export enum class MSAA : char
{
    Off = 0,
    x2 = 1,
    x4 = 2,
    x8 = 3
};

export int getSampleCount(MSAA msaa)
{
    switch (msaa)
    {
    case MSAA::x2:
        return 2;
    case MSAA::x4:
        return 4;
    case MSAA::x8:
        return 8;

    case MSAA::Off:
    default:
        return 1;
    }
}
