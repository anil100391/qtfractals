#include "mainwindow.h"
#include "fractalGLWidget.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::postMessage(const QString &message) const
{
    statusbar->showMessage(message);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::setupUI()
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("MainWindow"));
    resize(1200, 800 );
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);
    openGLWidget = new FractalGLWidget(this);
    openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
    layout->addWidget(openGLWidget);
    openGLWidget->resize(width(), height());

    menubar = new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 799, 22));

    auto fileMenu = menubar->addMenu("File");
    auto render = fileMenu->addAction("Render Image", [this](){openGLWidget->saveImage("render.png");});
    render->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    auto quit = fileMenu->addAction("Quit",[this](){close();});
    quit->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));

    auto viewMenu = menubar->addMenu("View");
    auto grids = viewMenu->addAction("Show Grid", [this](bool flag){openGLWidget->showGrid(flag); openGLWidget->update();});
    grids->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    grids->setChecked(true);
    grids->setCheckable(true);
    auto reset = viewMenu->addAction("Reset Bounds", [this](){openGLWidget->resetBounds(); openGLWidget->update();});
    reset->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));

    auto fracMenu = menubar->addMenu("Fractal");
    fracMenu->addAction("Julia", [this](){openGLWidget->setMode(0); openGLWidget->update();});
    fracMenu->addAction("Mandelbrot", [this](){openGLWidget->setMode(1); openGLWidget->update();});
    fracMenu->addAction("Newton", [this](){openGLWidget->setMode(2); openGLWidget->update();});

    setMenuBar(menubar);
    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    setStatusBar(statusbar);
    setWindowIcon(QIcon("julia.png"));
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
   QMainWindow::mouseMoveEvent(event);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::mousePressEvent(QMouseEvent *event)
{
   QMainWindow::mousePressEvent(event);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QMainWindow::mouseReleaseEvent(event);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    openGLWidget->resize(width(), height() - 20);
}
