#include <mbgl/text/glyph.hpp>

namespace mbgl {

// Note: this only works for the BMP
GlyphRange getGlyphRange(char32_t glyph) {
    unsigned start = (glyph/256) * 256;
    unsigned end = (start + 255);
    if (start > 65280) start = 65280;
    if (end > 65533) end = 65533;
    return { start, end };
}

}
