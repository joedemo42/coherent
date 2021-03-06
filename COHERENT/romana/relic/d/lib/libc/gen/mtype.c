/*
 * Routine to return a symbolic name
 * given an integer machine type.
 * Eventually, this may return other
 * information about the machine for
 * loaders, etc.
 */

#include <stdio.h>
#include <mtype.h>

#define	NTYPE	(sizeof types/sizeof types[0])

static	struct	types {
	int	m_type;
	char	*m_name;
}	types[] = {
	M_PDP11,	"PDP-11",
	M_VAX,		"VAX",
	M_360,		"IBM 370",
	M_Z8001,	"Z-8001",
	M_Z8002,	"Z-8002",
	M_8086,		"Intel 8086",
	M_8080,		"Intel 8080",
	M_6800,		"Motorola 6800",
	M_6809,		"Motorola 6809",
	M_68000,	"Motorola 68000"
};

char *
mtype(m)
int m;
{
	register struct types *mtp;

	for (mtp = types; mtp < &types[NTYPE]; mtp++)
		if (mtp->m_type == m)
			return (mtp->m_name);
	return (NULL);
}
