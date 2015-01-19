extern	char	*__modemcap;

mgetflag (id)
register char	*id;
{
	register char	*cp = __modemcap;

	if (__modemcap == (char *) 0)		/* has mgetent() been called? ...	*/
		return (-1);			/* ... no, can't find number*/

	while (*cp != ':' && *cp != 0)		/* find first entry in cap*/
		cp++;

	if (*cp == 0)				/* empty entry???	*/
		return (0);			/* ... yes, bad modemcap entry*/
	else
		cp++;				/* point to first character in next	*/

	while (*cp != 0) {			/* until entry found or end of entry	*/
		if (cp[0] == id[0] && cp[1] == id[1]) {	/* found entry!!!*/
			return (1);		/* return true		*/
		} else {			/* not entry, skip this entire entry	*/
			while (*cp != ':' && *cp != 0)
				cp++;		/* search for end of current entry	*/

			if (*cp != 0)
				cp++;		/* skip terminating character*/
		}
	}
	return (0);
}