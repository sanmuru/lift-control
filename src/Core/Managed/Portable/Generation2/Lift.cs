namespace LiftControl.Generation2;

using System.Diagnostics;
using LogEntry = (uint time, int floor, Status status);

public class Lift(int atFloor = 0) : Generation1.Lift(atFloor)
{
    private Status _status;
    private uint _offset;

    public override void Add(int fromFloor, int toFloor)
    {
        base.Add(fromFloor, toFloor);
    }

    public IEnumerable<LogEntry> TimePassing(uint time)
    {
        uint fullTime = time;
        while (time != 0)
        {
            static Status CombineSpeedAndDirection(int speed, Direction d)
            {
                Debug.Assert(d != Direction.None);
                return (speed, d) switch
                {
                    ( < 0, Direction.Up) => Status.DEUP,
                    (0, Direction.Up) => Status.UNUP,
                    ( > 0, Direction.Up) => Status.ACUP,
                    ( < 0, Direction.Down) => Status.DEDN,
                    (0, Direction.Down) => Status.UNDN,
                    ( > 0, Direction.Down) => Status.ACDN,

                    _ => throw new InvalidOperationException()
                };
            }

            switch (this._status)
            {
                case Status.IDLE:
                    {
                        if (!this.GetSchedule().Any()) // no schedule
                            yield break;
                        else
                        {
                            var nextFloor = this.GetSchedule().First();
                            if (this._atFloor == nextFloor)
                            {
                                this._status = Status.OPDR;
                            }
                            else
                            {
                                this._status = CombineSpeedAndDirection(1, LiftUtilities.GetDirection(this._atFloor, nextFloor));
                            }
                            this._offset = 0;
                            yield return (fullTime - time, this._atFloor, this._status);
                        }
                    }
                    break;

                case Status.ACUP:
                case Status.ACDN:
                    {
                        Debug.Assert(this._offset < 4);
                        if (time < 4 - this._offset)
                        {
                            this._offset += time;
                            time = 0;
                        }
                        else
                        {
                            var nextFloor = this.GetSchedule().First();
                            var nextDirection = LiftUtilities.GetDirection(this._atFloor, nextFloor);
                            this._atFloor += (int)nextDirection;
                            time -= 4 - this._offset;
                            this._status = time == 0 ? CombineSpeedAndDirection(-1, nextDirection) : CombineSpeedAndDirection(0, nextDirection);
                            this._offset = 0;
                            yield return (fullTime - time, this._atFloor, this._status);
                        }
                    }
                    break;

                case Status.UNUP:
                case Status.UNDN:
                    {
                        var nextFloor = this.GetSchedule().First();
                        var nextDirection = LiftUtilities.GetDirection(this._atFloor, nextFloor);
                        uint unTime = 2 * (uint)Math.Abs(nextFloor - this._atFloor) - this._offset;
                        if (time < unTime)
                        {
                            var (f, r) = Math.DivRem(time + this._offset, 2);
                            this._atFloor += (int)nextDirection * (int)f;
                            this._offset = r;
                        }
                        else
                        {
                            this._atFloor = nextFloor;
                            time -= unTime;
                            this._status = CombineSpeedAndDirection(-1, nextDirection);
                            this._offset = 0;
                            yield return (fullTime - time, this._atFloor, this._status);
                        }
                    }
                    break;

                case Status.DEUP:
                case Status.DEDN:
                    {
                        Debug.Assert(this._offset < 4);
                        if (time < 4 - this._offset)
                        {
                            this._offset += time;
                            time = 0;
                        }
                        else
                        {
                            time -= 4 - this._offset;
                            this._status = Status.OPDR;
                            this._offset = 0;
                            yield return (fullTime - time, this._atFloor, this._status);
                        }
                    }
                    break;

            }
        }
    }

    private void NextSchedule()
    {
        this.CheckStatus();

        int? nextFloor = this.GetSchedule().FirstOrDefault();
        if (!nextFloor.HasValue) return;

        if (this._schedule.Peek().Count == 0)
        {
            
        }
        else
        {
            Debug.Assert(this._schedule.Peek().Contains(nextFloor.Value));
            if (this._schedule.Peek().Count > 1)
            {
                this._schedule.Peek().Remove(nextFloor.Value);
                return;
            }
        }
    }
}
