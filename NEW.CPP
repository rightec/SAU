/**
@file		New.cpp
@brief		Support routines for the C++ dynamic memory management.
			
			Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002 Free Software Foundation

			This file is part of GNU CC.

			GNU CC is free software; you can redistribute it and/or modify
			it under the terms of the GNU General Public License as published by
			the Free Software Foundation; either version 2, or (at your option)
			any later version.

			GNU CC is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			GNU General Public License for more details.

			You should have received a copy of the GNU General Public License
			along with GNU CC; see the file COPYING.  If not, write to
			the Free Software Foundation, 59 Temple Place - Suite 330,
			Boston, MA 02111-1307, USA.

			As a special exception, you may use this file as part of a free software
			library without restriction.  Specifically, if other files instantiate
			templates or use macros or inline functions from this file, or you compile
			this file and link it with other files to produce an executable, this
			file does not by itself cause the resulting executable to be covered by
			the GNU General Public License.  This exception does not however
			invalidate any other reasons why the executable file might be covered by
			the GNU General Public License.
					
@author		
@date		
@version	
*/

#include "new.h"

static char safeArea[1000];

/**
new operator
*/
void *operator new (unsigned sz) 
{
void *p;

  /* malloc (0) is unpredictable; avoid it.  */

	if( sz == 0 )
	    sz = 1;
    
  	if( mem_alloc(sz, &p) == False )		// se non ce la fa
		p = NULL;
  
	return p;
}

/**
new operator
*/
void *operator new [](unsigned sz) 
{
void *p;

  /* malloc (0) is unpredictable; avoid it.  */

	if( sz == 0 )
	    sz = 1;
    
  	if( mem_alloc(sz, &p) == False )		// se non ce la fa
		p = NULL;
  
	return p;
}

/**
delete operator
*/
void operator delete (void *__p) 
{

	if( __p != safeArea )
		mem_free(__p);
		
}

/**
delete operator
*/
void operator delete [](void *__p) 
{

	if( __p != safeArea )
		mem_free(__p);
		
}
















