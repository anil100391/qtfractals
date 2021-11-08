#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class FractalGLWidget;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event);

private:

    void setupUI(QWidget *widget);

    FractalGLWidget *openGLWidget = nullptr;
    QMenuBar        *menubar = nullptr;
    QStatusBar      *statusbar = nullptr;
};

#endif // MAINWINDOW_H
