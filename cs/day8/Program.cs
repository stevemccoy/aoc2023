﻿using System.ComponentModel.DataAnnotations;
using System.Net;
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

        public bool EndsOnStep(int step) {
            if (EndPoints.ContainsKey(step)) {
                return true;
            }
            if (step >= LeadSteps) {
                return EndPoints.ContainsKey(LeadSteps + (step - LeadSteps) % CycleSteps);
            }
            return false;
        }
    }

    internal class Cycle
    {
        public string Start { get; set; }
        public string Finish { get; set; }
        public Int64 LeadSteps { get; set; }
        public Int64 RepeatSteps { get; set; }
        public Int64 FinishAt { get; set; }
    }

    class Program
    {
        static string Directions = string.Empty;
        static readonly Dictionary<string, Node> Nodes = new();

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

        static void Part2(String fileName) {
            Setup();
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);

            var cycles = ExtractCycles();
            if (cycles.Count == 0) {
                Console.WriteLine("No endpoints found.");
                return;
            }
            Dictionary<string, Int64> tideMarks = new();
            bool done = false;
            Int64 target = 0;
            while (!done) {
                // Find low water mark for each start point.
                foreach (var s1 in cycles.Keys) {
                    tideMarks[s1] = cycles[s1].Min(c1 => c1.FinishAt);
                }
                // Find highest low water mark and bring others up to it.
                target = tideMarks.Values.Max();
                foreach (var s2 in cycles.Keys) {
                    foreach (var c2 in cycles[s2]) {
                        while (c2.FinishAt < target) {
                            Int64 m = Int64.Max(1, (target - c2.FinishAt) / c2.RepeatSteps);
                            c2.FinishAt += c2.RepeatSteps * m;
                        }
                    }
                }
                // Have we found a solution?
                done = tideMarks.Values.All(v => v == target);
            }
            Console.WriteLine("Done. Found Endpoint At: " + target);
        }

        private static Int64 NumStepsFromTo(string fromNode, string toNode)
        {
            string node = fromNode;
            string directions = Directions;
            Int64 count = 0;
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

        private static Dictionary<string, List<Cycle>> ExtractCycles() {
            var paths = AnalysePaths();
            Dictionary<string, List<Cycle>> cycles = new();
            foreach (var path in paths) {
                foreach (var ep in path.EndPoints) {
                    if (!cycles.ContainsKey(path.Start)) {
                        cycles[path.Start] = new List<Cycle>();
                    }
                    cycles[path.Start].Add(new Cycle()
                    {
                        Start = path.Start,
                        Finish = ep.Value,
                        FinishAt = ep.Key,
                        LeadSteps = path.LeadSteps,
                        RepeatSteps = path.CycleSteps
                    });
                }
            }
            return cycles;
        }

        private static List<Path> AnalysePaths() {
            List<Path> result = new List<Path>();
            var starts = StartPositions();
            var ends = GoalPositions();
            foreach (var s in starts) {
                string whereAt = s;
                var directions = Directions;
                List<string> seen = new() { s };
                Dictionary<int, string> endNodes = new();
                while (true) {
                    char whereNow = directions[0];
                    directions = (directions.Length == 1) ? Directions : directions.Substring(1);
                    whereAt = (whereNow == 'L') ? Nodes[whereAt].Left : Nodes[whereAt].Right;
                    if (seen.Contains(whereAt)) {
                        int leadCount = seen.IndexOf(whereAt);
                        Path path = new Path()
                        {
                            Start = s,
                            CycleStart = whereAt,
                            LeadSteps = leadCount,
                            CycleSteps = seen.Count - leadCount,
                            EndPoints = endNodes
                        };
                        result.Add(path);
                        break;
                    }                   
                    else {
                        seen.Add(whereAt);
                    }
                    if (ends.Contains(whereAt)) {
                        endNodes[seen.Count - 1] = whereAt;
                    }
                }
            }
            return result;
        }

        private static List<string> StartPositions() => Nodes.Keys.Where(n1 => n1.EndsWith('A')).ToList() ?? new List<string>();

        private static List<string> GoalPositions() => Nodes.Keys.Where(n1 => n1.EndsWith('Z')).ToList() ?? new List<string>();

        private static Int64 CycleEndsMeetAt(Cycle c1, Cycle c2) {
            // Detect if cycles can or cannot meet.
            if (c1.RepeatSteps == c2.RepeatSteps) {
                if (c1.LeadSteps == c2.LeadSteps) {
                    return c1.FinishAt;
                }
                else {
                    return 0;
                }
            }
            // Make sure c1 has the longer period.
            if (c1.RepeatSteps < c2.RepeatSteps) {
                (c1, c2) = (c2, c1);
            }
            // Now trial multiples of c1 until we find one that is a multiple of c2.
            for (Int64 j = 1; j < c1.RepeatSteps; j++) {
                var num = j * c1.RepeatSteps + c1.LeadSteps - c2.LeadSteps;
                if (num % c2.RepeatSteps == 0) {
                    return j * c1.RepeatSteps + c1.LeadSteps;
                }
            }
            return 0;
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
