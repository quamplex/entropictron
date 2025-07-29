/**
 * File name: main.cpp
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
 *
 * This file is part of Entropictron.
 *
 * Entropictron is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "MainWindow.h"
//#include "EndDspProxy.h"

#include "RkMain.h"

#ifdef EONTRIPOCTRON_OS_GNU
#include <sys/mman.h>
#endif // EONTRIPOCTRON_OS_GNU

/*static entropictron* createDSP()
{
        //        EndDspProxy* dspProxy = nullptr;
        //        if (ent_create(.....) != ENTROPICTRON_OK) {
        //                ENTROPICTRON_LOG_ERROR("can't create entropictron API");
        //                return nullptr;
        //        }

        return dspProxy;
        }/*/

int main(int argc, char *argv[])
{
        //        auto dsp = createDSP();
        //	if (!dsp) {
        //                ENTROPICTRON_LOG_ERROR("can't create DSP");
        //		exit(EXIT_FAILURE);
        //	}

#ifdef EONTRIPOCTRON_OS_GNU
	if (mlockall(MCL_CURRENT) == -1) {
	        ENTROPICTRON_LOG_INFO("can't lock memory. Please, check your "
                                  << "system settings reladed locked memory"
                                  << "(https://jackaudio.org/faq/linux_rt_config.html)");
        }
#endif // EONTRIPOCTRON_OS_GNU

        RkMain app(argc, argv);

        //auto api = new EntDspProxy(...);
        //if (!api->init()) {
        //        ENTROPICTRON_LOG_ERROR("can't init API");
        //delete api;
        //exit(EXIT_FAILURE);
        //}
        [[maybe_unused]] auto window = new MainWindow(app);
        auto res = app.exec();

#ifdef EONTRIPOCTRON_OS_GNU
        munlockall();
#endif // EONTRIPOCTRON_OS_GNU

        return res;
}
