
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;


void core::make_repl_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("repl/completion",
		[](instance<Binding> a, instance<std::string> b) -> instance<List>
	{
		auto res = instance<List>::make();

		// Custom Completion Logic
		auto completion = a->getMeta("completion");
		if (completion)
		{
			if (completion.typeId().isType<MultiMethod>())
			{
				auto _res = Execution::exec(completion.asType<MultiMethod>(), { b });
				if (_res.typeId().isType<List>()) return _res.asType<List>();
				else return res;
			}
			else if (completion.typeId().isType<Function>())
			{
				auto _res = Execution::exec(completion.asType<Function>(), { b });
				if (_res.typeId().isType<List>()) return _res.asType<List>();
				else return res;
			}
			else if (completion.typeId().isType<List>())
			{
				return completion.asType<List>();
			}
		}

		//else
		//{
		//	
		//	auto target = a->getSymbol();
		//	auto parse = lisp::partial_parse(*b).top();

		//	auto s = parse->cells.back();
		//	if (s.typeId().isType<Symbol>())
		//	{
		//		auto sym = s.asType<Symbol>();
		//		auto bindings = fr//frame->getNamespace()->search(sym->value);

		//		if (target.typeId().isType<MultiMethod>())
		//		{
		//			auto mm = target.asType<MultiMethod>()->signatures();
		//			for (auto b : bindings)
		//			{
		//				bool found = false;
		//				bool any = false; // TODO If the ith argtype is any, then all bindings which match the name must be added
		//				for (auto sig : mm)
		//				{
		//					if (sig->arguments.size() >= parse->cells.size())
		//					{
		//						auto argType = sig->arguments[parse->cells.size() - 1]->type;

		//						if (argType.typeId().isType<lisp::types::CraftType>())
		//						{
		//							auto bindingId = b->getValue(frame).typeId();
		//							auto argtypeId = argType.asType<lisp::types::CraftType>()->type;
		//							if (bindingId == argtypeId)
		//							{
		//								res->insert(instance<int64_t>::make(0), instance<std::string>::make(b->name()));

		//							}
		//						}
		//						found = true;
		//						break;
		//					}

		//				}
		//				if (!found)
		//				{
		//					res->push(instance<std::string>::make(b->name()));
		//				}
		//			}

		//		}
		//		else if (target.typeId().isType<Function>())
		//		{
		//			/*auto _res = s.asType<Function>()->call(frame, { b });
		//			if (_res.typeId().isType<List>()) return _res.asType<List>();
		//			else return res;*/
		//		}
		//		else
		//		{
		//			return res;
		//		}
		//	}

		//}

		return res;
	});

	semantics->builtin_implementMultiMethod("repl/completion",
		[](instance<std::string> a, instance<int64_t> b) -> instance<List>
	{
		auto res = instance<List>::make();

		auto eol = a->at(a->size() - 1);
		if(eol ==  ' ') return res;

		auto parse = lisp::partial_parse(*a);
		

		//auto last = parse.top();
		//if (last->cells.size() == 0) return res;
		//else if (last->cells.size() == 1)
		//{
		//	auto s = last->cells[0];
		//	if (s.typeId().isType<Symbol>())
		//	{
		//		auto sym = s.asType<Symbol>();

		//		auto choices = frame->getNamespace()->search(sym->value);
		//		for (auto s : choices)
		//		{
		//			res->push(instance<std::string>::make(s->name()));
		//		}
		//	}
		//}
		//else
		//{
		//	auto s = last->cells[0];
		//	// We know the first symbol, so dispatch to the symbol lookup
		//	if (s.typeId().isType<Symbol>())
		//	{
		//		auto env = frame->getNamespace()->environment();
		//		auto sym = s.asType<Symbol>();
		//		
		//		try
		//		{
		//			auto target = frame->getNamespace()->lookup(sym->value);

		//			auto query = instance<std::string>::make(a->substr(a->find(sym->value) + sym->value.size()));
		//			auto repl = env->eval(env->ns_user->lookup("completion"));
		//			auto disp = repl.asType<craft::lisp::MultiMethod>()->call(frame, { target, query });
		//			if (disp.typeId().isType<List>()) return disp.asType<List>();
		//			else return res;
		//		}
		//		catch (...)
		//		{
		//		}
		//	}
		//}
		return res;
		

	});

}
