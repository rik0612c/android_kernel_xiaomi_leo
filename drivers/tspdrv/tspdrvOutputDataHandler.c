/*
 ** =========================================================================
 ** File:
 **     tspdrvOutputDataHandler.c
 **
 ** Description:
 **     Output data handler module for TSP 3000 Edition.
 **     ?This module supports only one actuator and one sample per update.
 **
 ** Portions Copyright (c) 2011-2014 Immersion Corporation. All Rights Reserved.
 ** Copyright (C) 2018 XiaoMi, Inc.
 **
 ** This file contains Original Code and/or Modifications of Original Code
 ** as defined in and that are subject to the GNU Public License v2 -
 ** (the 'License'). You may not use this file except in compliance with the
 ** License. You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software Foundation, Inc.,
 ** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or contact
 ** TouchSenseSales@immersion.com.
 **
 ** The Original Code and all software distributed under the License are
 ** distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 ** EXPRESS OR IMPLIED, AND IMMERSION HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 ** INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 ** FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see
 ** the License for the specific language governing rights and limitations
 ** under the License.
 ** =========================================================================
 */

VibeUInt8 g_nOutputData = 0;
VibeBool  g_bOutputDataBufferEmpty = 1;

#define SPI_BUFFER_SIZE (1 + SPI_HEADER_SIZE)

int SaveOutputData(const char *outputDataBuffer, int count)
{
	if (count < SPI_HEADER_SIZE) {
		DbgOutErr(("SaveOutputData: invalid write buffer size.\n"));
		return 0;
	}

	if (0 == outputDataBuffer) {
		DbgOutErr(("SaveOutputData: outputDataBuffer invalid.\n"));
		return 0;
	}

	if (count == SPI_HEADER_SIZE) {
		g_bOutputDataBufferEmpty = 1;
	}
	else {
		g_nOutputData = outputDataBuffer[SPI_HEADER_SIZE];
		g_bOutputDataBufferEmpty = 0;
	}

	return 1;
}

bool SendOutputData(void)
{
	if (g_bOutputDataBufferEmpty) {
#ifdef VIBE_RUNTIME_RECORD
		if (atomic_read(&g_bRuntimeRecord)) {
			DbgRecord((0,"0\n"));
		}
#endif
		return 0;
	}
	ImmVibeSPI_ForceOut_SetSamples(0, 8, 1, &g_nOutputData);

	/* Reset the buffer */
	g_nOutputData = 0;
	g_bOutputDataBufferEmpty = 1;

	return 1;
}

void ResetOutputData(void)
{
	g_nOutputData = 0;
	g_bOutputDataBufferEmpty = 1;
}

bool IsOutputDataBufferEmpty(void)
{
	return g_bOutputDataBufferEmpty;
}
