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

#if ANDROID
    setAttribute(Qt::WA_AcceptTouchEvents);
#endif
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
    openGLWidget = new FractalGLWidget(this);
    openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
    setCentralWidget(openGLWidget);

    menubar = new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 799, 22));

    auto fileMenu = menubar->addMenu("File");

    fileMenu->addAction( QIcon(":/resource/render.svg"),
                         "Render Image",
                         [this](){openGLWidget->saveImage();},
                         QKeySequence(Qt::CTRL | Qt::Key_P) );

    fileMenu->addAction( QIcon(":/resource/quit.svg"),
                         "Quit",
                         [this](){close();},
                         QKeySequence(Qt::CTRL | Qt::Key_Q) );

    auto viewMenu = menubar->addMenu("View");
    auto grids = viewMenu->addAction( "Show Grid",
                                      [this](bool flag){openGLWidget->showGrid(flag); openGLWidget->update();},
                                      QKeySequence(Qt::CTRL | Qt::Key_G) );
    grids->setCheckable(true);
    grids->setChecked(true);

    viewMenu->addAction( QIcon(":/resource/reset.svg"),
                         "Reset Bounds",
                         [this](){openGLWidget->resetBounds(); openGLWidget->update();},
                         QKeySequence(Qt::CTRL | Qt::Key_R) );

    auto fracMenu = menubar->addMenu("Fractal");
    fracMenu->addAction("Julia", [this](){openGLWidget->setMode(0); openGLWidget->update();});
    fracMenu->addAction("Mandelbrot", [this](){openGLWidget->setMode(1); openGLWidget->update();});
    fracMenu->addAction("Newton", [this](){openGLWidget->setMode(2); openGLWidget->update();});

    setMenuBar(menubar);
    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    setStatusBar(statusbar);

    setWindowIcon(QIcon(":/resource/julia.png"));
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
