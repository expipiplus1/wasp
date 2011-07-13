#!/usr/bin/python

import copy
import re
import sys

#
# Consts
#

type_regex = "(?P<Modifiers>(?:(?:const|unsigned|signed|long|short|static|volatile|mutable|virtual)\\s+?)*)\\s*(?P<Type>(?:[a-zA-Z_][\\w<>:]*)(?:\\s+(?:const))*\\s*?(?:\\*\\s*(?:const)?)*(?:\\s*&)?)"

optional_type_regex = "(?P<Modifiers>(?:(?:const|unsigned|signed|long|short|static|volatile|mutable|virtual)\\s+?)*)?\\s*(?P<Type>(?:[a-zA-Z_][\\w<>:]*)(?:\\s+(?:const))*\\s*?(?:\\*\\s*(?:const)?)*(?:\\s*&)?)?"

method_or_member_regex = "\\s*" + optional_type_regex + "\\s+" + "(?P<Name>(?:~?[a-zA-Z_]\\w*)|(?:operator\\s*(?:=|\\+|-|\\*|/|%|\\+\\+|--|==|!=|<|>|<=|>=|!|&&|\\|\\||~|&|\\||\\^|<<|>>|\\+=|-=|\\*=|/=|%=|&=|\\|=|\\^=|<<=|>>=|\\[\\s*]|->|->\\*|,|new|new\\s*\\[\\s*]|delete|delete\\s*\\[\\s*])))" + "\\s*" + "(?P<Parentheses>\((?P<Params>.*\\s*)\))?" + "\\s*" + "(?P<Const>const)?" + "\\s*" + "(?P<Init>=\\s*(?:default|delete|0))?" + "\\s*;\\s*$"

parameter_regex = "\\s*" + type_regex + "\\s+" + "(?P<Name>(?:[a-zA-Z_]\\w*))?" + "\\s*(?:,|$)"

function_pattern = re.compile( method_or_member_regex )
parameter_pattern = re.compile( parameter_regex )

file = []
functions = []

modifier_offset    = 99999
name_offset        = 0
params_offset      = 0
param_type_offset  = 0
param_name_offset  = 0
const_offset       = 0
initializer_offset = 0

#
# Functions
#

def GoToNextTabstop( i ):
    return ( ( i + 3 ) / 4 ) * 4

def LineLen( s ):
    return len( s ) - s.rfind( "\n" ) - 1
                                                                      
#
# Classes
#

class Param:
    def __init__( self ):
        self.m_type = ""
        self.m_name = ""

    def CondenseWhitespace( self ):
        self.m_type = self.m_type.strip()
        self.m_name = self.m_name.strip()
        self.m_type = re.sub( "\\s+", " ", self.m_type )
        self.m_name = re.sub( "\\s+", " ", self.m_name )

    def GetString( self ):
        return self.m_type + " " + self.m_name

    def __str__( self ):
        return self.m_type + " " + self.m_name

