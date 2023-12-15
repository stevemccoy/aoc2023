namespace Day7
{
    internal enum HandType {
        FiveOfAKind = 7, FourOfAKind = 6, FullHouse = 5, ThreeOfAKind = 4, 
        TwoPair = 3, OnePair = 2, HighCard = 1
    }

    class Program
    {
        static readonly List<Hand> Hands = new();
        static readonly Dictionary<HandType, List<Hand>> GroupedHands = new();

        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            Part1("test7.txt");
            Part1("input7.txt");
            Part2("test7.txt");
            Part2("input7.txt");
            Console.WriteLine("That's all folks!");
        }

        static void Part1(String fileName)
        {
            Setup();
            Console.WriteLine("Part 1.");
            ProcessInputFile(fileName);
            SortCardsPart1();
            Int64 winnings = CalculateWinnings();
            Console.WriteLine("Total winnings = " + winnings);
        }

        static void Part2(String fileName) {
            Setup();
            Hand.CardStrength['J'] = 0;
            Console.WriteLine("Part 2.");
            ProcessInputFile(fileName);
            SortCardsPart2();
            Int64 winnings = CalculateWinnings();
            Console.WriteLine("Total winnings = " + winnings);
        }

        static void Setup()
        {
            Hands.Clear();
            GroupedHands.Clear();
            foreach (var ht in Enum.GetValues<HandType>()) {
                GroupedHands.Add(ht, new List<Hand>());
            }
        }

        private static void SortCardsPart1()
        {
            // Group the hands by type.
            foreach (var h in Hands) {
                GroupedHands[h.Type()].Add(h);
            }
            // Within each group, sort the hands.
            foreach (var g in GroupedHands.Values) {
                g.Sort();
            }
        }

        private static void SortCardsPart2()
        {
            // Group the hands by type.
            foreach (var h in Hands) {
                GroupedHands[h.Part2Type()].Add(h);
            }
            // Within each group, sort the hands.
            foreach (var g in GroupedHands.Values) {
                g.Sort();
            }
        }

        private static Int64 CalculateWinnings() {
            Int64 winnings = 0;
            int rank = 1;
            foreach (var ht in Enum.GetValues<HandType>()) {
                foreach (var h in GroupedHands[ht]) {
                    winnings += h.Bid * rank;
                    rank++;
                }
            }
            return winnings;
        }

        private static void ProcessInputFile(string fileName)
        {
            Hands.Clear();
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
                        var fields = line.Split(" ");
                        var hand = fields[0].Trim();
                        var bid = Int64.Parse(fields[1]);
                        Hands.Add(new Hand() { Cards = hand, Bid = bid });
                    }
                }
            }
        }
    }
}
