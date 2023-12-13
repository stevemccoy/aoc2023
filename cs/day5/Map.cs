namespace Day5
{
    internal class Map
    {
        public String Name { get; set; }
        public SortedList<Int64, Mapping> Mappings { get; set; }
        public Int64 Forward(Int64 fromValue) {
            foreach (var m in Mappings) {                
                var d = fromValue - m.Key;
                if (d >= 0 &&  d < m.Value.Length) {
                    return (m.Value.Dest + d);
                }
            }
            return fromValue;
        }

        public SortedList<Int64, Range> Forward(Range inputRange) {
            var inputs = new SortedList<Int64, Range>();
            inputs.Add(inputRange.Start, inputRange);
            SortedList<Int64, Range> result = new SortedList<Int64, Range>();
            foreach (var m1 in Mappings) {
                var mapSource = m1.Value.SourceRange();
                var additions = new SortedList<Int64, Range>();
                foreach (var i1 in inputs) {
                    var mashed = i1.Value.Mash(mapSource);
                    foreach (var item in mashed) {
                        additions.Add(item.Key, item.Value);
                    }
                }
                inputs = additions;
            }
            foreach (var r in inputs.Values) {
                var output = new Range() { Start = Forward(r.Start), Finish = Forward(r.Finish) };
                result.Add(output.Start, output);
            }
            return result;
        }
    }
}
