#include <boost/assert.hpp>
#include <boost/test/included/unit_test.hpp>

void test2() {
	BOOST_ASSERT(1 == 1);
}



int main(){
	BOOST_TEST_CASE(test2);
	return 0;
}
