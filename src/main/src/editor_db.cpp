#include "editor_db.h"

#include <set>
#include <random>

namespace EditorDB
{
	unsigned int GenerateUniqueID()
	{
		static std::set<unsigned int> occupiedInts;

		static std::random_device dev;
		static std::mt19937_64 rng(dev());
		static std::uniform_int_distribution<std::mt19937_64::result_type> dist(0);

		unsigned int id;

		do
		{
			id = dist(rng);
		} while (occupiedInts.find(id) != occupiedInts.end());

		return id;
	}
}
