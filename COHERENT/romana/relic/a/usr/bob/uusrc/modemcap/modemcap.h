/* 
 *      @(#)modemcap.h  1.0
 *
 *      names of variables and whatnots for modemcap file
 */

extern
char    AS, /* True if numbers dialed in ASCII, False for binary digits     */
        DI, /* True if modem can dial numbers, False otherwise              */
        HC, /* True if modem hangs up when DTR drops, False otherwise       */
        TT; /* True if modem uses touchtone by default, False for pulse     */
extern
char    *AT, /* Enter command state when online                             */
        *CS, /* Command start string                                        */
        *CE, /* Command end string - must be present if CS is               */
        *DS, /* Dial command string                                         */
        *DE, /* End of dial command string - must be present if DS is       */
        *CO, /* Connection made at primary baud rate                        */
        *CL, /* Connection made at secondary (lower) baud rate              */
        *IS, /* Initialization string - reset modem to onhook and ready     */
	*HU; /* Hangup command                                              */
extern
int     AD, /* Delay after AT string before next command                    */
        BD, /* Highest communications baud rate                             */
        BL, /* Another, lower baud rate                                     */
        ID; /* Delay time after initialization                              */

/*
 * The dial command is the principle string that must be built.
 * The routines will build a dial command as follows:
 *
 *      <CS><DS><phone-number><DE><CE>
 *
 * Note that the DE and CE strings are present ALWAYS.
 * This procedure will be used to dial phone numbers if the DI flag is true.
 * If this isn't the way to dial numbers,
 * DO NOT SET DI IN THE MODEMCAP FILE!!!
 */

/*
 * a hangup command will be performed as follows:
 *
 *      1). any attention string (AT) will be sent followed by the delay (AD)
 *      2). modem should now be in command state, send hangup (HU) command
 *      3). send initialization string (IS) followed by the delay (ID)
 *
 * It is important that you determine a correct AT and HU string
 *  to perform this function.
 * If the modem hangs up when DTR falls (even if there is a HU string),
 *  declare the flag HC (for Hangup on Close).
 */
