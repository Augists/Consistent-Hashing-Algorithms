package jumphash

// Hash consistently chooses a bucket in the range [0, numBuckets) for the given key.
// numBuckets must be positive. If not, it returns -1.
func Hash(key uint64, numBuckets int32) int32 {
	if numBuckets <= 0 {
		return -1
	}

	var b int64 = -1
	var j int64 = 0

	for j < int64(numBuckets) {
		b = j
		key = key*2862933555777941757 + 1
		// The following is equivalent to the C++ implementation:
		// j = (b + 1) * (double(1LL << 31) / double((key >> 33) + 1));
		// Note: 1LL << 31 is 2^31, which is 2147483648.
		j = int64(float64(b+1) * (2147483648.0 / float64((key>>33)+1)))
	}
	return int32(b)
}