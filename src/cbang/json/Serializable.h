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

#ifndef CB_JSON_SERIALIZABLE_H
#define CB_JSON_SERIALIZABLE_H

#include <cbang/SStream.h>

#include <iostream>
#include <sstream>


namespace cb {
  namespace JSON {
    class Value;
    class Sync;

    class Serializable {
    public:
      virtual ~Serializable() {}

      virtual void read(const Value &value) = 0;
      virtual void write(Sync &sync) const = 0;

      virtual void read(std::istream &stream);
      virtual void write(std::ostream &stream) const;

      std::string toString() const {
        std::ostringstream str;
        write(str);
        return str.str();
      }

      void parse(const std::string &s) {
        std::istringstream str(s);
        read(str);
      }
    };

    inline static
    Sync &operator<<(Sync &sync, const Serializable &s) {
      s.write(sync);
      return sync;
    }

    inline static
    const Value &operator>>(const Value &value, Serializable &s) {
      s.read(value);
      return value;
    }

    inline static
    std::ostream &operator<<(std::ostream &stream, const Serializable &s) {
      s.write(stream);
      return stream;
    }

    inline static
    std::istream &operator>>(std::istream &stream, Serializable &s) {
      s.read(stream);
      return stream;
    }
  }
}

#endif // CB_JSON_SERIALIZABLE_H

