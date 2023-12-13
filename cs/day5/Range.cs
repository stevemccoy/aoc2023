namespace Day5
{
    internal class Range
    {
        public Int64 Start { get; set; }
        public Int64 Finish { get; set; }

        public bool Contains(Int64 value) {
            return (value >= Start)&&(value <= Finish);
        }

        public Range? Intersection(Range input) {
            var start   = (input.Start < this.Start ? this.Start : input.Start);
            var finish  = (input.Finish > this.Finish ? this.Finish : input.Finish);
            return (start > finish) ? null : new Range() { Start=start, Finish=finish };
        }

        public SortedList<Int64, Range> Mash(Range range) {
            var result = new SortedList<Int64, Range>();
            var i = Intersection(range);
            if (i == null) {
                result.Add(Start, this);
            }
            else {
                if (i.Start > this.Start) {
                    result.Add(this.Start, new Range() { Start = this.Start, Finish = i.Start - 1});
                }
                result.Add(i.Start, new Range() { Start = i.Start, Finish = i.Finish });
                if (this.Finish > i.Finish) {
                    result.Add(i.Finish + 1, new Range() { Start = i.Finish + 1, Finish = this.Finish});
                }
            }
            return result;
        }
    }
}
