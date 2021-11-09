#include "fractalGLWidget.h"
#include "mainwindow.h"

#include <cmath>

#include <QCursor>
#include <QFileDialog>
#include <QWheelEvent>
#include <QMouseEvent>

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
FractalGLWidget::FractalGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_mainWindow(static_cast<MainWindow *>(parent)),
      m_positionBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    QSurfaceFormat format;
    format.setVersion(4, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    setMouseTracking(true);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
FractalGLWidget::~FractalGLWidget()
{
    m_vao->release();
    delete m_vao;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::resetBounds()
{
    m_spanY = 3.0f;
    m_center = QVector2D(0.0f, 0.0f);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool FractalGLWidget::saveImage(const QString &file)
{
    QImage img = grabFramebuffer();
    img.save(file);
    m_mainWindow->postMessage(file + " saved");
    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::wheelEvent(QWheelEvent *event)
{
    QOpenGLWidget::wheelEvent(event);
    zoom(event->angleDelta().y() >= 0);
    update();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseMoveEvent(event);

    auto pos = event->pos();
    auto buttons = event->buttons();
    if ( (buttons & (Qt::LeftButton | Qt::MiddleButton | Qt::RightButton)) != 0 )
    {
        // panning
        float xposRel = (1.0 * pos.x()) / width();
        float yposRel = (1.0 * (height() - pos.y())) / height();

        float spanX = (m_spanY * width()) / height();
        QVector2D toCenter((0.5 - xposRel) * spanX, (0.5 - yposRel) * m_spanY);

        m_center = m_grabbedForPan + toCenter;
    }

    auto c = coord(pos.x(), pos.y());
    m_mainWindow->postMessage(QString::number(c.x()) +", " + QString::number(c.y()));
    update();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::mousePressEvent(QMouseEvent *event)
{
    QOpenGLWidget::mousePressEvent(event);
    QPoint pos = event->pos();
    m_grabbedForPan = coord(pos.x(), height() - pos.y());
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create VAO for first object to render
    m_vao = new QOpenGLVertexArrayObject( this );
    m_vao->create();
    m_vao->bind();

    // Setup VBOs and IBO (use QOpenGLBuffer to buffer data,
    // specify format, usage hint etc). These will be
    // remembered by the currently bound VAO
    m_positionBuffer.create();
    m_positionBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    allocatePositionBuffer(width(), height());

    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.bind();
    unsigned int indexData[] = { 0, 1, 2, 0, 2, 3 };
    m_indexBuffer.allocate( indexData, 3 * 2 * sizeof( unsigned int ) );

    m_shaderProgram.create();
    m_shaderProgram.bind();
    m_shaderProgram.enableAttributeArray(0);
    m_shaderProgram.setAttributeBuffer( 0, GL_FLOAT, 0, 2 );
    if ( !m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "vert.shader") )
    {
        QString log = m_shaderProgram.log();
    }

    if ( !m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "frag.shader") )
    {
        QString log = m_shaderProgram.log();
    }

    m_shaderProgram.link();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::resizeGL(int w, int h)
{
    allocatePositionBuffer(w, h);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::allocatePositionBuffer(int w, int h)
{
    if (w == -1)
        w = width();
    if (h == -1)
        h = height();

    m_positionBuffer.bind();
    float pad = 5.0f;
    float positionData[] = { pad, pad,
                             w - pad, pad,
                             w - pad, h - pad,
                             pad, h - pad
                           };
    int vertexCount = 4;
    m_positionBuffer.allocate( positionData, vertexCount * 2 * sizeof( float ) );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
QVector2D FractalGLWidget::minCoord() const
{
    float spanX = (m_spanY * width()) / height();
    return m_center - QVector2D(0.5 * spanX, 0.5 * m_spanY);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
QVector2D FractalGLWidget::maxCoord() const
{
    float spanX = (m_spanY * width()) / height();
    return m_center + QVector2D(0.5 * spanX, 0.5 * m_spanY);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
QVector2D FractalGLWidget::coord(int x, int y) const
{
    auto minc = minCoord();
    float spanX = (width() * m_spanY) / height();
    return { minc.x() + (spanX * x / width()),
             minc.y() + (m_spanY * y)/height() };
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    f->glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Bind shader program, textures for first set of objects
    m_shaderProgram.bind();
    auto log = m_shaderProgram.log();
    QMatrix4x4 mvp;
    mvp.ortho(0.0f, width(), 0.0f, height(), -1.0f, 1.0f);

    QPoint cpos = QCursor::pos();
    m_shaderProgram.setUniformValue("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);
    m_shaderProgram.setUniformValue("u_MVP", mvp);
    m_shaderProgram.setUniformValue("u_C0", -3 + (4.0 * cpos.x())/width(), -1.5 + (3.0 * cpos.y())/height());
    m_shaderProgram.setUniformValue("u_AspectRatio", 1.0f * width() / height());
    m_shaderProgram.setUniformValue("u_SpanY", m_spanY);
    m_shaderProgram.setUniformValue("u_Center", m_center);
    m_shaderProgram.setUniformValue("u_Mode", m_mode);
    m_shaderProgram.setUniformValue("u_Width", width());
    m_shaderProgram.setUniformValue("u_Height", height());
    m_shaderProgram.setUniformValue("u_ShowGrid", (m_showGrid && m_mode != 2) ? 1 : 0);
    // Switch to the vertex data for first object and draw it
    m_vao->bind();
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void FractalGLWidget::zoom(bool inverted)
{
    // auto minc = minCoord();
    int dir = inverted ? -1 : 1;
    m_spanY = m_spanY + 0.1 * dir;
}
