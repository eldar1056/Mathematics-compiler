#include "Numbers.h"
#include "Fundamental.h"
#include "Natural.h"
#include "memory_management.h"
#include "function.h"
#include "statement.h"


int main()
{
	Memory_registry* Memory = Memory_registry::get_register();

	Finite_set* n0 = Natural(0);
	Finite_set* n1 = Natural(1);
	Finite_set* n2 = Natural(2);
	Finite_set* n3 = Natural(3);

	auto N = Memory->create_finite_set("N", n0, n1, n2, n3);

	std::cout << N->get_cardinality() << std::endl;

	Function_values fv;
	fv.add_value(n0, n1);
	fv.add_value(n1, n2);
	fv.add_value(n2, n3);
	fv.add_value(n3, n0);

	Function rotation(N, N, fv);

	std::cout << rotation.apply("Natural 3") << std::endl;

}
