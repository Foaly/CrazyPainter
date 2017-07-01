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

#include "../PathResolver.hpp"
#import <Foundation/Foundation.h>

// On OSX find the path the bundle resources
// and append the original path to it

std::string resolvePath(std::string originalPath)
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    std::string result;
    NSBundle* bundle = [NSBundle mainBundle];

    if (bundle == nil) {
#ifdef DEBUG
        NSLog(@"bundle is nil. Path to bundle resources can not be found. Returning input string.");
#endif
    } else {
        NSString* originalPathNS = [NSString stringWithUTF8String:originalPath.c_str()];
        NSString* bundleResourcePath = [bundle resourcePath];
        result = [bundleResourcePath UTF8String] + std::string("/") + [originalPathNS UTF8String];
    }

    [pool drain];

    return result;
}
