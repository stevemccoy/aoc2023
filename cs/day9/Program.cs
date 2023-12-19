namespace Day9
{
    class Program
    {
        public static List<List<int>> Histories = new();

        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            Part1("test9.txt");
            Part1("input9.txt");
            Part2("test9.txt");
            Part2("input9.txt");
            Console.WriteLine("That's all folks!");
        }

        static void Part1(String fileName)
        {
            Setup();
            Console.WriteLine("Part 1.");
            ProcessInputFile(fileName);
            int count = 0;
            foreach (var h in Histories) {
                var n = NextValue(h);
                count += n;
            }
            Console.WriteLine("Count = " + count);
        }

        static void Part2(String fileName) {
            Setup();
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);
            int count = 0;
            foreach (var h in Histories) {
                var n = PrevValue(h);
                count += n;
            }
            Console.WriteLine("Count = " + count);
        }

        static void Setup()
        {
            Histories.Clear();
        }

        private static int NextValue(List<int> history) {
            List<List<int>> diffs = new();
            diffs.Add(history);
            int j = 0;
            bool done = false;
            // Analyse differences.
            while (!done) {
                List<int> nextLine = new();
                for (int i = 1; i < diffs[j].Count; i++) {
                    nextLine.Add(diffs[j][i] - diffs[j][i-1]);
                }
                done = nextLine.All(v => v == 0);
                diffs.Add(nextLine);
                j++;
            }
            // Project next value.
            int v = 0;
            for (int k = diffs.Count - 1; k >= 0; k--) {
                v += diffs[k].Last();
                diffs[k].Add(v);
            }
            v = diffs[0].Last();
            return v;
        }

        private static int PrevValue(List<int> history) {
            List<List<int>> diffs = new();
            diffs.Add(history);
            int j = 0;
            bool done = false;
            // Analyse differences.
            while (!done) {
                List<int> nextLine = new();
                for (int i = 1; i < diffs[j].Count; i++) {
                    nextLine.Add(diffs[j][i] - diffs[j][i-1]);
                }
                done = nextLine.All(v => v == 0);
                diffs.Add(nextLine);
                j++;
            }
            // Project previous value.
            int v = 0;
            for (int k = diffs.Count - 1; k >= 0; k--) {
                v = diffs[k].First() - v;
                diffs[k].Insert(0, v);
            }
            v = diffs[0].First();
            return v;
        }

        private static void ProcessInputFile(string fileName)
        {
            Console.WriteLine("Processing file " + fileName + " ...");
            ReadInputFile(fileName);
        }

        static void ReadInputFile(string fileName)
        {
            using StreamReader sr = new(fileName);
            if (sr != null)
            {
                while (!sr.EndOfStream)
                {
                    var line = sr?.ReadLine()?.Trim() ?? string.Empty;
                    if (line != string.Empty)
                    {
                        var history = line.Split(" ").Select(s => int.Parse(s));
                        Histories.Add(history.ToList());
                    }
                }
            }
        }
    }
}
