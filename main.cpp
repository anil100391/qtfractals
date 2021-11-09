#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
constexpr bool darkTheme = true;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static QPalette darkPalette()
{
    QPalette darkPalette;
    QColor darkColor = QColor(47, 47, 47);
    QColor disabledColor = QColor(127,127,127);
    darkPalette.setColor(QPalette::Window, darkColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(18,18,18));
    darkPalette.setColor(QPalette::AlternateBase, darkColor);
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Button, darkColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

    return darkPalette;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if constexpr ( darkTheme )
    {
        a.setStyle(QStyleFactory::create("Fusion"));
        a.setPalette(darkPalette());
        a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
