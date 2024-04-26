using LiftControl.UnitTests;
using GuestTask = (int fromFloor, int toFloor);

namespace LiftControl.Console.UnitTests;

public class Generation1Tests : TestBase
{
    [MemberData(memberName: nameof(ScheduleTestResources.GetSingleTaskTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestSingleTask_Managed(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        VerifyOutput(managed: true, atFloor, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetSingleTaskTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestSingleTask_Unmanaged(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        VerifyOutput(managed: false, atFloor, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetDoubleTasksTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestDoubleTasks_Managed(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        VerifyOutput(managed: true, atFloor, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetDoubleTasksTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestDoubleTasks_Unmanaged(int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int> schedule)
    {
        VerifyOutput(managed: false, atFloor, tasks, schedule);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetRandomTasksTestData), parameters: [
        100,    // sampleCount
        100,    // minLevelCount
        500,    // maxLevelCount
        10,     // minGuestCount
        100     // maxGuestCount
        ], MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestMultipleTasks_Managed(int atFloor, IEnumerable<GuestTask> tasks)
    {
        VerifyOutput(managed: true, atFloor, tasks);
    }

    [MemberData(memberName: nameof(ScheduleTestResources.GetRandomTasksTestData), parameters: [
        100,    // sampleCount
        100,    // minLevelCount
        500,    // maxLevelCount
        10,     // minGuestCount
        100     // maxGuestCount
        ], MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestMultipleTasks_Unmanaged(int atFloor, IEnumerable<GuestTask> tasks)
    {
        VerifyOutput(managed: false, atFloor, tasks);
    }

    private static void VerifyOutput(bool managed, int atFloor, IEnumerable<GuestTask> tasks, IEnumerable<int>? schedule = null)
    {
        string output = StartupProcess(managed, generation: 1, InputSettings);
        var actual = output.Split().Where(s => !string.IsNullOrEmpty(s)).Select(s =>
        {
            Assert.True(int.TryParse(s, out var i), $"Wrong output schedule format: \"{i}\"");
            return i;
        });
        ScheduleTestResources.VerifySchedule(schedule, actual, atFloor, tasks);

        void InputSettings(StreamWriter writer)
        {
            writer.WriteLine(atFloor);
            foreach (var (fromFloor, toFloor) in tasks)
            {
                writer.WriteLine("{0} {1}", fromFloor, toFloor);
            }
            writer.WriteLine();
        }
    }
}