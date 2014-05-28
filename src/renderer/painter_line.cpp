#include <llmr/renderer/painter.hpp>
#include <llmr/renderer/line_bucket.hpp>
#include <llmr/map/map.hpp>

using namespace llmr;

void Painter::renderLine(LineBucket& bucket, const StyleClass &properties, const std::string& layer_name, const Tile::ID& id) {
    // Abort early.
    float width = properties.get(StylePropertyKey::Width, StylePropertyDefault::Width());
    float offset = properties.get(StylePropertyKey::Offset, StylePropertyDefault::Offset()) / 2;

    // These are the radii of the line. We are limiting it to 16, which will result
    // in a point size of 64 on retina.
    float inset = std::fmin((std::fmax(-1, offset - width / 2 - 0.5) + 1), 16.0f);
    float outset = std::fmin(offset + width / 2 + 0.5, 16.0f);


    float opacity = properties.get(StylePropertyKey::Opacity, StylePropertyDefault::Opacity());
    Color color = properties.get(StylePropertyKey::Color, StylePropertyDefault::LineColor());
    color[0] *= opacity;
    color[1] *= opacity;
    color[2] *= opacity;
    color[3] *= opacity;

    float dash_length = properties.get(StylePropertyKey::DashArrayLand, StylePropertyDefault::DashArrayLand());
    float dash_gap = properties.get(StylePropertyKey::DashArrayGap, StylePropertyDefault::DashArrayGap());

    const float translate_x = properties.get(StylePropertyKey::TranslateX, StylePropertyDefault::TranslateX());
    const float translate_y = properties.get(StylePropertyKey::TranslateY, StylePropertyDefault::TranslateY());
    const TranslateAnchor translate_anchor = properties.get(StylePropertyKey::TranslateAnchor, StylePropertyDefault::TranslateAnchor());

    const mat4 &vtxMatrix = translatedMatrix({{ translate_x, translate_y }}, id, translate_anchor);

    glDepthRange(strata, 1.0f);

    // We're only drawing end caps + round line joins if the line is > 2px. Otherwise, they aren't visible anyway.
    if (bucket.hasPoints() && outset > 1.0f) {
        useProgram(linejoinShader->program);
        linejoinShader->setMatrix(vtxMatrix);
        linejoinShader->setColor(color);
        linejoinShader->setWorld({{
                map.getState().getFramebufferWidth() * 0.5f,
                map.getState().getFramebufferHeight() * 0.5f
            }
        });
        linejoinShader->setLineWidth({{
                ((outset - 0.25f) * map.getState().getPixelRatio()),
                ((inset - 0.25f) * map.getState().getPixelRatio())
            }
        });

        float pointSize = std::ceil(map.getState().getPixelRatio() * outset * 2.0);
#if defined(GL_ES_VERSION_2_0)
        linejoinShader->setSize(pointSize);
#else
        glPointSize(pointSize);
#endif
        bucket.drawPoints(*linejoinShader);
    }

    // var imagePos = properties.image && imageSprite.getPosition(properties.image);
    bool imagePos = false;
    if (imagePos) {
        // var factor = 8 / Math.pow(2, painter.transform.zoom - params.z);

        // imageSprite.bind(gl, true);

        // //factor = Math.pow(2, 4 - painter.transform.zoom + params.z);
        // gl.switchShader(painter.linepatternShader, painter.translatedMatrix || painter.posMatrix, painter.extrudeMatrix);
        // shader = painter.linepatternShader;
        // glUniform2fv(painter.linepatternShader.u_pattern_size, [imagePos.size[0] * factor, imagePos.size[1] ]);
        // glUniform2fv(painter.linepatternShader.u_pattern_tl, imagePos.tl);
        // glUniform2fv(painter.linepatternShader.u_pattern_br, imagePos.br);
        // glUniform1f(painter.linepatternShader.u_fade, painter.transform.z % 1.0);

    } else {
        useProgram(lineShader->program);
        lineShader->setMatrix(vtxMatrix);
        lineShader->setExtrudeMatrix(extrudeMatrix);
        lineShader->setDashArray({{ dash_length, dash_gap }});
        lineShader->setLineWidth({{ outset, inset }});
        lineShader->setRatio(map.getState().getPixelRatio());
        lineShader->setColor(color);
        bucket.drawLines(*lineShader);
    }
}
