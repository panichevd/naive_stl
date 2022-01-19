#include "Utility.h"

using namespace naive;

int main()
{
	IntegerSequence<long, 3, -7, 8> integer_seq;
	IndexSequence<3, 6, 8> index_seq;

	auto int_s = MakeIntegerSequence<int, 30>();
	auto ind_s = MakeIndexSequence<20>();

	auto isf = IndexSequenceFor<float, int, char*>();

	return 0;
}
