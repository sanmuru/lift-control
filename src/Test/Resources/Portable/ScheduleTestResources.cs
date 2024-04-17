using System.Diagnostics;
using GuestTask = (int fromFloor, int toFloor);

namespace LiftControl.UnitTests;

public static class ScheduleTestResources
{
    public static IEnumerable<object[]> GetSingleTaskTestData()
    {
        yield return new object[] { 1, new GuestTask[] { (1, 5) }, new int[] { 1, 5 } }; // at 1, from 1 to 5.
        yield return new object[] { 1, new GuestTask[] { (2, 5) }, new int[] { 2, 5 } }; // at 1, from 2 to 5.
        yield return new object[] { 1, new GuestTask[] { (5, 2) }, new int[] { 5, 2 } }; // at 1, from 5 to 2.
        yield return new object[] { 1, new GuestTask[] { (5, 1) }, new int[] { 5, 1 } }; // at 1, from 5 to 1.
        yield return new object[] { 3, new GuestTask[] { (1, 5) }, new int[] { 1, 5 } }; // at 3, from 1 to 5.
        yield return new object[] { 3, new GuestTask[] { (5, 1) }, new int[] { 5, 1 } }; // at 3, from 5 to 1.
        yield return new object[] { 5, new GuestTask[] { (5, 1) }, new int[] { 5, 1 } }; // at 5, from 5 to 1.
        yield return new object[] { 5, new GuestTask[] { (3, 1) }, new int[] { 3, 1 } }; // at 5, from 3 to 1.
        yield return new object[] { 5, new GuestTask[] { (1, 3) }, new int[] { 1, 3 } }; // at 5, from 1 to 3.
        yield return new object[] { 5, new GuestTask[] { (1, 5) }, new int[] { 1, 5 } }; // at 5, from 1 to 5.
    }

