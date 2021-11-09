#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class QMouseEvent;
class QOpenGLVertexArrayObject;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class FractalGLWidget : public QOpenGLWidget
{
public:

    FractalGLWidget(QWidget *parent);
    ~FractalGLWidget();

    void setMode(int mode) { m_mode = mode; }
    bool saveImage(const QString &file);

protected:

    void mouseMoveEvent(QMouseEvent *event) override
    {
        update();
    }

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    void allocatePositionBuffer(int w, int h);

    int                       m_mode = 0;
    QOpenGLBuffer             m_positionBuffer;
    QOpenGLBuffer             m_indexBuffer;
    QOpenGLVertexArrayObject *m_vao = nullptr;
    QOpenGLShaderProgram      m_shaderProgram;
};

#endif // OGLWIDGET_H
