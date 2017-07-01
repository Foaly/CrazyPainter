/// CrazyPainter - Colorfull psychedelic lines
/// Copyright (C) 2012 - 2017  Foaly

/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.

/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.

/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef RESOLVE_PATH_HPP
#define RESOLVE_PATH_HPP

#include <string>

////////////////////////////////////////////////////////////
/// \brief This function takes a path to a resource (like: data/images/image.png)
///        On OSX the path to the resource folder
///        associated with the main bundle is prepended.
///        On windows the input path is simply returned.
////////////////////////////////////////////////////////////
std::string resolvePath(std::string);

#endif // RESOLVE_PATH_HPP
