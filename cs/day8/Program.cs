using System.Text.RegularExpressions;

namespace Day7
{
    internal class Node
    {
        public string From { get; set; }
        public string Left { get; set; }
        public string Right { get; set; }
    }

    class Program
    {
        static string Directions = string.Empty;
        static Dictionary<string, Node> Nodes = new();

        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            Part1("test8a.txt");
            Part1("input8.txt");
            Part2("test8b.txt");
            Part2("input8.txt");
            Console.WriteLine("That's all folks!");
        }

        static void Part1(String fileName)
        {
            Setup();
            Console.WriteLine("Part 1.");
            ProcessInputFile(fileName);
            Int64 count = NumStepsFromTo("AAA", "ZZZ");
            Console.WriteLine("Number of steps from start to finish = " + count);
        }

        private static Int64 NumStepsFromTo(string fromNode, string toNode)
        {
            string node = fromNode;
            string directions = Directions;
            Int64 count = 0;
            while (node != toNode) {
                char whereNow = directions[0];
                directions = directions.Substring(1);
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

        private static List<string> StartPositions() => Nodes.Keys.Where(n1 => n1.EndsWith('A')).ToList();

        private static List<string> GoalPositions() => Nodes.Keys.Where(n1 => n1.EndsWith('Z')).ToList();

        static void Part2(String fileName) {
            Setup();
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);
            var currentPositions = StartPositions();
            var goalPositions = GoalPositions();

            string directions = Directions;
            Int64 count = 0;
            while (!currentPositions.All(p => p.EndsWith('Z'))) {
                char whereNow = directions[0];
                directions = directions.Substring(1);
                if (directions == string.Empty) {
                    directions = Directions;
                }
                if (whereNow == 'L') {
                    currentPositions = currentPositions.Select(p => Nodes[p].Left).ToList();
                    count++;
                }
                else if (whereNow == 'R') {
                    currentPositions = currentPositions.Select(p => Nodes[p].Right).ToList();
                    count++;
                }
                currentPositions.ForEach(p => System.Console.Write(" " + p));
                System.Console.WriteLine();
            }
            Console.WriteLine("Step count = " + count);
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
