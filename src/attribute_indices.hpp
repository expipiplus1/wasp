/*
    Copyright 2011 Joe Hermaszewski. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY Joe Hermaszewski "AS IS" AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Joe Hermaszewski OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those of the
    authors and should not be interpreted as representing official policies, either expressed
    or implied, of Joe Hermaszewski.
*/

#pragma once

#ifndef ATTRIBUTE_INDICES_HPP
#define ATTRIBUTE_INDICES_HPP

#define POSITION_INDEX    0
#define NORMAL_INDEX      1
#define TANGENT_INDEX     2
#define BITANGENT_INDEX   3
#define TEXCOORD_INDEX    4
#define COLOR_INDEX       12

#define POSITION_ATTRIBUTE ATTR0
#define NORMAL_ATTRIBUTE ATTR1
#define TANGENT_ATTRIBUTE ATTR2
#define BITANGENT_ATTRIBUTE ATTR3
#define TEXCOORD0_ATTRIBUTE ATTR4
#define TEXCOORD1_ATTRIBUTE ATTR5
#define TEXCOORD2_ATTRIBUTE ATTR6
#define TEXCOORD3_ATTRIBUTE ATTR7
#define TEXCOORD4_ATTRIBUTE ATTR8
#define TEXCOORD5_ATTRIBUTE ATTR9
#define TEXCOORD6_ATTRIBUTE ATTR10
#define TEXCOORD7_ATTRIBUTE ATTR11
#define COLOR0_ATTRIBUTE ATTR12
#define COLOR1_ATTRIBUTE ATTR13
#define COLOR2_ATTRIBUTE ATTR14
#define COLOR3_ATTRIBUTE ATTR15

//
// Formats
//

const int DEPTH32 = 0;
const int RGBA32F = 1;

#endif // ATTRIBUTE_INDICES_HPP
