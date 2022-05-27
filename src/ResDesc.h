#pragma once

#include <QtCore/QString>
class QDebug;

struct ResDesc
{
	qint64 res;
	QString desc;
};

QDebug operator << (QDebug dbg, const ResDesc& rd);
