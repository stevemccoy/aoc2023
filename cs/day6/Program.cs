
using System.Runtime.Serialization;

namespace Day6
{
    internal class RaceDetails {
        public Int64 RaceTime { get; set; }
        public Int64 TargetDistance { get; set; }
    }

    class Program
    {
        public static List<RaceDetails> Races = new List<RaceDetails>();

        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            Part1("test6.txt");
            Part1("input6.txt");
            Part2("test6.txt");
            Part2("input6.txt");
            Console.WriteLine("That's all folks!");
        }

        static void Part1(String fileName)
        {
            Console.WriteLine("Part 1.");
            ProcessInputFile(fileName);
            Int64 product = CalculateProduct();
            Console.WriteLine("Product of number of winning options = " + product);
        }

        private static long CalculateProduct()
        {
            Int64 product = 1;
            foreach(var race in Races) {
                var w = WinningRange(race.RaceTime, race.TargetDistance);
                Console.WriteLine("Winning " + w.Start + " - " + w.Finish);
                product *= (w.Finish - w.Start + 1);
            }
            return product;
        }

        static void Part2(String fileName) {
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);

            String strTime = string.Empty;
            String strDistance = string.Empty;
            foreach (var r in Races) {
                strTime = strTime + r.RaceTime.ToString();
                strDistance = strDistance + r.TargetDistance.ToString();
            }
            var bigRace = new RaceDetails() { RaceTime = Int64.Parse(strTime), TargetDistance = Int64.Parse(strDistance) };
            Races.Clear();
            Races.Add(bigRace);

            Int64 product = CalculateProduct();
            Console.WriteLine("Product of number of winning options = " + product);
        }

        private static void ProcessInputFile(string fileName)
        {
            Races.Clear();
            Console.WriteLine("Processing file " + fileName + " ...");
            ReadInputFile(fileName);
        }

        static void ReadInputFile(string fileName)
        {
            Races.Clear();
            using (StreamReader sr = new StreamReader(fileName))
            {
                var times = ReadNumbers(sr);
                var distances = ReadNumbers(sr);
                for (int i = 0; i < times.Count; i++) {
                    Races.Add(new RaceDetails() { RaceTime = times[i], TargetDistance = distances[i] });
                }
            }
        }

        private static List<Int64> ReadNumbers(StreamReader sr)
        {
            List<Int64> result = new List<Int64>();
            var line = sr.ReadLine()?.Substring(10).Trim();
            Int64 v = 0;
            if (line != null) {
                foreach (var item in line.Split(' ')) {
                    if (Int64.TryParse(item, out v)) {
                        result.Add(v);
                    }
                }
            }
            return result;
        }

        private static Range? WinningRange(Int64 raceTime, Int64 targetDistance) {
            Int64 high = raceTime / 2;
            Int64 low = 1;
            Int64 high_d = high * (raceTime - high) - targetDistance;
            Int64 low_d = low * (raceTime - low) - targetDistance;
            while ((high > low + 1)&&(high_d > 0)&&(low_d <= 0)) {
                Int64 mid = (low + high) / 2;
                Int64 mid_d = mid * (raceTime - mid) - targetDistance;
                if (mid_d <= 0) {
                    low = mid;
                    low_d = mid_d;
                }
                else {
                    high = mid;
                    high_d = mid_d;
                }
            }
            return new Range() { Start = high, Finish = raceTime - high };
        }
    }
}
