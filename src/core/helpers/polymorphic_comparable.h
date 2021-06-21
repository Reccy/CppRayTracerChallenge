#ifndef _CPPRAYTRACERCHALLENGE_CORE_HELPERS_POLYMORPHIC_COMPARABLE
#define _CPPRAYTRACERCHALLENGE_CORE_HELPERS_POLYMORPHIC_COMPARABLE

namespace CppRayTracerChallenge::Core::Helpers
{
	template<typename Base, typename Derived>
	class PolymorphicComparable : public Base
	{
		bool operator==(const Base& other) const
		{
			if (typeid(other) != typeid(Derived))
				return false;

			const Derived& a = static_cast<const Derived&>(*this);
			const Derived& b = static_cast<const Derived&>(other);

			return a == b;
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_HELPERS_POLYMORPHIC_COMPARABLE
