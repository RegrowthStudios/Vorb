/*! \file typesColor.inl
 * @brief Vorb's RGB and RGBA color types.
 */

/*! @brief RGB color using 8-bit elements
 */
struct ColorRGB8 {
public:
    /*! @brief Construct a color with 8-bit RGB elements.
     * 
     * @param r: Red value
     * @param g: Green value
     * @param b: Blue value
     */
    ColorRGB8(ui8 r, ui8 g, ui8 b) :
        r(r), g(g), b(b) {
        // Empty
    }
    /*! @brief Construct a color from integer values.
     * 
     * Values experience a conversion to ui8 elements via static_cast<ui8>(value).
     * 
     * @param r: Red value
     * @param g: Green value
     * @param b: Blue value
     */
    explicit ColorRGB8(i32 r, i32 g, i32 b) : ColorRGB8(static_cast<ui8>(r), static_cast<ui8>(g), static_cast<ui8>(b)) {
        // Empty
    }
    /*! @brief Construct a color from floating point values
     * 
     * Values experience a conversion to ui8 elements via static_cast<ui8>(value * 255.0f).
     * It is recommended that floating point values fall within the range [0.0f, 1.0f].
     * 
     * @param r: Red value
     * @param g: Green value
     * @param b: Blue value
     */
    explicit ColorRGB8(f32 r, f32 g, f32 b) : ColorRGB8(
        static_cast<ui8>(r * 255.0f),
        static_cast<ui8>(g * 255.0f),
        static_cast<ui8>(b * 255.0f)) {
        // Empty
    }
    /*! @brief Construct a black color
     * 
     * The RGB value of the color is (0, 0, 0)
     */
    ColorRGB8() : ColorRGB8(0, 0, 0) {
        // Empty
    }

    /*! @brief Access a color element by its index
     * 
     * No range checks are performed on the index argument.
     * 
     * @param i: Color index in range [0,2]
     * @return Reference to color element
     */
    const ui8& operator[] (const size_t& i) const {
        return data[i];
    }
    /*! @brief Access a color element by its index
     *
     * No range checks are performed on the index argument.
     *
     * @param i: Color index in range [0,2]
     * @return Reference to color element
     */
    ui8& operator[] (const size_t& i) {
        return data[i];
    }

    /*! @brief Set RGB values to the linear RGB interpolation of two colors.
     * 
     * The values are calculated in the following manner:
     * <code>
     * this = (1 - ratio) * ca + ratio * cb;
     * </code>
     * It is recommended that the ratio is a value between [0.0f, 1.0f].
     * 
     * @param ca: Starting color
     * @param cb: Ending color
     * @param ratio: Ratio of mixing between the start to the end color
     */
    void lerp(const ColorRGB8& ca, const ColorRGB8& cb, f32 ratio) {
        f32 invRatio = 1.0f - ratio;
        r = (ui8)(invRatio * ca.r + ratio * cb.r);
        g = (ui8)(invRatio * ca.g + ratio * cb.g);
        b = (ui8)(invRatio * ca.b + ratio * cb.b);
    }

    bool operator==(const ColorRGB8& rhs) const {
        return (r == rhs.r && g == rhs.g && b == rhs.b);
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
typedef ColorRGB8 color3; ///< Shortened name for ColorRGB8

/*! @brief RGBA color using 8-bit elements
 */
struct ColorRGBA8 {
public:
    /*! @brief Construct a color with 8-bit RGB elements.
     *
     * @param r: Red value
     * @param g: Green value
     * @param b: Blue value
     * @param a: Alpha value
     */
    ColorRGBA8(ui8 r, ui8 g, ui8 b, ui8 a = 0xffu) :
        r(r), g(g), b(b), a(a) {
        // Empty
    }
    /*! @brief Construct a color from integer values.
     *
     * Values experience a conversion to ui8 elements via static_cast<ui8>(value).
     *
     * @param r: Red value
     * @param g: Green value
     * @param b: Blue value
     * @param b: Alpha value
     */
    explicit ColorRGBA8(i32 r, i32 g, i32 b, i32 a = 255) : ColorRGBA8((ui8)r, (ui8)g, (ui8)b, (ui8)a) {
        // Empty
    }
    /*! @brief Construct a color from floating point values
     *
     * Values experience a conversion to ui8 elements via static_cast<ui8>(value * 255.0f).
     * It is recommended that floating point values fall within the range [0.0f, 1.0f].
     *
     * @param r: Red value
     * @param g: Green value
     * @param b: Blue value
     * @param a: Alpha value
     */
    explicit ColorRGBA8(f32 r, f32 g, f32 b, f32 a = 1.0f) : ColorRGBA8(
        ((ui8)(r * 255.0f)),
        ((ui8)(g * 255.0f)),
        ((ui8)(b * 255.0f)),
        ((ui8)(a * 255.0f))) {
        // Empty
    }
    /*! @brief Construct a black color
     *
     * The RGBA value of the color is (0, 0, 0, 255)
     */
    ColorRGBA8() : ColorRGBA8(0, 0, 0) {
        // Empty
    }

    /*! @brief Access a color element by its index
     *
     * No range checks are performed on the index argument.
     *
     * @param i: Color index in range [0,3]
     * @return Reference to color element
     */
    const ui8& operator[] (const size_t& i) const {
        return data[i];
    }
    /*! @brief Access a color element by its index
     *
     * No range checks are performed on the index argument.
     *
     * @param i: Color index in range [0,3]
     * @return Reference to color element
     */
    ui8& operator[] (const size_t& i) {
        return data[i];
    }

    /*! @brief Set RGBA values to the linear RGB interpolation of two colors.
     *
     * The values are calculated in the following manner:
     * <code>
     * this = (1 - ratio) * ca + ratio * cb;
     * </code>
     * It is recommended that the ratio is a value between [0.0f, 1.0f].
     *
     * @param ca: Starting color
     * @param cb: Ending color
     * @param ratio: Ratio of mixing between the start to the end color
     */
    void lerp(const ColorRGBA8& ca, const ColorRGBA8& cb, f32 ratio) {
        f32 invRatio = 1.0f - ratio;
        r = (ui8)(invRatio * ca.r + ratio * cb.r);
        g = (ui8)(invRatio * ca.g + ratio * cb.g);
        b = (ui8)(invRatio * ca.b + ratio * cb.b);
        a = (ui8)(invRatio * ca.r + ratio * cb.a);
    }

    bool operator==(const ColorRGBA8& rhs) const {
        return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
    }

    union {
        struct {
            ColorRGB8 rgb; ///< RGB value
            ui8 rgb_padding; ///< RGBA alpha remainder value from RGB padding
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
typedef ColorRGBA8 color4; ///< Shortened name for ColorRGBA8
