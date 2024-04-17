global using GuestTask = (int fromFloor, int toFloor);
using LiftControl;
using System.Diagnostics;
using Xunit.Abstractions;

namespace LiftControl.UnitTests;

public class ScheduleTests
{
    private readonly ITestOutputHelper _output;

    public ScheduleTests(ITestOutputHelper output) => this._output = output;

    [MemberData(memberName: nameof(ScheduleTestResources.GetSingleTaskTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestSingleTask(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        var lift = new Lift(atFloor);
        VerifySchedule(lift, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetDoubleTasksTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestDoubleTasks(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        var lift = new Lift(atFloor);
        VerifySchedule(lift, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetRandomTaskTestData), parameters: [
        100,      // sampleCount
        100,     // minLevelCount
        500,    // maxLevelCount
        10,     // minGuestCount
        100      // maxGuestCount
        ], MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestMultipleTasks(int atFloor, IEnumerable<GuestTask> tasks)
    {
        var lift = new Lift(atFloor);
        VerifySchedule(lift, tasks);

#if false
        this._output.WriteLine($"At {atFloor}.");

        this._output.WriteLine("-------------------------------------");

        var lift = new Lift(atFloor);
        foreach (var (fromFloor, toFloor) in tasks)
        {
            lift.Add(fromFloor, toFloor);
            this._output.WriteLine($"{(fromFloor < toFloor ? "¡ü" : "¡ý")} From {fromFloor} to {toFloor}.");
        }

        this._output.WriteLine("=====================================");

        var previousLevel = atFloor;
        foreach (var nextLevel in lift.GetSchedule())
        {
            this._output.WriteLine($"{(previousLevel < nextLevel ? "¡ü Up" : "¡ý Down")} to {nextLevel}.");
            previousLevel = nextLevel;
        }
#endif
    }

    [Fact]
    public void TestMultipleTasks1()
    {
        var lift = new Lift(46);
        VerifySchedule(lift, [
            (8, 72),
            (66, 79),
            (41, 66),
            (99, 80),
            (67, 61),
            (73, 47),
            (70, 46),
            (60, 31),
            (46, 68),
            (55, 5)
        ], [8, 41, 46, 66, 68, 72, 79, 99, 80, 73, 70, 67, 61, 60, 55, 47, 46, 31, 5]);
    }

    [Fact]
    public void TestMultipleTasks2()
    {
        var lift = new Lift(69);
        VerifySchedule(lift, [
            (27, 50),
            (54, 89),
            (24, 10),
            (8, 27),
            (38, 30),
            (78, 48),
            (20, 81),
            (85, 67),
            (49, 77),
            (79, 60)
        ], [38, 30, 24, 10, 8, 20, 27, 49, 50, 54, 77, 81, 89, 85, 79, 78, 67, 60, 48]);
    }

    [Fact]
    public void TestMultipleTasks3()
    {
        var lift = new Lift(69);
        VerifySchedule(lift, [
            (16, 96),
            (78, 77),
            (83, 40),
            (91, 46),
            (32, 42),
            (11, 4),
            (54, 98),
            (73, 19),
            (98, 37),
            (78, 84)
        ], [11, 4, 16, 32, 42, 54, 78, 84, 96, 98, 91, 83, 78, 77, 73, 46, 40, 37, 19]);
    }

    private static void VerifySchedule(Lift lift, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        foreach (var (fromFloor, toFloor) in tasks)
        {
            lift.Add(fromFloor, toFloor);
        }
        Assert.Equal(schedule, lift.GetSchedule().Select(tuple => tuple.floor));
    }

    private static void VerifySchedule(Lift lift, IEnumerable<GuestTask> tasks)
    {
        var outList = new HashSet<GuestTask>(tasks);
        var inList = new HashSet<GuestTask>();

        foreach (var (fromFloor, toFloor) in tasks)
        {
            lift.Add(fromFloor, toFloor);
        }

        int previousFloor = lift.AtFloor;
        Lift.Direction previousDirection = default;
        int directionChanged = 0;
        foreach (var (floor, direction) in lift.GetSchedule())
        {
            var enter = (from task in outList
                         let guestDirection = Lift.GetDirection(task.fromFloor, task.toFloor)
                         let canEnter = guestDirection == direction
                         where task.fromFloor == floor && canEnter
                         select task).ToArray();
            var leave = (from task in inList where task.toFloor == floor select task).ToArray();
            Assert.True(enter.Length != 0 || leave.Length != 0); // otherwise the stop makes no sense.

            inList.ExceptWith(leave);
            inList.UnionWith(enter);
            outList.ExceptWith(enter);

            if (directionChanged == 0)
            {
                directionChanged = 1; // initialize.
            }
            else
            {
                if (previousDirection != default && direction != default && previousDirection != direction)
                {
                    directionChanged++;
                    Assert.False(directionChanged > 3); // too many passes, not optimistic.
                }
            }
            previousDirection = direction;
            previousFloor = floor;
        }

        Assert.Empty(outList);
        Assert.Empty(inList);
    }
}