#include "material_p.h"
#include "background_p.h"
#include "uniform_p.h"

#include <QMetaType>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QRegularExpression>
#include <QSGGeometry>
#include <QSGMaterialShader>
#include <QSGNode>
#include <QSGTextureProvider>
#include <QSurface>

#include <private/qquickitem_p.h>

namespace Sailfish { namespace Silica { namespace Background {

// BackgroundState class
class BackgroundState : public QSharedData
{
public:
    using Pointer = QExplicitlySharedDataPointer<BackgroundState>;

    explicit BackgroundState(const QString &vertexShader, const QString &fragmentShader, uint serial);
    QSGMaterialShader *createInteriorShader() const;
    QSGMaterialShader *createInteriorAlphaShader() const;
    QSGMaterialShader *createCornerShader() const;

    const uint serial;
    QSGMaterialType interiorType;
    QSGMaterialType interiorAlphaType;
    QSGMaterialType cornerType;
    const std::vector<Uniform> uniforms;
    const bool includeNormalizedPosition;

private:
    const QByteArray m_vertexShader;
    const QByteArray m_fragmentShader;
};

// CornerVertex struct
struct CornerVertex
{
    float m_x, m_y;
    float m_tx, m_ty;
    float m_cx, m_cy;

    void set(float x, float y, float tx, float ty, float cx, float cy)
    {
        m_x = x;
        m_y = y;
        m_tx = tx;
        m_ty = ty;
        m_cx = cx;
        m_cy = cy;
    }
};

// Define cornerVertexAttributes
const QSGGeometry::AttributeSet &cornerVertexAttributes()
{
    static QSGGeometry::Attribute data[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true),
        QSGGeometry::Attribute::create(1, 2, GL_FLOAT),
        QSGGeometry::Attribute::create(2, 2, GL_FLOAT)
    };
    static QSGGeometry::AttributeSet attributes = {3, sizeof(CornerVertex), data};
    return attributes;
}

// MaterialState class
class MaterialState
{
public:
    explicit MaterialState(const BackgroundState::Pointer &shader);

    int compare(const MaterialState &other, Uniform::Aspect aspect) const;

    const BackgroundState::Pointer shader;
    std::vector<Uniform> uniforms;
    QSGTextureProvider *pattern = nullptr;
    QSGTextureProvider *source = nullptr;
    QSGTransformNode *sourceTransform = nullptr;
    QSizeF patternSize;
    float radius = 0.0f;
    uint serial = 0;
    Background::FillMode fillMode = Background::PreserveAspectWidth;
    QSGTexture::Filtering backgroundFiltering = QSGTexture::Linear;
    bool resolved[3] = { false, false, false };
};

// InteriorShader class
class InteriorShader : public QSGMaterialShader
{
public:
    explicit InteriorShader(const QByteArray &vertex, const QByteArray &fragment, char const *const *attributeNames, Uniform::Aspect aspect = Uniform::Interior);
    const char *vertexShader() const override;
    const char *fragmentShader() const override;
    char const *const *attributeNames() const override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    const QByteArray m_vertexShader;
    const QByteArray m_fragmentShader;
    char const *const * const m_attributeNames;
    const Uniform::Aspect m_aspect;
    int id_positionMatrix, id_transformMatrix, id_sourceMatrix, id_patternMatrix;
    int id_sourceTexture, id_patternTexture, id_clip;
};

// InteriorMaterial class
class InteriorMaterial : public QSGMaterial
{
public:
    explicit InteriorMaterial(MaterialState &state, Uniform::Aspect aspect = Uniform::Interior);
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;

    MaterialState &state;

private:
    const Uniform::Aspect m_aspect;
};

// InteriorAlphaShader class
class InteriorAlphaShader : public InteriorShader
{
public:
    explicit InteriorAlphaShader(const QByteArray &vertex, const QByteArray &fragment, char const *const *attributeNames, Uniform::Aspect aspect = Uniform::InteriorAlpha);
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int id_opacity;
};

// InteriorAlphaMaterial class
class InteriorAlphaMaterial : public InteriorMaterial
{
public:
    explicit InteriorAlphaMaterial(MaterialState &state, Uniform::Aspect aspect = Uniform::InteriorAlpha);
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
};

// CornerShader class
class CornerShader : public InteriorAlphaShader
{
public:
    explicit CornerShader(const QByteArray &vertex, const QByteArray &fragment, char const *const *attributeNames);
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void initialize() override;

private:
    int id_edge;
};

// CornerMaterial class
class CornerMaterial : public InteriorAlphaMaterial
{
public:
    explicit CornerMaterial(MaterialState &state);
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;
};

// BackgroundNode class
class BackgroundNode : public QObject, public QSGNode
{
public:
    explicit BackgroundNode(const BackgroundState::Pointer &shader);
    ~BackgroundNode() override;

