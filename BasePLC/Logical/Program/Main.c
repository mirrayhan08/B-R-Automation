
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	Test = 1;
}

void _CYCLIC ProgramCyclic(void)
{
	if (Test == 1)
		Test = 0;
	else
		Test = 1;
}

void _EXIT ProgramExit(void)
{

}

