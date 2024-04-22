using System.ComponentModel.DataAnnotations;
using System.Diagnostics;

namespace LiftControl
{
    internal static class Program
    {
        private const int HostErrorCode = 24397;

        private static int Main(object[] args)
        {
            if (args.Length != 0 && args[0] is string gArg &&
                gArg.StartsWith("/g:") && int.TryParse(gArg[3..], out var g))
            {
                switch (g)
                {
                    case 1: return RunGeneration1();
                }
            }

            Console.WriteLine("expected argument /g:< generation_num >.");
            return HostErrorCode;
        }

        private static int RunGeneration1()
        {
            string? line;

            line = Console.ReadLine();
            if (!int.TryParse(line, out var atFloor))
            {
                Debug.Fail($"wrong format '{line}' for lift initial position.");
                return HostErrorCode;
            }

            var lift = new LiftControl.Generation1.Lift(atFloor);

            while (true)
            {
                line = Console.ReadLine();
                if (string.IsNullOrWhiteSpace(line))
                {
                    break;
                }

                var args = (from arg in line.Split() where !string.IsNullOrEmpty(arg) select arg).ToArray();
                if (args.Length == 2 &&
                    int.TryParse(args[0], out var fromFloor) &&
                    int.TryParse(args[1], out var toFloor))
                {
                    lift.Add(fromFloor, toFloor);
                }
                else
                {
                    Debug.Fail($"wrong format {line} for guest task.");
                    return HostErrorCode;
                }
            }

            Console.WriteLine(string.Join(' ', lift.GetSchedule().Select(t => t.floor)));
        }
    }
}
