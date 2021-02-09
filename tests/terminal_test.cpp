#include <QMainWindow>

class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window();
};

#include <QDir>

#include <KService>
#include <QApplication>
#include <kde_terminal_interface.h>
#include <kparts/readonlypart.h>

Window::Window()
{
    this->resize(800, 600);

    KService::Ptr service = KService::serviceByDesktopName(QStringLiteral("konsolepart"));
    Q_ASSERT(service);

    KParts::ReadOnlyPart *part = service->createInstance<KParts::ReadOnlyPart>(this, this, QVariantList());
    Q_ASSERT(part);

    setCentralWidget(part->widget());

    TerminalInterface *interface = qobject_cast<TerminalInterface *>(part);
    interface->showShellInDir(QDir::home().path());

    connect(part, &QObject::destroyed, this, &QObject::deleteLater);
}

int main(int argc, char **argv)
{
    QApplication::setApplicationName(QStringLiteral("tetest"));
    QApplication app(argc, argv);
    Window *window = new Window();
    window->show();
    return app.exec();
}

#include "terminal_test.moc"
