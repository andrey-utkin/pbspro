/*
 * Copyright (C) 1994-2019 Altair Engineering, Inc.
 * For more information, contact Altair at www.altair.com.
 *
 * This file is part of the PBS Professional ("PBS Pro") software.
 *
 * Open Source License Information:
 *
 * PBS Pro is free software. You can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Commercial License Information:
 *
 * For a copy of the commercial license terms and conditions,
 * go to: (http://www.pbspro.com/UserArea/agreement.html)
 * or contact the Altair Legal Department.
 *
 * Altair’s dual-license business model allows companies, individuals, and
 * organizations to create proprietary derivative works of PBS Pro and
 * distribute them - whether embedded or bundled with other software -
 * under a commercial license agreement.
 *
 * Use of Altair’s trademarks, including but not limited to "PBS™",
 * "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's
 * trademark licensing policies.
 *
 */

#include <pbs_config.h>   /* the master config generated by configure */

#if defined(PBS_SECURITY) && (PBS_SECURITY == KRB5)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "pbs_gss.h"

extern char server_host[];

extern int __tcp_gss_process(int sfds, char *hostname, char *ebuf, int ebufsz);

#define ERR_BUF_SIZE 4096

void
tcp_gss_logerror(const char *func_name, const char* msg)
{
	log_event(PBSEVENT_ERROR | PBSEVENT_FORCE, PBS_EVENTCLASS_SERVER, LOG_ERR, func_name, msg);
}

void
tcp_gss_logdebug(const char *func_name, const char* msg)
{
	log_event(PBSEVENT_DEBUG | PBSEVENT_FORCE, PBS_EVENTCLASS_SERVER, LOG_DEBUG, func_name, msg);
}

/**
 * @brief
 *	Log GSS-API messages associated with maj_stat or min_stat
 *
 * @param[in] m - error message followed by GSS maj or min message
 * @param[in] code - gss error code
 * @param[in] type - type of gss error code
 */
static void
log_status_1(const char *m, OM_uint32 code, int type)
{
	OM_uint32 min_stat;
	gss_buffer_desc msg;
	OM_uint32 msg_ctx;
	msg_ctx = 0;

	do {
		gss_display_status(&min_stat, code, type, GSS_C_NULL_OID, &msg_ctx, &msg);
		log_eventf(PBSEVENT_ERROR, PBS_EVENTCLASS_SERVER, LOG_ERR,
			msg_daemonname, "%s : %.*s\n", m, (int)msg.length, (char *)msg.value);
		(void) gss_release_buffer(&min_stat, &msg);
	} while (msg_ctx != 0);
}

/**
 * @brief
 *	The GSS-API messages associated with maj_stat and min_stat are
 *	logged, each preceeded by "GSS-API error <msg>: ".
 *
 * @param[in] msg - a error string to be displayed with the message
 * @param[in] maj_stat - the GSS-API major status code
 * @param[in] min_stat - the GSS-API minor status code
 */
void
tcp_gss_log_status(const char *msg, OM_uint32 maj_stat, OM_uint32 min_stat)
{
	log_status_1(msg, maj_stat, GSS_C_GSS_CODE);
	log_status_1(msg, min_stat, GSS_C_MECH_CODE);
}

/**
 * @brief
 *	Wrapper for __tcp_gss_process. First, the log handlers are set.
 *
 * @param[in] sfds - socket descriptor
 *
 * @return	int
 * @retval	>0	data ready
 * @retval	0	no data ready
 * @retval	-1	error
 * @retval	-2	on EOF
 */
int
tcp_gss_process(int sfds)
{
	char errbuf[ERR_BUF_SIZE];
	int rc;

	errbuf[0] = '\0';

	pbs_gss_set_log_handlers(tcp_gss_log_status, tcp_gss_logerror, tcp_gss_logdebug);

	rc = __tcp_gss_process(sfds, server_host, errbuf, sizeof(errbuf));
	if (errbuf[0] != '\0')
		tcp_gss_logerror(__func__, errbuf);

	return rc;
}

#endif
