using System.Diagnostics;
using LiftControl.Generation1;
using LiftControl.UnitTests;

namespace LiftControl.Console.UnitTests;

public class Generation1Tests
{
    [MemberData(memberName: nameof(ScheduleTestResources.GetSingleTaskTestData), MemberType = typeof(ScheduleTestResources))]
    [Theory]
    public void TestSingleTask_Managed(int atFloor, IEnumerable<(int fromFloor, int toFloor)> tasks, IEnumerable<int> schedule)
    {

    }

    private static void VerifyOutput(bool managed, int atFloor, IEnumerable<(int fromFloor, int toFloor)> tasks, IEnumerable<int> schedule)
    {
        string output = StartupProcess(managed, 1, InputSettings);
        var schedule = output.Split();

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

    private static string StartupProcess(bool managed, int generation, Action<StreamWriter> callback)
    {
        Debug.Assert(generation >= 1);

        const string managedExecutionFileName = "mlc";
        const string unmanagedExecutionFileName = "umlc";

        var process = new Process
        {
            StartInfo = new()
            {
                FileName = managed ? managedExecutionFileName : unmanagedExecutionFileName,
                ArgumentList =
                {
                    $"/g:{generation}"
                },
                UseShellExecute = false,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true,
                WindowStyle = ProcessWindowStyle.Hidden
            }
        };
        process.Start();

        callback(process.StandardInput);
        var result = process.StandardOutput.ReadToEnd();

        process.WaitForExit(TimeSpan.FromSeconds(10));
        return result;
    }
}