#define SHIFT_ONE_BYTE 8
#define SHIFT_TWO_BYTES (SHIFT_ONE_BYTE * 2)
#define SHIFT_THREE_BYTES (SHIFT_ONE_BYTE * 3)
#define BYTE04 0xff000000u
#define BYTE03 0x00ff0000u
#define BYTE02 0x0000ff00u
#define BYTE01 0x000000ffu

// Endianness Change
inline ui32 changeEndian(const ui32& val) {
    // TODO: Use Assembly
    return (((val << SHIFT_THREE_BYTES) & BYTE04) |
        ((val << SHIFT_ONE_BYTE) & BYTE03) |
        ((val >> SHIFT_ONE_BYTE) & BYTE02) |
        ((val >> SHIFT_THREE_BYTES) & BYTE01));
}

/************************************************************************/
/* Buffer Utilities (Little Endian)                                     */
/************************************************************************/
namespace BufferUtils {
    /************************************************************************/
    /* Extraction                                                           */
    /************************************************************************/
    inline ui32 extractInt(const ui8* a, ui32 i) {
        return (((ui32)a[i + 3]) << SHIFT_THREE_BYTES) |
               (((ui32)a[i + 2]) << SHIFT_TWO_BYTES) |
               (((ui32)a[i + 1]) << SHIFT_ONE_BYTE) |
               (((ui32)a[i]));
    }
    inline ui32 extractInt(const ui8* a) {
        return (((ui32)a[3]) << SHIFT_THREE_BYTES) |
               (((ui32)a[2]) << SHIFT_TWO_BYTES) |
               (((ui32)a[1]) << SHIFT_ONE_BYTE) |
               (((ui32)a[0]));
    }

    inline ui16 extractShort(const ui8* a, ui32 i) {
        return (((ui32)a[i + 1]) << SHIFT_ONE_BYTE) | (((ui32)a[i]));
    }
    inline ui16 extractShort(const ui8* a) {
        return (((ui32)a[1]) << SHIFT_ONE_BYTE) | (((ui32)a[0]));
    }

    inline f32 extractFloat(const ui8* a, ui32 i) {
        ui32 n = extractInt(a, i);
        return *((f32*)(&n));
    }
    inline f32 extractFloat(const ui8* a) {
        ui32 n = extractInt(a);
        return *((f32*)(&n));
    }

    inline bool extractBool(const ui8* a, ui32 i) {
        return (a[i] != 0);
    }

    /************************************************************************/
    /* Insertion                                                            */
    /************************************************************************/
    inline i32 setInt(ui8* a, ui32 i, ui32 data) {
        a[i + 3] = (ui8)(data >> SHIFT_THREE_BYTES);
        a[i + 2] = (ui8)((data & BYTE03) >> SHIFT_TWO_BYTES);
        a[i + 1] = (ui8)((data & BYTE02) >> SHIFT_ONE_BYTE);
        a[i] = (ui8)(data & BYTE01);
        return sizeof(ui32);
    }
    inline i32 setInt(ui8* a, ui32 data) {
        a[3] = (ui8)(data >> SHIFT_THREE_BYTES);
        a[2] = (ui8)((data & BYTE03) >> SHIFT_TWO_BYTES);
        a[1] = (ui8)((data & BYTE02) >> SHIFT_ONE_BYTE);
        a[0] = (ui8)(data & BYTE01);
        return sizeof(ui32);
    }

    inline i32 setShort(ui8* a, ui32 i, ui32 data) {
        a[i + 1] = (ui8)((data & BYTE02) >> SHIFT_ONE_BYTE);
        a[i] = (ui8)(data & BYTE01);
        return sizeof(ui32);
    }
    inline i32 setShort(ui8* a, ui32 data) {
        a[1] = (ui8)((data & BYTE02) >> SHIFT_ONE_BYTE);
        a[0] = (ui8)(data & BYTE01);
        return sizeof(ui32);
    }

    inline i32 setFloat(ui8* a, ui32 i, f32 data) {
        setInt(a, i, *((ui32*)(&data)));
        return sizeof(f32);
    }
    inline i32 setFloat(ui8* a, f32 data) {
        setInt(a, *((ui32*)(&data)));
        return sizeof(f32);
    }

    inline i32 setBool(ui8* a, ui32 i, bool data) {
        a[i] = (ui8)data;
        return sizeof(ui8);
    }
}
