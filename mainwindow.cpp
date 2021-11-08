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
    setupUI(this);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void MainWindow::setupUI(QWidget *widget)
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
    setMenuBar(menubar);
    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    setStatusBar(statusbar);
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
    openGLWidget->resize(width(), height());
}
