/*****************************************************************************\
 *  $Id: tool-hostrange-common.h,v 1.1 2008-09-24 04:00:29 chu11 Exp $
\*****************************************************************************/

#ifndef _TOOL_HOSTRANGE_COMMON_H
#define _TOOL_HOSTRANGE_COMMON_H

/* Returns number of hosts setup for, -1 on error */
int pstdout_setup(char **hosts,
                  int buffer_output,
                  int consolidate_output,
                  int fanout,
                  int eliminate,
                  int always_prefix);

#endif /* _TOOL_HOSTRANGE_COMMON_H */