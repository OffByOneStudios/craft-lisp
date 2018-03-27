#pragma once
#include "common.h"

namespace craft {
namespace lisp
{
	// All errors in this library throw this
	class lang_error : public stdext::exception
	{
	public:
		using stdext::exception::exception;
	};

	// An error using the language implementation
	class usage_error : public lang_error
	{
	public:
		using lang_error::lang_error;
	};

	// An error getting a projection out of a class. Commonly when getting a
	//  specific type of object off of another.
	class bad_projection_error : public usage_error
	{
	public:
		using usage_error::usage_error;
	};

	// An error resolving a module uri
	class module_resolve_error : public usage_error
	{
	public:
		using usage_error::usage_error;
	};
}}
