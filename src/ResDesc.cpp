#include "ResDesc.h"
#include <QtCore/QDebug>

QDebug operator<< (QDebug dbg, const ResDesc& rd)
{
	dbg.nospace() << "res=" << rd.res << " desc=" << rd.desc;
	return dbg.space();
}
