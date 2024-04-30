using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

namespace LiftControl.Generation1;

public class Lift(int atFloor = 0)
{
    protected int _atFloor = atFloor;
    protected Direction _direction;
    protected Queue<SortedSet<int>>? _schedule;

#if DEBUG
    public int AtFloor => this._atFloor;

    public IEnumerable<int> GetSchedule()
    {
        if (this._direction == Direction.None) yield break;

        this.CheckStatus();

        foreach (var pass in this._schedule)
        {
            foreach (var floor in pass)
            {
                yield return floor;
            }
        }
    }
#endif

    [MemberNotNull(nameof(_direction), nameof(_schedule))]
    protected void CheckStatus()
    {
        Debug.Assert(this._direction != Direction.None);
        Debug.Assert(this._schedule is not null);
    }

    public virtual void Add(int fromFloor, int toFloor)
    {
        var guestDirection = LiftUtilities.GetDirection(fromFloor, toFloor);
        if (this._direction == Direction.None)
        {
            Initialize();
        }

        [MemberNotNull(nameof(_direction), nameof(_schedule))]
        void Initialize()
        {
            this._direction = this._atFloor == fromFloor ? guestDirection : LiftUtilities.GetDirection(this._atFloor, fromFloor);
            this._schedule = new(4);
            var comparer = FloorComparer.GetInstance(this._direction);
            var reversedComparer = comparer.Reverse();
            this._schedule.Enqueue(new(comparer));
            this._schedule.Enqueue(new(reversedComparer));
            this._schedule.Enqueue(new(comparer));
        }

        this.CheckStatus();

        if (this._direction == guestDirection)
        {
            if (this._atFloor == fromFloor || this._direction == LiftUtilities.GetDirection(this._atFloor, fromFloor))
            {
                this._schedule.ElementAt(0).Add(fromFloor);
                this._schedule.ElementAt(0).Add(toFloor);
            }
            else
            {
                Debug.Assert(this._direction == LiftUtilities.GetDirection(fromFloor, this._atFloor));
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
}
