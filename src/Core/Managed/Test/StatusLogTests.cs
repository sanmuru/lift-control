global using GuestTaskWithTime = (int fromFloor, int toFloor, uint time);
global using StatusLogEntry = (uint time, int floor, LiftControl.Status status);

namespace LiftControl.UnitTests;

public class StatusLogTests
{
    [MemberData(memberName: nameof(ScheduleTestResources.GetSingleTaskTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestSingleTask(int atFloor, IEnumerable<GuestTaskWithTime> tasks, IEnumerable<StatusLogEntry> schedule)
    {
        VerifyStatusLog(atFloor, tasks, schedule);
    }

    private void VerifyStatusLog(int atFloor, IEnumerable<GuestTaskWithTime> tasks, IEnumerable<StatusLogEntry> schedule)
    {
        var lift = new Generation2.Lift(atFloor);
        var groups = tasks.GroupBy(task => task.time).Order();

        foreach (var group in groups)
        {

        }
    }
}
