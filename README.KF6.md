## KF6 Porting notes

### Plugin registration

In KPluginFactory the `registerPlugin(QWidget *parentWidget, QObject *parent, const QVariantList &args)` method was removed in favor of
`registerPlugin(QWidget *parentWidget, QObject *parent, const KPluginMetaData &data, const QVariantList &args)`.
You should adjust your constructors accordingly and pass in the `KPluginMetaData` object to the KParts-superclass you are extending.

Also, you are allowed to remove the `const QVariantList &args` constructor parameter, in case your part does not have any logic using it.
