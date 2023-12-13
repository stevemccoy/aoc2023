
using System.Runtime.Serialization;

namespace Day5
{

    internal class Mapping
    {
        public Int64 Dest { get; set; }
        public Int64 Source { get; set; }
        public Int64 Length { get; set; }

        public Range SourceRange() {
            return new Range() { Start = Source, Finish = Source + Length - 1 };
        }

        public Range DestRange() {
            return new Range() { Start = Dest, Finish = Dest + Length - 1 };
        }
    }

    class Program
    {
        public static List<Int64> SeedNumbers = new List<Int64>();

        public static List<Map> Maps = new List<Map>();

        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            Part1("test5.txt");
            Part1("input5.txt");
            Part2("test5.txt");
            Part2("input5.txt");
            Console.WriteLine("That's all folks!");
        }

        static void Part1(String fileName)
        {
            Console.WriteLine("Part 1.");
            ProcessInputFile(fileName);
            var result = ForwardAllLevels(SeedNumbers);
            var answer = result.Min();
            Console.WriteLine("Min Location Number = " + answer);
        }

        static void Part2(String fileName) {
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);
            var seedRanges = GetSeedRanges();
            var inputs = new SortedList<Int64, Range>();
            var outputs = seedRanges;
            foreach (var m1 in Maps) {
                inputs = outputs;
                outputs = new SortedList<Int64, Range>();
                foreach (var r1 in inputs.Values) {
                    var temp = m1.Forward(r1);
                    foreach (var r2 in temp.Values) {
                        outputs.Add(r2.Start, r2);
                    }
                }
            }
            Int64 answer = LowestStartIn(outputs);
            Console.WriteLine("Lowest location number corresponding to a seed = " + answer);
        }

        private static void ProcessInputFile(string fileName)
        {
            SeedNumbers.Clear();
            Maps.Clear();
            Console.WriteLine("Processing file " + fileName + " ...");
            ReadInputFile(fileName);
        }

        private static Int64 LowestStartIn(SortedList<long, Range> outputs)
        {
            return outputs.First().Value.Start;
        }

        private static SortedList<Int64, Range> GetSeedRanges()
        {
            SortedList<Int64, Range> result = new SortedList<long, Range>();
            for (int i = 0; i < SeedNumbers.Count; i += 2) {
                var s = SeedNumbers[i];
                result.Add(s, new Range() { Start = s, Finish = s + SeedNumbers[i+1] - 1 });
            }
            return result;
        }

        static List<Int64> ForwardAllLevels(List<Int64> inputs) {
            List<Int64> result = inputs;
            foreach(Map m in Maps) {
                result = result.AsEnumerable().Select(v => m.Forward(v)).ToList();
            }
            return result;
        }

        static void ReadInputFile(string fileName)
        {
            using (StreamReader sr = new StreamReader(fileName))
            {
                ReadSeeds(sr);
                Maps.Clear();
                ReadMap("seed-to-soil", sr);
                ReadMap("soil-to-fertilizer", sr);
                ReadMap("fertilizer-to-water", sr);
                ReadMap("water-to-light", sr);
                ReadMap("light-to-temperature", sr);
                ReadMap("temperature-to-humidity", sr);
                ReadMap("humidity-to-location", sr);
            }
        }

        private static void ReadMap(string name, StreamReader sr)
        {
            String line = string.Empty;
            do {
                line = sr?.ReadLine()?.Trim()??string.Empty;

            } while (line == string.Empty);
            String mapName = line.Split(' ')[0];
            Map m = new Map() { Name=mapName, Mappings=new SortedList<Int64, Mapping>() };
            while (line != string.Empty) {
                line = sr?.ReadLine()?.Trim()??string.Empty;
                if (line != string.Empty) {
                    var nums = line.Split(' ').AsEnumerable<String>().Select(s => Int64.Parse(s)).ToList();
                    var m1 = new Mapping() { Dest = nums[0], Source = nums[1], Length = nums[2] };
                    m.Mappings.Add(m1.Source, m1);
                }
            }
            Maps.Add(m);
        }

        private static void ReadSeeds(StreamReader sr)
        {
            var line = sr.ReadLine()?.Substring(7);
            if (line != null) {
                foreach (var item in line.Split(' ')) {
                    SeedNumbers.Add(Int64.Parse(item));
                }
            }
        }
    }
}
