using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using LiftTask = (int floor, LiftControl.Lift.Direction direction);

namespace LiftControl;

internal class Lift(int atFloor = 0)
{
    private int _atFloor = atFloor;
    private Direction? _direction;
    private Queue<SortedSet<int>>? _schedule;

#if DEBUG
    public int AtFloor => this._atFloor;

    public IEnumerable<LiftTask> GetSchedule()
    {
        if (!this._direction.HasValue) yield break;

        this.CheckStatus();

        int? previous = null;
        foreach (var pass in this._schedule)
        {
            foreach (var floor in pass)
            {
                if (!previous.HasValue)
                {
                    previous = floor;
                    continue;
                }
                else if (previous.Value == floor)
                {
                    continue;
                }
                yield return (previous.Value, GetDirection(previous.Value, floor));
                previous = floor;
            }
        }
        Debug.Assert(previous.HasValue);
        yield return (previous.Value, default);
    }
#endif

    public enum Direction { Up = 1, Down = -1 }

    private sealed class FloorComparer : IComparer<int>
    {
        private readonly Direction _direction;

        private FloorComparer(Direction direction) => this._direction = direction;

        public static FloorComparer Up { get; } = new(Direction.Up);
        public static FloorComparer Down { get; } = new(Direction.Down);

        public int Compare(int x, int y) => x.CompareTo(y) * (int)this._direction;

        public FloorComparer Reverse() => GetInstance((Direction)(-(int)this._direction));

        public static FloorComparer GetInstance(Direction direction) => direction switch
        {
            Direction.Up => Up,
            Direction.Down => Down,
            _ => throw new ArgumentOutOfRangeException(nameof(direction)),
        };
    }

    [MemberNotNull(nameof(_direction), nameof(_schedule))]
    private void CheckStatus()
    {
        Debug.Assert(this._direction.HasValue);
        Debug.Assert(this._schedule is not null);
    }

    public void Add(int fromFloor, int toFloor)
    {
        var guestDirection = GetDirection(fromFloor, toFloor);
        if (!this._direction.HasValue)
        {
            Initialize();
        }

        [MemberNotNull(nameof(_direction), nameof(_schedule))]
        void Initialize()
        {
            this._direction = this._atFloor == fromFloor ? guestDirection : GetDirection(this._atFloor, fromFloor);
            this._schedule = new(4);
            var comparer = FloorComparer.GetInstance(this._direction.Value);
            var reversedComparer = comparer.Reverse();
            this._schedule.Enqueue(new(comparer));
            this._schedule.Enqueue(new(reversedComparer));
            this._schedule.Enqueue(new(comparer));
        }

        this.CheckStatus();

        if (this._direction == guestDirection)
        {
            if (this._atFloor == fromFloor || this._direction == GetDirection(this._atFloor, fromFloor))
            {
                this._schedule.ElementAt(0).Add(fromFloor);
                this._schedule.ElementAt(0).Add(toFloor);
            }
            else
            {
                Debug.Assert(this._direction == GetDirection(fromFloor, this._atFloor));
                this._schedule.ElementAt(2).Add(fromFloor);
                this._schedule.ElementAt(2).Add(toFloor);
            }
        }
        else
        {
            Debug.Assert(this._direction != guestDirection);
            this._schedule.ElementAt(1).Add(fromFloor);
            this._schedule.ElementAt(1).Add(toFloor);
        }
    }

    [DebuggerStepThrough]
    internal static Direction GetDirection(int fromFloor, int toFloor)
    {
        Debug.Assert(fromFloor != toFloor);
        return fromFloor < toFloor ? Direction.Up : Direction.Down;
    }
}