    public static IEnumerable<object[]> GetDoubleTasksTestData()
    {
        yield return new object[] {1, new GuestTask[] { (1, 2), (4, 5) }, new int[] { 1, 2, 4, 5 } }; // at 1, get A, from 1 to 2, left A, from 2 to 4, get B, from 4 to 5, left B.
        yield return new object[] {1, new GuestTask[] { (1, 4), (2, 5) }, new int[] { 1, 2, 4, 5 } }; // at 1, get A, from 1 to 2, get B, from 2 to 4, left A, from 4 to 5, left B.
        yield return new object[] {1, new GuestTask[] { (1, 5), (2, 4) }, new int[] { 1, 2, 4, 5 } }; // at 1, get A, from 1 to 2, get B, from 2 to 4, left B, from 4 to 5, left A.
        yield return new object[] {1, new GuestTask[] { (4, 5), (1, 2) }, new int[] { 1, 2, 4, 5 } }; // at 1, get B, from 1 to 2, left B, from 2 to 4, get A, from 4 to 5, left A.
        yield return new object[] {1, new GuestTask[] { (2, 5), (1, 4) }, new int[] { 1, 2, 4, 5 } }; // at 1, get B, from 1 to 2, get A, from 2 to 4, left B, from 4 to 5, left A.
        yield return new object[] {1, new GuestTask[] { (2, 4), (1, 5) }, new int[] { 1, 2, 4, 5 } }; // at 1, get B, from 1 to 2, get A, from 2 to 4, left A, from 4 to 5, left B.

        yield return new object[] {5, new GuestTask[] { (5, 4), (2, 1) }, new int[] { 5, 4, 2, 1 } }; // at 5, get A, from 5 to 4, left A, from 4 to 2, get B, from 2 to 1, left B.
        yield return new object[] {5, new GuestTask[] { (5, 2), (4, 1) }, new int[] { 5, 4, 2, 1 } }; // at 5, get A, from 5 to 4, get B, from 4 to 2, left A, from 2 to 1, left B.
        yield return new object[] {5, new GuestTask[] { (4, 2), (5, 1) }, new int[] { 5, 4, 2, 1 } }; // at 5, get A, from 5 to 4, get B, from 4 to 2, left B, from 2 to 1, left A.
        yield return new object[] {5, new GuestTask[] { (2, 1), (5, 4) }, new int[] { 5, 4, 2, 1 } }; // at 5, get B, from 5 to 4, left B, from 4 to 2, get A, from 2 to 1, left A.
        yield return new object[] {5, new GuestTask[] { (4, 1), (5, 2) }, new int[] { 5, 4, 2, 1 } }; // at 5, get B, from 5 to 4, get A, from 4 to 2, left B, from 2 to 1, left A.
        yield return new object[] {5, new GuestTask[] { (5, 1), (4, 2) }, new int[] { 5, 4, 2, 1 } }; // at 5, get B, from 5 to 4, get A, from 4 to 2, left A, from 2 to 1, left B.

        yield return new object[] {3, new GuestTask[] { (2, 1), (4, 5) }, new int[] { 2, 1, 4, 5 } }; // at 3, from 3 to 2, get A, from 2 to 1, left A, from 1 to 4, get B, from 4 to 5, left B.
        yield return new object[] {3, new GuestTask[] { (2, 1), (5, 4) }, new int[] { 2, 1, 5, 4 } }; // at 3, from 3 to 2, get A, from 2 to 1, left A, from 1 to 5, get B, from 5 to 4, left B.
        yield return new object[] {3, new GuestTask[] { (1, 2), (4, 5) }, new int[] { 1, 2, 4, 5 } }; // at 3, from 3 to 1, get A, from 1 to 2, left A, from 2 to 4, get B, from 4 to 5, left B.
        yield return new object[] {3, new GuestTask[] { (1, 2), (5, 4) }, new int[] { 1, 2, 5, 4 } }; // at 3, from 3 to 1, get A, from 1 to 2, left A, from 2 to 5, get B, from 5 to 4, left B.
        yield return new object[] {3, new GuestTask[] { (4, 5), (2, 1) }, new int[] { 4, 5, 2, 1 } }; // at 3, from 3 to 4, get A, from 4 to 5, left A, from 5 to 2, get B, from 2 to 1, left B.
        yield return new object[] {3, new GuestTask[] { (4, 5), (1, 2) }, new int[] { 4, 5, 1, 2 } }; // at 3, from 3 to 4, get A, from 4 to 5, left A, from 5 to 1, get B, from 1 to 2, left B.
        yield return new object[] {3, new GuestTask[] { (5, 4), (2, 1) }, new int[] { 5, 4, 2, 1 } }; // at 3, from 3 to 5, get A, from 5 to 4, left A, from 4 to 2, get B, from 2 to 1, left B.
        yield return new object[] {3, new GuestTask[] { (5, 4), (1, 2) }, new int[] { 5, 4, 1, 2 } }; // at 3, from 3 to 5, get A, from 5 to 4, left A, from 4 to 1, get B, from 1 to 2, left B.

        yield return new object[] {3, new GuestTask[] { (2, 5), (4, 1) }, new int[] { 2, 5, 4, 1 } }; // at 3, from 3 to 2, get A, from 2 to 5, left A, from 5 to 4, get B, from 4 to 1, left B.
        yield return new object[] {3, new GuestTask[] { (2, 5), (1, 4) }, new int[] { 1, 2, 4, 5 } }; // at 3, from 3 to 1, get B, from 1 to 2, get A, from 2 to 4, left B, from 4 to 5, left A.
        yield return new object[] {3, new GuestTask[] { (5, 2), (4, 1) }, new int[] { 5, 4, 2, 1 } }; // at 3, from 3 to 5, get A, from 5 to 4, get B, from 4 to 2, left A, from 2 to 1, left B.
        yield return new object[] {3, new GuestTask[] { (5, 2), (1, 4) }, new int[] { 5, 2, 1, 4 } }; // at 3, from 3 to 5, get A, from 5 to 2, left A, from 2 to 1, get B, from 1 to 4, left B.
        yield return new object[] {3, new GuestTask[] { (4, 1), (2, 5) }, new int[] { 4, 1, 2, 5 } }; // at 3, from 3 to 4, get A, from 4 to 1, left A, from 1 to 2, get B, from 2 to 5, left B.
        yield return new object[] {3, new GuestTask[] { (4, 1), (5, 2) }, new int[] { 5, 4, 2, 1 } }; // at 3, from 3 to 5, get B, from 5 to 4, get A, from 4 to 2, left B, from 2 to 1, left A.
        yield return new object[] {3, new GuestTask[] { (1, 4), (2, 5) }, new int[] { 1, 2, 4, 5 } }; // at 3, from 3 to 1, get A, from 1 to 2, get B, from 2 to 4, left A, from 4 to 5, left B.
        yield return new object[] {3, new GuestTask[] { (1, 4), (5, 2) }, new int[] { 1, 4, 5, 2 } }; // at 3, from 3 to 1, get A, from 1 to 4, left A, from 4 to 5, get B, from 5 to 2, left B.

        yield return new object[] {3, new GuestTask[] { (2, 4), (5, 1) }, new int[] { 2, 4, 5, 1 } }; // at 3, from 3 to 2, get A, from 2 to 4, left A, from 4 to 5, get B, from 5 to 1, left B.
        yield return new object[] {3, new GuestTask[] { (2, 4), (1, 5) }, new int[] { 1, 2, 4, 5 } }; // at 3, from 3 to 1, get B, from 1 to 2, get A, from 2 to 4, left A, from 4 to 5, left B.
        yield return new object[] {3, new GuestTask[] { (4, 2), (5, 1) }, new int[] { 5, 4, 2, 1 } }; // at 3, from 3 to 5, get B, from 5 to 4, get A, from 4 to 2, left A, from 2 to 1, left B.
        yield return new object[] {3, new GuestTask[] { (4, 2), (1, 5) }, new int[] { 4, 2, 1, 5 } }; // at 3, from 3 to 4, get A, from 4 to 2, left A, from 2 to 1, get B, from 1 to 5, left B.
        yield return new object[] {3, new GuestTask[] { (1, 5), (4, 2) }, new int[] { 1, 5, 4, 2 } }; // at 3, from 3 to 1, get A, from 1 to 5, left A, from 5 to 4, get B, from 4 to 2, left B.
        yield return new object[] {3, new GuestTask[] { (1, 5), (2, 4) }, new int[] { 1, 2, 4, 5 } }; // at 3, from 3 to 1, get A, from 1 to 2, get B, from 2 to 4, left B, from 4 to 5, left A.
        yield return new object[] {3, new GuestTask[] { (5, 1), (4, 2) }, new int[] { 5, 4, 2, 1 } }; // at 3, from 3 to 5, get A, from 5 to 4, get B, from 4 to 2, left B, from 2 to 1, left A.
        yield return new object[] {3, new GuestTask[] { (5, 1), (2, 4) }, new int[] { 5, 1, 2, 4 } }; // at 3, from 3 to 5, get A, from 5 to 1, left A, from 1 to 2, get B, from 2 to 4, left B.
    }

