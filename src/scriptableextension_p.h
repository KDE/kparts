/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2010 Maksim Orlovich <maksim@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef kparts_scriptableextension_p_h
#define kparts_scriptableextension_p_h

#include "liveconnectextension.h"
#include "scriptableextension.h"

namespace KParts
{
// LiveConnectExtension -> ScriptableExtension adapter.
class ScriptableLiveConnectExtension : public ScriptableExtension
{
    Q_OBJECT
public:
    ScriptableLiveConnectExtension(QObject *parent, LiveConnectExtension *old);

    QVariant rootObject() override;
    // enclosingObject: not applicable, plugins wouldn't have children

    // callAsFunction: we only have function rereferences.
    QVariant callFunctionReference(ScriptableExtension *callerPrincipal, quint64 objId, const QString &f, const ArgList &args) override;

    // callAsConstructor: unsupported by LC

    bool hasProperty(ScriptableExtension *callerPrincipal, quint64 objId, const QString &propName) override;

    QVariant get(ScriptableExtension *callerPrincipal, quint64 objId, const QString &propName) override;

    bool put(ScriptableExtension *callerPrincipal, quint64 objId, const QString &propName, const QVariant &value) override;

    // removeProperty: unsupported by LC
    // enumerateProperties: unsupported by LC
    // setException: unsupported by LC
    // evaluateScript: unsupported by LC, though we have to
    //                 route LC evaluation requests to our parent
    //                 as appropriate

    void acquire(quint64 objid) override;
    void release(quint64 objid) override;

private:
    // LC uses 0-1 refcounting, we use arbitrary, so we need to call
    // unregister when done.
    QHash<quint64, int> refCounts;
    LiveConnectExtension *wrapee;

    // also registers when needed
    QVariant fromLC(const QString &name, LiveConnectExtension::Type type, unsigned long objId, const QString &value);

    QString toLC(const QVariant &in, bool *ok);
public Q_SLOTS:
    void liveConnectEvent(const unsigned long, const QString &, const KParts::LiveConnectExtension::ArgList &);
};

} // namespace KParts

#endif