class Function:
    def __init__( self ):
        self.m_original_line = 0
        self.m_original_indentation = 0
        self.m_params = ""
        self.m_param_list = []
        self.m_modifiers = ""
        self.m_type = ""
        self.m_name = ""
        self.m_const = ""
        self.m_initializer = ""
        self.m_isFunction = False

    def Read( self, line_number ):
        line = file[line_number]
        match = function_pattern.match( line )
        if not match:
            return False
        else:
            self.m_original_line = line_number
            self.m_original_indentation = match.start( "Modifiers" )
            if match.group( "Modifiers" ):
                self.m_modifiers = match.group( "Modifiers" )
            if match.group( "Type" ):
                self.m_type = match.group( "Type" )
            self.m_name = match.group( "Name" )
            if match.group( "Params" ):
                self.m_params = match.group( "Params" )
            if match.group( "Const" ):
                self.m_const = match.group( "Const" )
            if match.group( "Init" ):
                self.m_initializer = match.group( "Init" )

            self.m_isFunction = match.group( "Parentheses" )
            
            parameters = parameter_pattern.findall( self.m_params )
            for p in parameters:
                param = Param()
                param.m_type = p[parameter_pattern.groupindex["Modifiers"] - 1] + " " + \
                               p[parameter_pattern.groupindex["Type"] - 1]
                param.m_name = p[parameter_pattern.groupindex["Name"] - 1]
                self.m_param_list.append( param )

            return True;

    def GetString( self ):
        return  self.m_modifiers + " " + \
                self.m_type + " " + \
                self.m_name + " " + \
                self.m_params + " " + \
                self.m_const + " " + \
                self.m_initializer + ";"

    def GetAlignedString( self ):
        ret = ""
        for i in range( 0, modifier_offset ):
            ret += " "
        if len( self.m_modifiers ) != 0:
            ret += self.m_modifiers
            ret += " "
        ret += self.m_type
        for i in range( len(ret), name_offset ):
            ret += " "
        ret += self.m_name
        if self.m_isFunction:
            for i in range( len(ret), params_offset ):
                ret += " "
            ret += "("
            for p in enumerate( self.m_param_list ):
                line = ""
                if ( p[0] == 0 ):
                    for i in range( len(ret), param_type_offset ):
                        ret += " "
                    ret += p[1].m_type
                    for i in range( len(ret), param_name_offset ):
                        ret += " "
                    ret += p[1].m_name
                else:
                    for i in range( len(line), param_type_offset ):
                        line += " "
                    line += p[1].m_type
                    for i in range( len(line), param_name_offset ):
                        line += " "
                    line += p[1].m_name
                if p[0] != len( self.m_param_list ) - 1:
                    line += ",\n"
                ret += line
            ret += " )"
        if len( self.m_const ) != 0:
        #    for i in range( LineLen(ret), const_offset ):
        #        ret += " "
            ret += " " + self.m_const
        if len( self.m_initializer ) != 0:
            for i in range( LineLen(ret), initializer_offset ):
                ret += " "
            ret += self.m_initializer
        ret += ";"
        return ret

    def CondenseWhitespace( self ):
        self.m_params = self.m_params.strip()
        self.m_modifiers = self.m_modifiers.strip()
        self.m_type = self.m_type.strip()
        self.m_name = self.m_name.strip()
        self.m_const = self.m_const.strip()
        self.m_initializer = self.m_initializer.strip()

        self.m_params       = re.sub( "\\s+", " ", self.m_params )
        self.m_modifiers    = re.sub( "\\s+", " ", self.m_modifiers )
        self.m_type         = re.sub( "\\s+", " ", self.m_type )
        self.m_name         = re.sub( "\\s+", " ", self.m_name )
        self.m_const        = re.sub( "\\s+", " ", self.m_const )
        self.m_initializer  = re.sub( "\\s+", " ", self.m_initializer )

        for p in self.m_param_list:
            p.CondenseWhitespace()

    def __str__( self ):
        return self.GetString()
#
# Main
#

for line in sys.stdin.readlines():
    file.append(line);

max_original_indentation = 0

max_modifier_type_length = 0
max_name_length     = 0
max_params_length   = 0
max_const_length    = 0

max_num_params = 0
max_param_type_length = 0
max_param_name_length = 0

for i in range( 0, len(file) ):
    f = Function()
    if f.Read( i ):
        f.CondenseWhitespace()

        max_original_indentation = max( max_original_indentation, f.m_original_indentation )

        max_modifier_type_length = max( max_modifier_type_length, len( f.m_modifiers ) + len( f.m_type) )
        max_name_length     = max( max_name_length,     len( f.m_name ) )
        max_params_length   = max( max_params_length,   len( f.m_params ) )
        max_const_length    = max( max_const_length,    len( f.m_const ) )

        max_num_params = max( max_num_params, len( f.m_param_list ) )

        for p in f.m_param_list:
            max_param_type_length = max( max_param_type_length, len( p.m_type ) )
            max_param_name_length = max( max_param_name_length, len( p.m_name ) )

        modifier_offset = min( modifier_offset, f.m_original_indentation )
        
        functions.append( f )

name_offset         = GoToNextTabstop( modifier_offset + max_modifier_type_length + 2 )
params_offset       = GoToNextTabstop( name_offset + max_name_length + 1 )
param_type_offset   = params_offset + 2
param_name_offset   = param_type_offset + max_param_type_length + 1
const_offset        = GoToNextTabstop( param_name_offset + max_param_name_length + 3 )
initializer_offset  = GoToNextTabstop( const_offset + max_const_length + 1)

for i in range( 0, len(file) ):
    f = None
    for j in functions:
        if j.m_original_line == i:
            f = j
            break
    if not f:
        sys.stdout.write( file[i] )
    else:
        sys.stdout.write( f.GetAlignedString() + "\n" )