    uint shaderSerial() const { return m_materialState.shader->serial; }
    uint materialSerial() const { return m_materialState.serial; }

    void setGeometry(const QRectF &rectangle, qreal radius, Corners::Flags corners);
    void setBlending(bool blending);
    bool setTransformItem(QQuickItem *transformItem);
    bool setFillMode(Background::FillMode mode);
    bool setPatternSize(const QSizeF &size);
    bool setSource(QQuickItem *sourceItem);
    bool setPattern(QQuickItem *patternItem);
    bool setUniforms(Material *material, Background *shader, QQmlEngine *engine, QQuickWindow *window);

    void preprocess() override;
    void materialChanged();
    void providerDestroyed();

private:
    MaterialState m_materialState;
    InteriorMaterial m_interiorMaterial { m_materialState };
    InteriorAlphaMaterial m_interiorAlphaMaterial { m_materialState };
    CornerMaterial m_cornerMaterial { m_materialState };
    QSGGeometry m_interiorGeometry {
        m_materialState.shader->includeNormalizedPosition
                ? QSGGeometry::defaultAttributes_TexturedPoint2D()
                : QSGGeometry::defaultAttributes_Point2D(),
        8
    };
    QSGGeometry m_cornerGeometry {
        m_materialState.shader->includeNormalizedPosition
                ? cornerVertexAttributes()
                : QSGGeometry::defaultAttributes_TexturedPoint2D(),
        12
    };
    QSGGeometryNode m_interiorNode;
    QSGGeometryNode m_cornerNode;
    QRectF m_rectangle;
    Corners::Flags m_corners = Corners::All;
    bool m_blending = false;
};

// Object-level functionality

static QByteArray injectBackground2D(const QByteArray &fragmentShader)
{
    if (!fragmentShader.contains("background2D")) {
        return fragmentShader;
    }

    QByteArray copy = "\n uniform highp vec4 silica_backgroundClip;" + fragmentShader;

    const int insertAt = copy.indexOf("void backgroundMain()");

    if (insertAt == -1) {
        return fragmentShader;
    }

    copy.insert(insertAt,
                "\n lowp vec4 background2D(lowp sampler2D sampler, highp vec2 coord) {"
                "\n     lowp vec2 bounds = step("
                "\n                 abs(coord - silica_backgroundClip.xy),"
                "\n                 silica_backgroundClip.zw);"
                "\n     return texture2D(sampler, coord) * bounds.x * bounds.y;"
                "\n }"
                "\n");

    return copy;
}

static bool hasNormalizedPosition(const QString &vertexShader)
{
    static const QRegularExpression positionAttribute(QLatin1String(
                "attribute\\s+(?:highp\\s+|lowp\\s+)?(?:vec2|vec3|vec4)\\s+normalizedPosition\\s*;"));
    return positionAttribute.match(vertexShader).hasMatch();
}

BackgroundState::BackgroundState(const QString &vertexShader, const QString &fragmentShader, uint serial)
    : serial(serial)
    , uniforms(Uniform::parse(vertexShader, fragmentShader))
    , includeNormalizedPosition(hasNormalizedPosition(vertexShader))
    , m_vertexShader(vertexShader.toUtf8())
    , m_fragmentShader(injectBackground2D(fragmentShader.toUtf8()))
{
}

static const char * const ATTRIBUTES_NORMALIZED[] = {
    "position",
    "normalizedPosition",
    nullptr
};

static const char * const ATTRIBUTES[] = {
    "position",
    nullptr
};

static const char * const CORNER_ATTRIBUTES_NORMALIZED[] = {
    "position",
    "normalizedPosition",
    "silica_corner",
    nullptr
};

static const char * const CORNER_ATTRIBUTES[] = {
    "position",
    "silica_corner",
    nullptr
};

QSGMaterialShader *BackgroundState::createInteriorShader() const
{
    QByteArray vertex = m_vertexShader + "\n void main() {"
                                         "\n backgroundMain();"
                                         "\n }";
    
    QByteArray fragment = m_fragmentShader + "\n void main() {"
                                         "\n backgroundMain();"
                                         "\n }";

    return new InteriorShader(vertex, fragment, includeNormalizedPosition ? ATTRIBUTES_NORMALIZED : ATTRIBUTES);
}

QSGMaterialShader *BackgroundState::createInteriorAlphaShader() const
{
    QByteArray vertex = m_vertexShader + "\n void main() {"
                                         "\n backgroundMain();"
                                         "\n }";
    
    QByteArray fragment = QByteArray("\n uniform lowp float silica_opacity;")
                         + m_fragmentShader 
                         + "\n void main() {"
                                         "\n backgroundMain();"
                                         "\n gl_FragColor = gl_FragColor * silica_opacity;"
                                         "\n }";

    return new InteriorAlphaShader(vertex, fragment, includeNormalizedPosition ? ATTRIBUTES_NORMALIZED : ATTRIBUTES);
}

QSGMaterialShader *BackgroundState::createCornerShader() const
{
    QByteArray vertex
            = "\n attribute highp vec2 silica_corner;"
            "\n varying highp vec2 silica_c;"
            + m_vertexShader + "\n void main() {"
                       "\n silica_c = silica_corner;"
                       "\n backgroundMain();"
                       "\n }";
    
    QByteArray fragment
            = "\n varying highp vec2 silica_c;"
            "\n uniform lowp float silica_opacity;"
            "\n uniform lowp vec2 silica_edge;"
            + m_fragmentShader + "\n void main() {"
                         "\n backgroundMain();"
                         "\n gl_FragColor = gl_FragColor * (silica_opacity * smoothstep("
                                             "\n silica_edge.x,"
                                             "\n silica_edge.y,"
                                                                             "\n dot(silica_c, silica_c)));"
                         "\n }";

    return new CornerShader(vertex, fragment, includeNormalizedPosition ? CORNER_ATTRIBUTES_NORMALIZED : CORNER_ATTRIBUTES);
}

// MaterialState methods
MaterialState::MaterialState(const BackgroundState::Pointer &shader)
    : shader(shader)
{
}

int MaterialState::compare(const MaterialState &other, Uniform::Aspect aspect) const
{
    int comparison = compareAttributes(
                pattern, other.pattern,
                source, other.source,
                sourceTransform, other.sourceTransform,
                fillMode, other.fillMode,
                uniforms.size(), other.uniforms.size());

    for (auto it = uniforms.begin(), otherIt = other.uniforms.begin();
                comparison == 0 && it != uniforms.end() && otherIt != other.uniforms.end();
                ++it, ++otherIt) {
        comparison = it->compare(*otherIt, aspect);
    }

    return comparison;
}

// InteriorShader methods
InteriorShader::InteriorShader(
        const QByteArray &vertex,
        const QByteArray &fragment,
        char const *const *attributeNames,
        Uniform::Aspect aspect)
    : m_vertexShader(vertex)
    , m_fragmentShader(fragment)
    , m_attributeNames(attributeNames)
    , m_aspect(aspect)
    , id_positionMatrix(-1)
    , id_transformMatrix(-1)
    , id_sourceMatrix(-1)
    , id_patternMatrix(-1)
    , id_sourceTexture(-1)
    , id_patternTexture(-1)
    , id_clip(-1)
{
}

const char *InteriorShader::vertexShader() const
{
    return m_vertexShader.constData();
}

const char *InteriorShader::fragmentShader() const
{
    return m_fragmentShader.constData();
}

char const *const *InteriorShader::attributeNames() const
{
    return m_attributeNames;
}

void InteriorShader::updateState(
        const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    MaterialState *newState = &static_cast<InteriorMaterial *>(newMaterial)->state;
    const MaterialState *oldState = oldMaterial ? &static_cast<InteriorMaterial *>(oldMaterial)->state : nullptr;

    QOpenGLShaderProgram *program = QSGMaterialShader::program();
    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();

    QSize surfaceSize = QOpenGLContext::currentContext()->surface()->size();
    QSGTexture *source = newState->source ? newState->source->texture() : nullptr;
    QSGTexture *pattern = newState->pattern ? newState->pattern->texture() : nullptr;

    if (state.isMatrixDirty() && id_positionMatrix != -1) {
        program->setUniformValue(id_positionMatrix, state.combinedMatrix());
    }

    GLenum activeTexture = 0;

    if (id_sourceTexture != -1) {
        if (!oldState || oldState->source != newState->source) {
            functions->glActiveTexture(GL_TEXTURE0 + activeTexture);
            program->setUniformValue(id_sourceTexture, activeTexture);
            
            if (source) {
                source->setFiltering(QSGTexture::Linear);
                source->bind();
            } else {
                functions->glBindTexture(GL_TEXTURE_2D, 0);
            }
        }

        ++activeTexture;
    }

    qreal surfaceRatio = surfaceSize.width() / qreal(surfaceSize.height());
    bool mirrored = state.projectionMatrix()(1, 1) < 0;

    QMatrix4x4 transformMatrix = QMatrix4x4({{0.5, 0, 0, 0.5},
                                              {0, mirrored ? -0.5 : 0.5, 0, 0.5},
                                              {0, 0, 1, 0},
                                              {0, 0, 0, 1}});

    if (newState->sourceTransform) {
        QMatrix4x4 sourceTransform = mirrored
                ? newState->sourceTransform->matrix()
                : newState->sourceTransform->matrix().transposed();

        sourceTransform.setRow(3, QVector4D(0, 0, 0, 1));
        sourceTransform.setColumn(3, QVector4D(0, 0, 0, 1));
        transformMatrix *= sourceTransform;
        if (newState->fillMode == Background::PreserveAspectWidth && surfaceRatio >= 1.0) {
            transformMatrix.scale(1.0 / surfaceRatio, 1.0 / surfaceRatio);
        }
    }

    if (id_transformMatrix != -1) {
        QMatrix4x4 matrix = transformMatrix;
        matrix.scale(surfaceRatio, 1.f);
        program->setUniformValue(id_transformMatrix, matrix);
    }

    if (id_sourceMatrix == -1) {
        if (id_clip != -1) {
            program->setUniformValue(id_clip, QVector4D(0, 0, 0.5f, 0.5f));
        }
    } else {
        bool update = !oldState
                || oldState->source != newState->source
                || oldState->sourceTransform != newState->sourceTransform
                || oldState->fillMode != newState->fillMode;
        if (update) {
            QSizeF sourceSize = source ? source->textureSize() : QSizeF(1, 1);
            QRectF sourceSubRect = source ? source->normalizedTextureSubRect() : QRectF(0, 0, 1, 1);
            qreal sourceRatio = sourceSize.width() / sourceSize.height();
            qreal scaleX = 1.0;
            qreal scaleY = 1.0;

            switch (newState->fillMode)
            {
                case Background::Stretch:
                    break;
                case Background::PreserveAspectWidth:
                    scaleX = surfaceRatio / sourceRatio;
                    break;
                case Background::PreserveAspectSquare:
                    if (sourceSize.width() > sourceSize.height())
                    {
                        scaleX = qMin<qreal>(1.0, sourceRatio) * surfaceRatio;
                        scaleY = qMin<qreal>(1.0, sourceRatio) / sourceRatio;
                    } else {
                        scaleX = qMin<qreal>(1.0, sourceRatio) * surfaceRatio / sourceRatio;
                        scaleY = qMin<qreal>(1.0, sourceRatio);
                    }
                    break;
            }

            QMatrix4x4 sourceMatrix;
            sourceMatrix.translate(sourceSubRect.x(), sourceSubRect.y());
            sourceMatrix.scale(sourceSubRect.width(), sourceSubRect.height());

            sourceMatrix *= transformMatrix;
            sourceMatrix.scale(float(scaleX), float(scaleY));
            
            program->setUniformValue(id_sourceMatrix, sourceMatrix);
            
            if (id_clip != -1)
            {
                QPointF center = sourceSubRect.center();
                program->setUniformValue(id_clip, QVector4D(center.x(), center.y(), sourceSubRect.width() / 2, sourceSubRect.height() / 2));
            }
        }
    }

    if (id_patternTexture != -1)
    {
        if (!oldState || oldState->pattern != newState->pattern)
        {
            functions->glActiveTexture(GL_TEXTURE0 + activeTexture);
            program->setUniformValue(id_patternTexture, activeTexture);

            if (pattern)
            {
                if (pattern->isAtlasTexture())
                    pattern = pattern->removedFromAtlas();
                pattern->setFiltering(QSGTexture::Linear);
                pattern->setVerticalWrapMode(QSGTexture::Repeat);
                pattern->setHorizontalWrapMode(QSGTexture::Repeat);
                pattern->bind();
            } else {
                functions->glBindTexture(GL_TEXTURE_2D, 0);
            }
        }

        ++activeTexture;
    }

    if (id_patternMatrix != -1 && (!oldState || oldState->pattern != newState->pattern)) {
        QMatrix4x4 patternMatrix;

        QSizeF patternSize = newState->patternSize.isValid() || !pattern
                                ? newState->patternSize
                                : pattern->textureSize();

        if (patternSize.isValid())
        {
            patternMatrix.scale(1.0 / patternSize.width(), (mirrored ? -1.0 : 1.0) / patternSize.height());
            patternMatrix.scale(0.5 * surfaceSize.width(), 0.5 * surfaceSize.height());
        }

        program->setUniformValue(id_patternMatrix, patternMatrix);
    }

    if (!newState->resolved[m_aspect])
    {
        newState->resolved[m_aspect] = true;

        for (auto &uniform: newState->uniforms)
        {
            uniform.location[m_aspect] = program->uniformLocation(uniform.name);
        }
    }

    auto itNEW = newState->uniforms.begin();
    if (oldState) {
        auto oldIT = oldState->uniforms.begin();

        for (; itNEW != newState->uniforms.end() && oldIT != oldState->uniforms.end(); ++itNEW, ++oldIT) {
            if ((itNEW->location[m_aspect] == -1) || (itNEW->compare(*oldIT, m_aspect) == 0)) {
                continue;
            }

            itNEW->set(program, functions, activeTexture, m_aspect);
        }
    }

    for (; itNEW != newState->uniforms.end(); ++itNEW)
    {
        if (itNEW->location[m_aspect] != -1)
            itNEW->set(program, functions, activeTexture, m_aspect);
    }

    if (activeTexture > 1) {
        functions->glActiveTexture(GL_TEXTURE0);
    }
}

void InteriorShader::initialize()
{
    QOpenGLShaderProgram *program = QSGMaterialShader::program();

    id_positionMatrix = program->uniformLocation("positionMatrix");
    id_transformMatrix = program->uniformLocation("transformMatrix");
    id_sourceTexture = program->uniformLocation("sourceTexture");
    id_sourceMatrix = program->uniformLocation("sourceMatrix");
    id_patternTexture = program->uniformLocation("patternTexture");
    id_patternMatrix = program->uniformLocation("patternMatrix");
    id_clip = program->uniformLocation("silica_backgroundClip");
}

// InteriorMaterial methods
InteriorMaterial::InteriorMaterial(MaterialState &state, Uniform::Aspect aspect)
    : state(state)
    , m_aspect(aspect)
{

}

QSGMaterialType *InteriorMaterial::type() const
{
    return &state.shader->interiorType;
}

QSGMaterialShader *InteriorMaterial::createShader() const
{
    return const_cast<BackgroundState*>(state.shader.data())->createInteriorShader();
}

int InteriorMaterial::compare(const QSGMaterial *other) const
{
    return state.compare(static_cast<const InteriorMaterial *>(other)->state, m_aspect);
}

// InteriorAlphaShader methods
InteriorAlphaShader::InteriorAlphaShader(
        const QByteArray &vertex,
        const QByteArray &fragment,
        char const *const *attributeNames,
        Uniform::Aspect aspect)
    : InteriorShader(vertex, fragment, attributeNames, aspect)
    , id_opacity(-1)
{

}

void InteriorAlphaShader::updateState(
        const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    InteriorShader::updateState(state, new