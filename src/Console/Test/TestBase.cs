using System.Diagnostics;

namespace LiftControl.Console.UnitTests;

public abstract class TestBase
{
    protected static string StartupProcess(bool managed, int generation, Action<StreamWriter> callback)
    {
        Debug.Assert(generation >= 1);

        const string managedExecutionFileName = "mlc.exe";
        const string unmanagedExecutionFileName = "umlc.exe";

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