    public static IEnumerable<object[]> GetRandomTaskTestData(int sampleCount, int minLevelCount, int maxLevelCount, int minGuestCount, int maxGuestCount)
    {
        Debug.Assert(sampleCount >= 0);
        Debug.Assert(minLevelCount >= 0);
        Debug.Assert(maxLevelCount >= 0);
        Debug.Assert(minLevelCount <= maxLevelCount);
        Debug.Assert(minGuestCount >= 0);
        Debug.Assert(maxLevelCount >= 0);
        Debug.Assert(minGuestCount <= maxGuestCount);

        for (int i = 0; i < sampleCount; i++)
        {
            var levelCount = Random.Shared.Next(minLevelCount, maxLevelCount + 1);
            var guestCount = Random.Shared.Next(minGuestCount, maxGuestCount + 1);

            var atFloor = Random.Shared.Next(levelCount) + 1;
            var tasks = new GuestTask[guestCount];
            for (int j = 0; j < guestCount; j++)
            {
                var fromFloor = Random.Shared.Next(levelCount) + 1;
                int toFloor;
                do { toFloor = Random.Shared.Next(levelCount) + 1; } while (fromFloor == toFloor);
                tasks[j] = (fromFloor, toFloor);
            }

            yield return new object[] { atFloor, tasks };
        }
    }
}
