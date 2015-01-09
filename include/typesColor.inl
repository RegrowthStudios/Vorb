/// RGB color using 8-bit elements
struct ColorRGB8 {
public:
    /// Create a color specifying all elements
    /// @param r: Red value
    /// @param g: Green value 
    /// @param b: Blue value
    ColorRGB8(ui8 r, ui8 g, ui8 b) :
        r(r), g(g), b(b) {
        // Empty
    }
    /// Create a color specifying all elements in integer values
    /// @param r: Red value
    /// @param g: Green value
    /// @param b: Blue value
    ColorRGB8(i32 r, i32 g, i32 b) : ColorRGB8((ui8)r, (ui8)g, (ui8)b) {
        // Empty
    }
    /// Create a color specifying all elements in floating point values
    /// @param r: Red value [0.0f, 1.0f]
    /// @param g: Green value [0.0f, 1.0f]
    /// @param b: Blue value [0.0f, 1.0f]
    ColorRGB8(f32 r, f32 g, f32 b) : ColorRGB8(
        ((ui8)(r * 255.0f)),
        ((ui8)(g * 255.0f)),
        ((ui8)(b * 255.0f))) {
        // Empty
    }
    /// Default black color
    ColorRGB8() : ColorRGB8(0, 0, 0) {
        // Empty
    }

    /// Access a color element by its index
    /// @param i: Color index in range [0,2]
    /// @return Reference to color element
    const ui8& operator[] (const size_t& i) const {
        return data[i];
    }
    /// Access a color element by its index
    /// @param i: Color index in range [0,2]
    /// @return Reference to color element
    ui8& operator[] (const size_t& i) {
        return data[i];
    }

    union {
        struct {
            ui8 r; ///< Red value
            ui8 g; ///< Green value
            ui8 b; ///< Blue value
        };
        ui8 data[3]; ///< RGB values stored in array
    };
};
typedef ColorRGB8 color3; ///< Simple name for ColorRGB8

/// RGBA color using 8-bit elements
struct ColorRGBA8 {
public:
    /// Create a color specifying all elements
    /// @param r: Red value
    /// @param g: Green value 
    /// @param b: Blue value
    /// @param a: Alpha value
    ColorRGBA8(ui8 r, ui8 g, ui8 b, ui8 a = 0xffu) :
        r(r), g(g), b(b), a(a) {
        // Empty
    }
    /// Create a color specifying all elements in integer values
    /// @param r: Red value
    /// @param g: Green value
    /// @param b: Blue value
    /// @param a: Alpha value
    ColorRGBA8(i32 r, i32 g, i32 b, i32 a = 255) : ColorRGBA8((ui8)r, (ui8)g, (ui8)b, (ui8)a) {
        // Empty
    }
    /// Create a color specifying all elements in floating point values
    /// @param r: Red value [0.0f, 1.0f]
    /// @param g: Green value [0.0f, 1.0f]
    /// @param b: Blue value [0.0f, 1.0f]
    /// @param a: Alpha value [0.0f, 1.0f]
    ColorRGBA8(f32 r, f32 g, f32 b, f32 a = 1.0f) : ColorRGBA8(
        ((ui8)(r * 255.0f)),
        ((ui8)(g * 255.0f)),
        ((ui8)(b * 255.0f)),
        ((ui8)(a * 255.0f))) {
        // Empty
    }
    /// Default black color
    ColorRGBA8() : ColorRGBA8(0, 0, 0) {
        // Empty
    }

    /// Access a color element by its index
    /// @param i: Color index in range [0,3]
    /// @return Reference to color element
    const ui8& operator[] (const size_t& i) const {
        return data[i];
    }
    /// Access a color element by its index
    /// @param i: Color index in range [0,3]
    /// @return Reference to color element
    ui8& operator[] (const size_t& i) {
        return data[i];
    }

    union {
        struct {
            ColorRGB8 rgb; ///< RGB value
            ui8 rgb_padding; ///< RGBA remainder value from RGB padding
        };
        struct {
            ui8 r; ///< Red value
            ui8 g; ///< Green value
            ui8 b; ///< Blue value
            ui8 a; ///< Alpha value
        };
        ui8 data[4]; ///< RGBA values stored in array
    };
};
typedef ColorRGBA8 color4; ///< Simple name for ColorRGBA8
