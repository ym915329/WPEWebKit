/*
 * Copyright (C) 2004, 2005, 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006 Rob Buis <buis@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "SVGLengthList.h"

#include "SVGParserUtilities.h"
#include <wtf/text/StringBuilder.h>

namespace WebCore {

void SVGLengthList::parse(const String& value, SVGLengthMode mode)
{
    clear();

    auto upconvertedCharacters = StringView(value).upconvertedCharacters();
    const UChar* ptr = upconvertedCharacters;
    const UChar* end = ptr + value.length();
    while (ptr < end) {
        const UChar* start = ptr;
        while (ptr < end && *ptr != ',' && !isSVGSpace(*ptr))
            ptr++;
        if (ptr == start)
            break;

        SVGLength length(mode);
        String valueString(start, ptr - start);
        if (valueString.isEmpty())
            return;
        if (length.setValueAsString(valueString).hasException())
            return;
        append(length);
        skipOptionalSVGSpacesOrDelimiter(ptr, end);
    }
}

String SVGLengthList::valueAsString() const
{
    StringBuilder builder;

    unsigned size = this->size();
    for (unsigned i = 0; i < size; ++i) {
        if (i > 0)
            builder.append(' ');

        builder.append(at(i).valueAsString());
    }

    return builder.toString();
}

}
