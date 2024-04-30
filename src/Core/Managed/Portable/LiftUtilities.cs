using System.Diagnostics;

namespace LiftControl;

public enum Direction { Up = 1, None = 0, Down = -1 }

internal sealed class FloorComparer : IComparer<int>
{
    private readonly Direction _direction;

    private FloorComparer(Direction direction) => _direction = direction;

    public static FloorComparer Up { get; } = new(Direction.Up);
    public static FloorComparer Down { get; } = new(Direction.Down);

    public int Compare(int x, int y) => x.CompareTo(y) * (int)_direction;

    public FloorComparer Reverse() => GetInstance((Direction)(-(int)_direction));

    public static FloorComparer GetInstance(Direction direction) => direction switch
    {
        Direction.Up => Up,
        Direction.Down => Down,
        _ => throw new ArgumentOutOfRangeException(nameof(direction)),
    };
}

public enum Status
{
    IDLE,
    ACUP, UNUP, DEUP,
    ACDN, UNDN, DEDN,
    OPDR, WAIT, CLDR
}

internal static class LiftUtilities
{
    [DebuggerStepThrough]
    internal static Direction GetDirection(int fromFloor, int toFloor)
    {
        Debug.Assert(fromFloor != toFloor);
        return fromFloor < toFloor ? Direction.Up : Direction.Down;
    }
}
