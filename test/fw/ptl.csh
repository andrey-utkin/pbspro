#!/usr/bin/csh
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

# This file will set path variables in case of ptl installation

if ( -f "/etc/debian_version" ) then
	set ptl_prefix_lib=`dpkg -L pbspro-ptl 2>/dev/null | grep -m 1 lib$ 2>/dev/null`
else
	set ptl_prefix_lib=`rpm -ql pbspro-ptl 2>/dev/null | grep -m 1 lib$ 2>/dev/null`
endif
if ( ! $?ptl_prefix_lib ) then
	set python_dir=`/bin/ls -1 ${ptl_prefix_lib}`
	set prefix=`dirname ${ptl_prefix_lib}`

	setenv PATH=${prefix}/bin/:${PATH} 
	setenv PYTHONPATH=${prefix}/lib/${python_dir}/site-packages/:$PYTHONPATH
	unset python_dir
	unset prefix
	unset ptl_prefix_lib
else
	if ( $?PBS_CONF_FILE ) then
		set conf = "$PBS_CONF_FILE"
	else
		set conf = /etc/pbs.conf
	endif
	if ( -r "${conf}" ) then
		# we only need PBS_EXEC from pbs.conf
		set __PBS_EXEC=`grep '^[[:space:]]*PBS_EXEC=' "$conf" | tail -1 | sed 's/^[[:space:]]*PBS_EXEC=\([^[:space:]]*\)[[:space:]]*/\1/'`
		if ( "X${__PBS_EXEC}" != "X" ) then
			# Define PATH and PYTHONPATH for the users
			set PTL_PREFIX=`dirname ${__PBS_EXEC}`/ptl
			set python_dir=`/bin/ls -1 ${PTL_PREFIX}/lib`/site-packages
			if ( $?PATH && -d ${PTL_PREFIX}/bin ) then
				setenv export PATH="${PATH}:${PTL_PREFIX}/bin"
			endif
			if ( $?PYTHONPATH && -d "${PTL_PREFIX}/lib/${python_dir}" ) then 
				setenv PYTHONPATH="${PYTHONPATH}:${PTL_PREFIX}/lib/${python_dir}"
			endif
		endif
		unset __PBS_EXEC
		unset PTL_PREFIX
		unset conf
		unset python_dir
	endif
endif
