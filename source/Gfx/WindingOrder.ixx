export module WindingOrder;

export enum class WindingOrder
{
    CW = 0,
    CCW = 1
};

export WindingOrder reverse(WindingOrder order)
{
    return order == WindingOrder::CW ? WindingOrder::CCW : WindingOrder::CW;
}