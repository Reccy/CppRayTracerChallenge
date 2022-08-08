#pragma once

namespace CppRayTracerChallenge::Core::Helpers
{
	template<typename Base, typename Derived>
	class PolymorphicComparable : public Base
	{
		bool operator==(const Base& other) const
		{
			if (typeid(other) != typeid(Derived))
				return false;

			const Derived& derivedA = static_cast<const Derived&>(*this);
			const Derived& derivedB = static_cast<const Derived&>(other);

			return derivedA == derivedB;
		}
	};
}
