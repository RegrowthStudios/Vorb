#define RADIX_MAX_MASK_BITS 8
#define RADIX_MAX_BITS 31
// Radix Sort That Makes Sorted Index List
template<typename TData, typename TIndex, i32 bits>
void radixSort(TIndex* indices, TData* data, i32 n, i32(*converter)(TData*), i32 maxBits) {
    // We Don't Want This Many Bins
    if (bits < 1 || bits > RADIX_MAX_MASK_BITS) return;
    // To Prevent Sorting Negative Values Or Nothing
    if (maxBits < 1 || maxBits > RADIX_MAX_BITS || n < 1) return;
    // The Number Of Bins
    const i32 bins = 1 << bits;
    // Create The Bit Mask
    const i32 mask = bins - 1;
    // Create The Indices And Converted Values
    TIndex* inds = indices;
    ui32* converted = new ui32[n];
    REGISTER i32 i = 0;
    for (; i < n; i++) {
        inds[i] = i;
        converted[i] = converter(data + i) + 0x80000000;
    }
    // Buffer Arrays For Sorted Elements
    TIndex* indsBuf = new TIndex[n];
    ui32* convertedBuf = new ui32[n];
    // For The Count Sorting
    i32* binCounts = new i32[bins];
    REGISTER i32 bin;
    bool isUnsorted;
    // Sort All Of The Bits
    while (maxBits > 0) {
        maxBits -= bits;
        // Clear Counts
        memset(binCounts, 0, bins * sizeof(i32));
        // Count Number Of Elements In Each Bin
        isUnsorted = false;
        for (i = n - 1; i > 0; i--) {
            // Check For Unsorted
            if (converted[i] < converted[i - 1]) {
                isUnsorted = true;
                while (i > 0) {
                    binCounts[converted[i] & mask]++;
                    i--;
                }
                break;
            }
            binCounts[converted[i] & mask]++;
        }
        // Temporal Coherence Is Established
        if (!isUnsorted) {
            break;
        }
        binCounts[converted[0] & mask]++;
        // Make Counts As End-Value Indices
        for (i = 1; i < bins; i++) {
            binCounts[i] += binCounts[i - 1];
        }
        for (i = n - 1; i >= 0; i--) {
            bin = converted[i] & mask;
            // Set The Data
            binCounts[bin]--;
            indsBuf[binCounts[bin]] = inds[i];
            convertedBuf[binCounts[bin]] = converted[i] >> bits;
        }
        // Swap Pointers
        TIndex* sbuf1 = indsBuf; indsBuf = inds; inds = sbuf1;
        ui32* sbuf2 = convertedBuf;
        convertedBuf = converted;
        converted = sbuf2;
    }
    // Delete Temporary Data Buffers
    delete converted;
    delete convertedBuf;
    // Make Sure Initial Entry Is Modified
    if (inds != indices) {
        memcpy_s(indices, n * sizeof(TIndex), inds, n * sizeof(TIndex));
        delete inds;
    } else {
        delete indsBuf;
    }
}
// Radix Sort That Sorts In Place
template<typename TData, i32 bits>
void radixSort(TData* data, i32 n, i32(*converter)(TData*), i32 maxBits) {
    // We Don't Want This Many Bins
    if (bits < 1 || bits > RADIX_MAX_MASK_BITS) {
        return;
    }
    // To Prevent Sorting Negative Values Or Nothing
    if (maxBits < 1 || maxBits > RADIX_MAX_BITS || n < 1) {
        return;
    }
    // The Number Of Bins
    const i32 bins = 1 << bits;
    // Create The Bit Mask
    const i32 mask = bins - 1;
    // Create The Indices And Converted Values
    TData* dBuf1 = data;
    ui32* converted = new ui32[n];
    REGISTER i32 i = 0;
    for (; i < n; i++) {
        converted[i] = converter(data + i) + 0x80000000;
    }
    // Buffer Arrays For Sorted Elements
    TData* dBuf2 = new TData[n];
    ui32* convertedBuf = new ui32[n];
    // For The Count Sorting
    i32* binCounts = new i32[bins];
    REGISTER i32 bin;
    bool isUnsorted;
    // Sort All Of The Bits
    while (maxBits > 0) {
        maxBits -= bits;
        // Clear Counts
        memset(binCounts, 0, bins * sizeof(i32));
        // Count Number Of Elements In Each Bin
        isUnsorted = false;
        for (i = n - 1; i > 0; i--) {
            // Check For Unsorted
            if (converted[i] < converted[i - 1]) {
                isUnsorted = true;
                while (i > 0) {
                    binCounts[converted[i] & mask]++;
                    i--;
                }
                break;
            }
            binCounts[converted[i] & mask]++;
        }
        // Temporal Coherence Is Established
        if (!isUnsorted) {
            break;
        }
        binCounts[converted[0] & mask]++;
        // Make Counts As End-Value Indices
        for (i = 1; i < bins; i++) {
            binCounts[i] += binCounts[i - 1];
        }
        for (i = n - 1; i >= 0; i--) {
            bin = converted[i] & mask;
            // Set The Data
            binCounts[bin]--;
            dBuf2[binCounts[bin]] = dBuf1[i];
            convertedBuf[binCounts[bin]] = converted[i] >> bits;
        }
        // Swap Pointers
        TData* sbuf1 = dBuf2; dBuf2 = dBuf1; dBuf1 = sbuf1;
        ui32* sbuf2 = convertedBuf; convertedBuf = converted; converted = sbuf2;
    }
    // Delete Temporary Data Buffers
    delete converted;
    delete convertedBuf;
    // Make Sure Initial Entry Is Modified
    if (dBuf1 != data) {
        memcpy_s(data, n * sizeof(TData), dBuf1, n * sizeof(TData));
        delete dBuf1;
    } else {
        delete dBuf2;
    }
}
