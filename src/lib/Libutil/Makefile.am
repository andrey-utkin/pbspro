
#
# Copyright (C) 1994-2019 Altair Engineering, Inc.
# For more information, contact Altair at www.altair.com.
#
# This file is part of the PBS Professional ("PBS Pro") software.
#
# Open Source License Information:
#
# PBS Pro is free software. You can redistribute it and/or modify it under the
# terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.
# See the GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Commercial License Information:
#
# For a copy of the commercial license terms and conditions,
# go to: (http://www.pbspro.com/UserArea/agreement.html)
# or contact the Altair Legal Department.
#
# Altair’s dual-license business model allows companies, individuals, and
# organizations to create proprietary derivative works of PBS Pro and
# distribute them - whether embedded or bundled with other software -
# under a commercial license agreement.
#
# Use of Altair’s trademarks, including but not limited to "PBS™",
# "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's
# trademark licensing policies.
#

noinst_LIBRARIES = libutil.a

libutil_a_CPPFLAGS = \
	-I$(top_srcdir)/src/include \
	@libical_inc@ @KRB5_CFLAGS@ \
	@libundolr_inc@

libutil_a_SOURCES = \
	execvnode_seq_util.c \
	pbs_ical.c \
	misc_utils.c \
	avltree.c \
	hook.c \
	work_task.c \
	entlim.c \
	daemon_protect.c \
	pbs_array_list.c \
	pbs_secrets.c \
	pbs_aes_encrypt.c \
	munge_supp.c \
	krb5_util.c \
	pbs_gss.c

if UNDOLR_ENABLED
libutil_a_SOURCES += undolr.c
endif

