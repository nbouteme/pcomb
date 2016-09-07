#include "../parser.h"

void *sm_fold(int n, void **b)
{
	void **ret;

	if (!b)
		return 0;
	ret = memcpy(malloc((n + 1) * sizeof(void*)), b, n * sizeof(void*));
	ret[n] = 0;
	return ret;
}
