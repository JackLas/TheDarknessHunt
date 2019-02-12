#include "dataTypes.h"

sDamage& operator+=(sDamage& aLeft, const sDamage& aRight)
{
	aLeft.physical += aRight.physical;
	aLeft.magical += aRight.magical;
	return aLeft;
}

sDamage& operator-=(sDamage& aLeft, const sDamage& aRight)
{
	aLeft.physical -= aRight.physical;
	aLeft.magical -= aRight.magical;
	return aLeft;
}