/******************************************************************************\

          This file is part of the C! library.  A.K.A the cbang library.

              Copyright (c) 2003-2014, Cauldron Development LLC
                 Copyright (c) 2003-2014, Stanford University
                             All rights reserved.

        The C! library is free software: you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public License
        as published by the Free Software Foundation, either version 2.1 of
        the License, or (at your option) any later version.

        The C! library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the C! library.  If not, see
        <http://www.gnu.org/licenses/>.

        In addition, BSD licensing may be granted on a case by case basis
        by written permission from at least one of the copyright holders.
        You may request written permission by emailing the authors.

                For information regarding this software email:
                               Joseph Coffland
                        joseph@cauldrondevelopment.com

\******************************************************************************/

#include "AsyncCopyStreamToLog.h"

#include "Logger.h"

#include <string.h>

using namespace std;
using namespace cb;


void AsyncCopyStreamToLog::run() {
  Logger::LogStream log =
    Logger::instance().createStream(CBANG_LOG_DOMAIN, CBANG_LOG_INFO_LEVEL(1),
                                    prefix);

  char buffer[4096];
  while (!in.fail() && !log->fail() && !shouldShutdown()) {
    in.getline(buffer, 4095);

    if (in.fail()) break;

    streamsize len = strlen(buffer);
    buffer[len++] = '\n';

    log->write(buffer, len);
    log->flush();
  }
}
