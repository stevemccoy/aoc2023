namespace Day7
{
    internal class Hand : IComparable<Hand> {
        public required String Cards { get; set; }
        public Int64 Bid { get; set; }
        // Order of precedence for cards.
        public static Dictionary<char, int> CardStrength = new Dictionary<char, int>() {
            {'A', 13}, {'K', 12}, {'Q', 11}, {'J', 10}, {'T', 9}, {'9', 8}, {'8', 7},
            {'7', 6}, {'6', 5}, {'5', 4}, {'4', 3}, {'3', 2}, {'2', 1}};

        // Determine what sort of hand this is.
        public HandType Type() {
            var counts = CalculateCounts();
            if (counts.ContainsValue(5)) {
                return HandType.FiveOfAKind;
            } 
            else if (counts.ContainsValue(4)) {
                return HandType.FourOfAKind;
            }
            var hasThree = counts.ContainsValue(3);
            var hasTwo = counts.ContainsValue(2);
            if (hasThree) {
                return hasTwo ? HandType.FullHouse : HandType.ThreeOfAKind;
            }
            if (hasTwo) {
                var twoPair = counts.Where(c => c.Value == 2).Count() == 2;
                return twoPair ? HandType.TwoPair : HandType.OnePair;
            }
            return HandType.HighCard;
        }

        public HandType Part2Type() {
            var ht = Type();
            var jacks = Cards.Where(c => c == 'J');
            var jackCount = jacks.Count();
            if ((jackCount == 0)||(ht == HandType.FiveOfAKind)) {
                return ht;
            }
            var counts = CalculateCounts();
            counts['J'] = 0;
            while (jackCount > 0) {
                if (TransferWildCard(counts, 4)) {
                    ht = HandType.FiveOfAKind;
                }
                else if (TransferWildCard(counts, 3)) {
                    ht = HandType.FourOfAKind;
                }
                else if (TransferWildCard(counts, 2)) {
                    if (counts.ContainsValue(2)) {
                        ht = HandType.FullHouse;
                    }
                    else {
                        ht = HandType.ThreeOfAKind;
                    }
                }
                else {
                    TransferWildCard(counts, 1);
                    ht = HandType.OnePair;
                }
                jackCount--;
            }
            return ht;
        }

        private static bool TransferWildCard(SortedList<char, int> counts, int lookForCount) {
            if (counts.ContainsValue(lookForCount)) {
                char ch = counts.First(p => p.Value == lookForCount).Key;
                counts[ch]++;
                return true;
            }
            return false;
        }

        private SortedList<char, int> CalculateCounts() {
            SortedList<char, int> counts = new SortedList<char, int>();
            foreach (var ch in Cards) {
                if (counts.ContainsKey(ch)) {
                    counts[ch]++;
                }
                else {
                    counts.Add(ch, 1);
                }
            }
            return counts;
        }

        // Assuming two hands have the same type, compare the cards in turn.
        public int StrengthCompare(Hand other) {
            for (int i = 0; i < Cards.Length; i++) {
                var d = CardStrength[Cards[i]] - CardStrength[other.Cards[i]];
                if (d != 0) {
                    return d;
                }
            }
            return 0;
        }

        public int CompareTo(Hand? other) => (other == null) ? 1 : StrengthCompare(other);
    }
}
