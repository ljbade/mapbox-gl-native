#ifndef LLMR_STYLE_BUCKET_DESCRIPTION
#define LLMR_STYLE_BUCKET_DESCRIPTION

#include <string>
#include <vector>
#include <cmath>

#include "value.hpp"

namespace llmr {

enum class BucketType {
    None = 0,
    Fill = 1,
    Line = 2,
    Icon = 3,
    Text = 4,
    Raster = 5
};

enum class CapType {
    None = 0,
    Round = 1,
    Butt = 2,
    Square = 3
};

enum class JoinType {
    None = 0,
    Miter = 1,
    Bevel = 2,
    Round = 3
};

enum class TextPathType {
    Horizontal = 0,
    Curve = 1
};

/*
 * For style v1, this is for parsing values for bucket.*.feature.feature_type, in which case the
 * type refers to data which is being filtered out of a data source. For style v0, this also used
 * to also be for parsing bucket.type, in which case the type refered to the type of the bucket,
 * but now the syntax has changed.
 */
inline BucketType bucketType(const std::string& type) {
    if (type == "fill") return BucketType::Fill;
    else if (type == "line") return BucketType::Line;
    else if (type == "point") return BucketType::Icon;
    else if (type == "text") return BucketType::Text;
    else if (type == "raster") return BucketType::Raster;
    else return BucketType::None;
}

inline CapType capType(const std::string& cap) {
    if (cap == "round") return CapType::Round;
    else if (cap == "butt") return CapType::Butt;
    else if (cap == "square") return CapType::Square;
    else return CapType::None;
}


inline JoinType joinType(const std::string& join) {
    if (join == "miter") return JoinType::Miter;
    else if (join == "bevel") return JoinType::Bevel;
    else if (join == "round") return JoinType::Round;
    else return JoinType::None;
}

inline TextPathType textPathType(const std::string& path) {
    if (path == "horizontal") return TextPathType::Horizontal;
    else if (path == "curve") return TextPathType::Curve;
    else return TextPathType::Horizontal;
};

class BucketGeometryDescription {
public:
    CapType cap = CapType::None;
    JoinType join = JoinType::None;
    std::string font;
    std::string text_field;
    float font_size = 0.0f;
    float miter_limit = 2.0f;
    float round_limit = 1.0f;
    TextPathType path = TextPathType::Horizontal;
    float padding = 2.0f;
    float textMinDistance = 250.0f;
    float rotate = 0.0f; // what is this?
    float maxAngleDelta = M_PI;
    bool alwaysVisible = false;
};

class BucketDescription {
public:
    // This is the type of the source for the filter
    BucketType feature_type = BucketType::None;

    // This is type of the bucket itself for rendering purposes
    BucketType type = BucketType::None;

    // Specify what data to pull into this bucket
    std::string source_name;
    std::string source_layer;
    std::string source_field;
    std::vector<Value> source_value;

    // Specifies how the geometry for this bucket should be created
    BucketGeometryDescription geometry;
};

std::ostream& operator<<(std::ostream&, const BucketDescription& bucket);

}

#endif
