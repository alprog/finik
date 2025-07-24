export module Finik.Core.Math:CardinalDirection;

export enum CardinalDirection
{
    East = 0,
    North = 1,
    West = 2,
    South = 3,

    Count
};

CardinalDirection TurnsToCardinalDirection(int turns)
{
    if (turns < 0)
    {
        turns = (turns % 4) + 4;
    }   
    return static_cast<CardinalDirection>(turns % 4);
}

CardinalDirection operator<<(CardinalDirection direction, int turns) // CCW
{
    return TurnsToCardinalDirection(static_cast<int>(direction) + turns);
}

CardinalDirection operator>>(CardinalDirection direction, int turns) // CW
{
    return TurnsToCardinalDirection(static_cast<int>(direction) - turns);
}