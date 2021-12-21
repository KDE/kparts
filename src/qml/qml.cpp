#include <KPluginFactory>
#include <KLocalizedString>

#include "qml.h"
#include "mimeutils.h"

QVector<KPluginMetaData> partsForMimeType(const QString &mimeType)
{
    auto supportsMime = [&](const KPluginMetaData &md) {
        return md.supportsMimeType(mimeType);
    };
    QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("kf5/parts-qml"), supportsMime);

    auto orderPredicate = orderPredicateForMimetype(mimeType);
    std::sort(plugins.begin(), plugins.end(), orderPredicate);

    return plugins;
}

QMLPart *createPartInstanceForMimeType(const QString &mimeType, QWidget *parentWidget = nullptr, QObject *parent = nullptr, QString *error = nullptr)
{
    const QVector<KPluginMetaData> plugins = partsForMimeType(mimeType);
    for (const auto &plugin : plugins) {
        auto factory = KPluginFactory::loadFactory(plugin);
        if (factory) {
            if (QMLPart *part = factory.plugin->create<QMLPart>(parentWidget, parent, QVariantList())) {
                return part;
            } else if (error) {
                *error = i18n("The plugin '%1' does not provide an interface '%2'", //
                              plugin.fileName(),
                              QLatin1String(QMLPart::staticMetaObject.className()));
            }
        } else if (error) {
            *error = factory.errorString;
        }
    }
    if (error && error->isEmpty()) {
        *error = i18n("No part was found for mimeType %1", mimeType);
    }
    return nullptr;
}