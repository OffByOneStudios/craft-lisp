
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void system::make_repl_globals(instance<Module>& ret, instance<Namespace>& ns)
{
	auto env = ns->environment();

	auto _complete = instance<MultiMethod>::make();

	_complete->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<Binding, std::string, List>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<Binding> a(expect<Binding>(args[0]));
		instance<std::string> b(expect<std::string>(args[1]));
		auto res = instance<List>::make();

		// Custom Completion Logic
		auto completion = a->getMeta("completion");
		if (completion)
		{
			if (completion.typeId().isType<Symbol>())
			{
				auto s = frame->getNamespace()->lookup(completion.asType<Symbol>()->value)->getValue(frame);
				auto kind = s.getFeature<PIdentifier>()->identifier();
				if (s.typeId().isType<MultiMethod>())
				{
					auto _res = s.asType<MultiMethod>()->call(frame, { b });
					if (_res.typeId().isType<List>()) return _res.asType<List>();
					else return res;
				}
				else if (s.typeId().isType<Function>())
				{
					auto _res = s.asType<Function>()->call(frame, { b });
					if (_res.typeId().isType<List>()) return _res.asType<List>();
					else return res;
				}
				else
				{
					return res;
				}
			}
		}

		else
		{
			auto target = a->getValue(frame);
			auto parse = lisp::partial_parse(*b).top();

			auto s = parse->cells.back();
			if (s.typeId().isType<Symbol>())
			{
				auto sym = s.asType<Symbol>();
				auto bindings = frame->getNamespace()->search(sym->value);

				if (target.typeId().isType<MultiMethod>())
				{
					auto mm = target.asType<MultiMethod>()->signatures();
					for (auto b : bindings)
					{
						bool found = false;
						bool any = false; // TODO If the ith argtype is any, then all bindings which match the name must be added
						for (auto sig : mm)
						{
							if (sig->arguments.size() >= parse->cells.size())
							{
								auto argType = sig->arguments[parse->cells.size() - 1]->type;
								
								if (argType.typeId().isType<lisp::types::CraftType>())
								{
									auto bindingId = b->getValue(frame).typeId();
									auto argtypeId = argType.asType<lisp::types::CraftType>()->type;
									if (bindingId == argtypeId)
									{
										res->insert(instance<int64_t>::make(0), instance<std::string>::make(b->name()));
										
									}
								}
								found = true;
								break;
							}
							
						}
						if (!found)
						{
							res->push(instance<std::string>::make(b->name()));
						}
					}
					
				}
				else if (target.typeId().isType<Function>())
				{
					/*auto _res = s.asType<Function>()->call(frame, { b });
					if (_res.typeId().isType<List>()) return _res.asType<List>();
					else return res;*/
				}
				else
				{
					return res;
				}
			}
			
		}

		return res;
	}));

	_complete->attach(env, instance<BuiltinFunction>::make(
		SubroutineSignature::makeFromArgsAndReturn<std::string, int64_t, List>(),
		[](instance<SFrame> frame, auto args)
	{
		instance<std::string> a(expect<std::string>(args[0]));
		instance<int64_t> b(expect<int64_t>(args[1]));
		auto res = instance<List>::make();

		auto eol = a->at(a->size() - 1);
		if(eol ==  ' ') return res;

		auto parse = lisp::partial_parse(*a);
		

		auto last = parse.top();
		if (last->cells.size() == 0) return res;
		else if (last->cells.size() == 1)
		{
			auto s = last->cells[0];
			if (s.typeId().isType<Symbol>())
			{
				auto sym = s.asType<Symbol>();

				auto choices = frame->getNamespace()->search(sym->value);
				for (auto s : choices)
				{
					res->push(instance<std::string>::make(s->name()));
				}
			}
		}
		else
		{
			auto s = last->cells[0];
			// We know the first symbol, so dispatch to the symbol lookup
			if (s.typeId().isType<Symbol>())
			{
				auto env = frame->getNamespace()->environment();
				auto sym = s.asType<Symbol>();
				
				try
				{
					auto target = frame->getNamespace()->lookup(sym->value);

					auto query = instance<std::string>::make(a->substr(a->find(sym->value) + sym->value.size()));
					auto repl = env->eval(env->ns_user->lookup("completion"));
					auto disp = repl.asType<craft::lisp::MultiMethod>()->call(frame, { target, query });
					if (disp.typeId().isType<List>()) return disp.asType<List>();
					else return res;
				}
				catch (...)
				{
				}
			}
		}
		
		
		/*if (size_t(*b) < a->length())
		{
			auto query = a->substr(size_t(*b));
			auto choices = frame->getNamespace()->search(query);
			for (auto s : choices)
			{
				res->push(instance<std::string>::make(s->name()));
			}
		}
		else
		{
			auto lp = a->rfind('(');
			if (lp != a->npos)
			{
				auto n = a->find(' ', lp);
				auto symbol = a->substr(lp + 1, n - 1);
				try
				{
					auto b = frame->getNamespace()->lookup(symbol);
					auto completer = b->getMeta("completion");

					instance<Function> target;
					if (completer.typeId().isType<Symbol>())
					{
						auto s = frame->getNamespace()->lookup(completer.asType<Symbol>()->value)->getValue(frame);
						auto kind = s.getFeature<PIdentifier>()->identifier();
						if (s.typeId().isType<MultiMethod>())
						{
							auto _res = s.asType<MultiMethod>()->call(frame, {});
							if (_res.typeId().isType<List>()) return _res.asType<List>();
							else return res;
						}
						else if (s.typeId().isType<Function>())
						{
							auto _res = s.asType<Function>()->call(frame, {});
							if (_res.typeId().isType<List>()) return _res.asType<List>();
							else return res;
						}
						else
						{
							return res;
						}	
					}
				}
				catch(stdext::exception e)
				{
					frame->getNamespace()->environment()->log()->error(e.what());
				}
			}
			

		}*/
		return res;
		

	}));
	ret->define_eval("completion", _complete);

	
}
