global using GuestTask = (int fromFloor, int toFloor);
using LiftControl.Generation1;

namespace LiftControl.UnitTests;

public class ScheduleTests
{
    [MemberData(memberName: nameof(ScheduleTestResources.GetSingleTaskTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestSingleTask(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        VerifySchedule(atFloor, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetDoubleTasksTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestDoubleTasks(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        VerifySchedule(atFloor, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetRandomTasksTestData), parameters: [
        100,    // sampleCount
        100,    // minLevelCount
        500,    // maxLevelCount
        10,     // minGuestCount
        100     // maxGuestCount
        ], MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestMultipleTasks(int atFloor, IEnumerable<GuestTask> tasks)
    {
        VerifySchedule(atFloor, tasks);
    }

    private static void VerifySchedule(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int>? schedule = null)
    {
        var lift = new Lift(atFloor);
        foreach (var (fromFloor, toFloor) in tasks)
        {
            lift.Add(fromFloor, toFloor);
        }
        ScheduleTestResources.VerifySchedule(schedule, lift.GetSchedule(), atFloor, tasks);
    }
}