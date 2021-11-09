#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class MainWindow;
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
    void showGrid(bool flag) { m_showGrid = flag; }
    bool saveImage(const QString &file);

protected:

    void mouseMoveEvent(QMouseEvent *event) override
    {
        QOpenGLWidget::mouseMoveEvent(event);
        update();
    }

    void wheelEvent(QWheelEvent *event) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    void zoom(bool inverted);
    void allocatePositionBuffer(int w, int h);
    QVector2D minCoord() const;
    QVector2D maxCoord() const;

    int                       m_mode = 1;
    bool					  m_showGrid = true;
    QVector2D				  m_center = {0.0f, 0.0f};
    float				      m_spanY = 3.0f;

    MainWindow				 *m_mainWindow = nullptr;
    QOpenGLBuffer             m_positionBuffer;
    QOpenGLBuffer             m_indexBuffer;
    QOpenGLVertexArrayObject *m_vao = nullptr;
    QOpenGLShaderProgram      m_shaderProgram;
};

#endif // OGLWIDGET_H
