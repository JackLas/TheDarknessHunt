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

sDamage operator*(const sDamage& aLeft, const sResistance& aRight)
{
	sDamage result;
	result.physical = aLeft.physical - (aLeft.physical * (aRight.physical / 100.0f));
	result.magical = aLeft.magical - (aLeft.magical * (aRight.magical / 100.0f));
	return result;
}

int& operator-=(int& aLeft, const sDamage& aRight)
{
	int fullDamage = aRight.physical + aRight.magical;
	aLeft -= fullDamage;
	return aLeft;
}