using System.Text.RegularExpressions;

namespace Day8
{
    internal class Node
    {
        public required string From { get; set; }
        public required string Left { get; set; }
        public required string Right { get; set; }
    }

    internal class Path
    {
        public required string Start { get; set; }
        public int LeadSteps { get; set; }
        public required string CycleStart { get; set; }
        public int CycleSteps { get; set; }
        public required Dictionary<int,string> EndPoints { get; set; }
    }

    internal class Cycle
    {
        public required string Start { get; set; }
        public required string Finish { get; set; }
        public long LeadSteps { get; set; }
        public long RepeatSteps { get; set; }
        public long FinishAt { get; set; }
    }

    class Program
    {
        static string Directions = string.Empty;
        static readonly Dictionary<string, Node> Nodes = [];

        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            Part1("test8a.txt");
            Part1("input8.txt");
            Part2("test8b.txt");
            Part2("input8.txt");
            Console.WriteLine("That's all folks!");
        }

        static void Part1(string fileName)
        {
            Setup();
            Console.WriteLine("Part 1.");
            ProcessInputFile(fileName);
            long count = NumStepsFromTo("AAA", "ZZZ");
            Console.WriteLine("Number of steps from start to finish = " + count);
        }

        static void Part2(string fileName)
        {
            Setup();
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);
            // This approach works only because there is the same number of lead steps into the 
            // first goal for each path found.
            var paths = AnalysePaths();
            long result = 1L;
            foreach (var p in paths) {
                result = lcm(result, p.CycleSteps);
            }
            Console.WriteLine("Big LCM of all paths = " + result);
        }

        private static long NumStepsFromTo(string fromNode, string toNode)
        {
            string node = fromNode;
            string directions = Directions;
            long count = 0;
            while (node != toNode) {
                char whereNow = directions[0];
                directions = directions[1..];
                if (directions == string.Empty) {
                    directions = Directions;
                }
                if (whereNow == 'L') {
                    node = Nodes[node].Left;
                    count++;
                }
                else if (whereNow == 'R') {
                    node = Nodes[node].Right;
                    count++;
                }
            }
            return count;
        }

        private static List<Path> AnalysePaths() {
            List<Path> result = [];
            var starts = StartPositions();
            var ends = GoalPositions();
            foreach (var s in starts) {
                string whereAt = s;
                Stack<char> directions = new([.. Directions.Reverse()]);
                long index = 1;
                List<string> seen = [s + "-" + index];
                Dictionary<int, string> endNodes = [];
                char whereNow;
                bool seenEndNode = false;
                while (true) {
                    while (!directions.TryPop(out whereNow)) {
                        directions = new([.. Directions.Reverse()]);
                        index = 0;
                    }
                    index++;
                    whereAt = (whereNow == 'L') ? Nodes[whereAt].Left : Nodes[whereAt].Right;
                    string entry = whereAt + "-" + index;
                    if (seenEndNode && seen.Contains(entry)) {     // What if no ends yet?
                        int leadCount = seen.IndexOf(entry);
                        Path path = new()
                        {
                            Start = seen[0],
                            CycleStart = entry,
                            LeadSteps = leadCount,
                            CycleSteps = seen.Count - leadCount,
                            EndPoints = endNodes
                        };
                        result.Add(path);
                        break;
                    }                   
                    else {
                        seen.Add(entry);
                    }
                    if (ends.Contains(whereAt)) {
                        seenEndNode = true;
                        endNodes.Add(seen.Count - 1, entry);
                    }
                }
            }
            return result;
        }

        private static List<string> StartPositions() => Nodes.Keys.Where(n1 => n1.EndsWith('A')).ToList() ?? [];

        private static List<string> GoalPositions() => Nodes.Keys.Where(n1 => n1.EndsWith('Z')).ToList() ?? [];

        // Least common multiple (LCM), using GCD.
        private static long lcm(long num1, long num2) {
            long gcd1 = gcd(num1, num2);
            long lcm1 = num1 * num2 / gcd1;
            return lcm1;
        }

        // Greatest common divisor (GCD) using Euclid's algorithm.
        private static long gcd(long num1, long num2) {
            if (num2 == 0) {
                return num1;
            }
            else {
                return gcd(num2, num1 % num2);
            }
        }

        static void Setup()
        {
            Directions = string.Empty;
            Nodes.Clear();
        }

        private static void ProcessInputFile(string fileName)
        {
            Console.WriteLine("Processing file " + fileName + " ...");
            ReadInputFile(fileName);
        }

        static void ReadInputFile(string fileName)
        {
            bool directionsSeen = false;
            string token = "[A-Z0-9]+";
            Regex lineFormat = new Regex(@"(" + token + @") = \((" + token + "), (" + token + @")\)");
            using StreamReader sr = new(fileName);
            if (sr != null)
            {
                while (!sr.EndOfStream)
                {
                    var line = sr?.ReadLine()?.Trim() ?? string.Empty;
                    if (line != string.Empty)
                    {
                        if (!directionsSeen) {
                            Directions = line.Trim();
                            directionsSeen = true;
                            continue;
                        }
                        var m = lineFormat.Match(line);
                        if (m.Success) {
                            var fromNode = m.Groups[1].Value;
                            Nodes.Add(fromNode, new Node() { From = fromNode, 
                                Left = m.Groups[2].Value, Right = m.Groups[3].Value });
                        }
                    }
                }
            }
        }
    }
}
