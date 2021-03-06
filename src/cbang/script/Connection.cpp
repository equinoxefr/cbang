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

#include "Connection.h"

#include "Server.h"

#include <cbang/socket/SocketDevice.h>
#include <cbang/log/Logger.h>

#include <boost/iostreams/filtering_stream.hpp>

using namespace std;
using namespace cb;
using namespace cb::Script;
namespace io = boost::iostreams;


bool Connection::isFinished() const {
  return getState() == THREAD_DONE || getState() == THREAD_STOPPED;
}


void Connection::run() {
  try {
#ifdef _WIN32
    // NOTE Works around socket write not blocking when buffers are full.
    socket->setSendBuffer(0);
#endif
    socket->setSendTimeout(5);
    socket->setReceiveTimeout(0.001);
    socket->setKeepAlive(true);
    SocketStream stream(*socket, true);

    Processor::run(*server, stream, stream);

  } catch (const Exception &e) {
    LOG_ERROR(e);
  }

  LOG_DEBUG(2, "Closing connection " << getID());

  socket->close();
}


void Connection::stop() {
  Processor::quit = true;
  Thread::stop();
}
