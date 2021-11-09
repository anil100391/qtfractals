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

    void postMessage(const QString &message) const;

protected:

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    void setupUI();

    FractalGLWidget *openGLWidget = nullptr;
    QMenuBar        *menubar = nullptr;
    QStatusBar      *statusbar = nullptr;
};

#endif // MAINWINDOW_H
