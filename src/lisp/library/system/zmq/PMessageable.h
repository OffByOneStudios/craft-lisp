#pragma once
#include "lisp/common.h"

#include "lisp/library/system/zmq/zmq.h"

namespace craft {
namespace lisp {
namespace library {
	//class PMessageable abstract
	//	: public types::Provider
	//{
	//	CRAFT_LISP_EXPORTED CRAFT_PROVIDER_DECLARE(PMessageable, "zmq.messagable", types::SingletonProviderManager);

	//public:
	//	virtual instance<ZMessage> toMessage(instance<> inst) const = 0;
	//};

	///******************************************************************************
	//** FunctionalMessenger
	//******************************************************************************/

	//namespace _details
	//{
	//	static std::function<instance<ZMessage>(instance<>)> FunctionalMessenger_defaultReport;
	//}

	//template <typename T>
	//class FunctionalMessenger
	//	: public types::Implements<PHashable>::For<T>
	//{
	//	std::function<instance<ZMessage>(instance<>)> _stringer;

	//public:
	//	inline FunctionalMessenger(std::function<ZMessage(instance<T>)> const& messenger)
	//		: _messenger([messenger](instance<> inst) { return messenger(inst.asType<T>()); })
	//	{ }

	//	inline virtual ZMessage (instance<> inst) const override { return _hasher(inst); }
	//};
}}}
