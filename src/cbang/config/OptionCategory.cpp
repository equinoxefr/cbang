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

#include "OptionCategory.h"

#include "Option.h"

#include <cbang/String.h>

using namespace std;
using namespace cb;


void OptionCategory::add(const SmartPointer<Option> &option) {
  options.insert(options_t::value_type(option->getName(), option));
}


void OptionCategory::write(XMLHandler &handler, uint32_t flags) const {
  bool first = true;

  options_t::const_iterator it;
  for (it = options.begin(); it != options.end(); it++) {
    Option &option = *it->second;

    // Don't print options set on the command line
    if (option.isCommandLine() && !(flags & Option::COMMAND_LINE_FLAG) &&
        (!option.hasDefault() || !(flags & Option::DEFAULT_SET_FLAG)))
      continue;

    if (((flags & Option::DEFAULT_SET_FLAG) && option.hasValue()) ||
        (option.isSet() && !option.isDefault())) {
      if (first) {
        first = false;
        if (!name.empty()) handler.comment(name);
      }

      option.write(handler, flags);
    }
  }

  if (!first) handler.text("\n");
}


void OptionCategory::printHelp(ostream &stream) const {
  if (!name.empty()) stream << name << ":\n";
  if (!description.empty()) stream << description << "\n";

  bool first = true;
  options_t::const_iterator it;
  for (it = options.begin(); it != options.end(); it++) {
    const string &name = it->second->getName();

    if (!name.empty() && name[0] != '_') {
      if (first) first = false;
      else stream << "\n\n";
      it->second->printHelp(stream, false);
    }
  }
}
